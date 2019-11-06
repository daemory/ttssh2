/*
 * (C) 2019 TeraTerm Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <windows.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "tttypes.h"
#include "ttcommon.h"
#include "ftdlg_lite.h"

#include "sendmem.h"

// 送信中にVTWINに排他をかける
#define	USE_ENABLE_WINDOW	0	// 1=排他する

typedef struct SendMemTag {
	const BYTE *send_ptr;  // 送信データへのポインタ
	size_t send_len;	   // 送信データサイズ
	SendMemType type;
	BOOL local_echo_enable;
	BOOL send_enable;
	DWORD delay_per_line;  // (ms)
	DWORD delay_per_char;
	HWND hWnd;	 // タイマーを受けるwindow
	int timer_id;  // タイマーID
	char *UILanguageFile;
	wchar_t *dialog_caption;
	wchar_t *filename;
	//
	size_t send_left;
	size_t send_index;
	BOOL waited;
	DWORD last_send_tick;
	//
	CFileTransLiteDlg *dlg;
	class SendMemDlgObserver *dlg_observer;
	HINSTANCE hInst_;
	HWND hWndParent_;
	//
	PComVar cv_;
	BOOL pause;
} SendMem;

typedef SendMem sendmem_work_t;

extern "C" IdTalk TalkStatus;
extern "C" HWND HVTWin;

static sendmem_work_t *sendmem_work;

class SendMemDlgObserver : public CFileTransLiteDlg::Observer {
public:
	SendMemDlgObserver(HWND hWndParent, void (*OnClose)(), void (*OnPause)(BOOL paused)) {
		hWndParent_ = hWndParent;
		OnClose_ = OnClose;
		OnPause_ = OnPause;
	}
private:
	void OnClose()
	{
		OnClose_();
	};
	void OnPause(BOOL paused)
	{
		OnPause_(paused);
	};
	void OnHelp()
	{
		MessageBoxA(hWndParent_, "no help topic", "Tera Term", MB_OK);
	}
	HWND hWndParent_;
	void (*OnClose_)();
	void (*OnPause_)(BOOL paused);
};

static wchar_t *wcsnchr(const wchar_t *str, size_t len, wchar_t chr)
{
	for (size_t i = 0; i < len; ++i) {
		wchar_t c = *str;
		if (c == 0) {
			return NULL;
		}
		if (c == chr) {
			return (wchar_t *)str;
		}
		str++;
	}
	return NULL;
}

static void EndPaste()
{
	sendmem_work_t *p = sendmem_work;
	p->send_ptr = NULL;

	TalkStatus = IdTalkKeyb;
	if (p->dlg != NULL) {
		p->dlg->Destroy();
		delete p->dlg;
		delete p->dlg_observer;
	}
	free(p->UILanguageFile);
	free(p->dialog_caption);
	free(p->filename);
	free(p);

	sendmem_work = NULL;

	// 操作できるようにする
#if USE_ENABLE_WINDOW
	EnableWindow(HVTWin, TRUE);
	SetFocus(HVTWin);
#endif
}

static void OnClose()
{
	EndPaste();
}

static void OnPause(BOOL paused)
{
	sendmem_work_t *p = sendmem_work;
	p->pause = paused;
	if (!paused) {
		// ポーズ解除時, タイマーイベントで再送のトリガを引く
		SetTimer(p->hWnd, p->timer_id, 0, NULL);
	}
}

static void SendMemStart_i(SendMem *sm)
{
	sendmem_work = sm;
	sendmem_work_t *p = sm;

	p->send_left = p->send_len;
	p->send_index = 0;
	p->waited = FALSE;
	p->pause = FALSE;

	if (p->hWndParent_ != NULL) {
		// ダイアログを生成する
		p->dlg = new CFileTransLiteDlg();
		p->dlg->Create(NULL, NULL, sm->UILanguageFile);
		if (p->dialog_caption != NULL) {
			p->dlg->SetCaption(p->dialog_caption);
		}
		if (p->filename != NULL) {
			p->dlg->SetFilename(p->filename);
		}
		p->dlg_observer = new SendMemDlgObserver(p->hWndParent_, OnClose, OnPause);
		p->dlg->SetObserver(p->dlg_observer);
	}

	TalkStatus = IdTalkSendMem;

	// 送信開始時にウィンドウを操作できないようにする
#if USE_ENABLE_WINDOW
	EnableWindow(HVTWin, FALSE);
#endif
}

static void GetOutBuffInfo(const TComVar *cv_, size_t *use, size_t *free)
{
	if (use != NULL) {
		*use = cv_->OutBuffCount;
	}
	if (free != NULL) {
		*free = OutBuffSize - cv_->InBuffCount;
	}
}

static void GetInBuffInfo(const TComVar *cv_, size_t *use, size_t *free)
{
	if (use != NULL) {
		*use = cv_->InBuffCount;
	}
	if (free != NULL) {
		*free = InBuffSize - cv_->InBuffCount;
	}
}

/**
 * 送信
 */
void SendMemContinuously(void)
{
	sendmem_work_t *p = sendmem_work;

	if (p->send_ptr == NULL) {
		EndPaste();
		return;
	}

	if (p->pause) {
		return;
	}

	// 終端?
	if (p->send_left == 0) {
		// 終了, 送信バッファが空になるまで待つ
		size_t out_buff_use;
		GetOutBuffInfo(p->cv_, &out_buff_use, NULL);

		if (p->dlg != NULL) {
			p->dlg->RefreshNum(p->send_index, p->send_len - out_buff_use);
		}

		if (out_buff_use == 0) {
			// 送信バッファも空になった
			EndPaste();
			return;
		}
	}

	if (p->waited) {
		const DWORD delay = p->delay_per_line > 0 ? p->delay_per_line : p->delay_per_char;
		if (GetTickCount() - p->last_send_tick < delay) {
			// ウエイトする
			return;
		}
	}

	// 送信できるバッファサイズ
	size_t buff_len;
	{
		size_t out_buff_free;
		GetOutBuffInfo(p->cv_, NULL, &out_buff_free);
		buff_len = out_buff_free;
		if (p->local_echo_enable) {
			// ローカルエコーが必要な場合は、入力バッファも考慮
			size_t in_buff_free;
			GetInBuffInfo(p->cv_, NULL, &in_buff_free);
			if (buff_len > in_buff_free) {
				buff_len = in_buff_free;
			}
		}
	}
	if (buff_len == 0) {
		// バッファに空きがない
		return;
	}

	// 送信長
	BOOL need_delay = FALSE;
	size_t send_len;
	if (p->delay_per_char > 0) {
		// 1キャラクタ送信
		need_delay = TRUE;
		if (p->type == SendMemTypeBinary) {
			send_len = 1;
		}
		else {
			send_len = sizeof(wchar_t);
		}
	}
	else if (p->delay_per_line > 0) {
		// 1ライン送信
		need_delay = TRUE;

		// 1行取り出し(改行コードは 0x0a に正規化されている)
		const wchar_t *line_top = (wchar_t *)&p->send_ptr[p->send_index];
		const wchar_t *line_end = wcsnchr(line_top, p->send_left, 0x0a);
		if (line_end != NULL) {
			// 0x0a まで送信
			send_len = ((line_end - line_top) + 1) * sizeof(wchar_t);
		}
		else {
			// 改行見つからず、最後まで送信
			send_len = p->send_left;
		}

		// 1行が送信バッファより大きい
		if (buff_len < send_len) {
			// 送信バッファ長まで切り詰める
			send_len = buff_len;
			return;
		}
	}
	else {
		// 全力送信
		send_len = p->send_left;
		if (buff_len < send_len) {
			send_len = buff_len;
		}
	}

	// 送信する
	const BYTE *send_ptr = (BYTE *)&p->send_ptr[p->send_index];
	p->send_index += send_len;
	p->send_left -= send_len;
	size_t sended_len;
	if (p->type == SendMemTypeBinary) {
		sended_len = CommBinaryBuffOut(p->cv_, (PCHAR)send_ptr, (int)send_len);
		if (p->local_echo_enable) {
			CommTextEcho(p->cv_, (PCHAR)send_ptr, (int)send_len);
		}
	}
	else {
		sended_len = CommTextOutW(p->cv_, (wchar_t *)send_ptr, (int)(send_len / sizeof(wchar_t)));
		if (p->local_echo_enable) {
			CommTextEchoW(p->cv_, (wchar_t *)send_ptr, (int)(send_len / sizeof(wchar_t)));
		}
		sended_len *= sizeof(wchar_t);
	}

	// ダイアログ更新
	if (p->dlg != NULL) {
		size_t out_buff_use;
		GetOutBuffInfo(p->cv_, &out_buff_use, NULL);
		p->dlg->RefreshNum(p->send_index - out_buff_use, p->send_len);
	}

	if (p->send_left != 0 && need_delay) {
		// waitに入る
		p->waited = TRUE;
		p->last_send_tick = GetTickCount();
		// タイマーはidleを動作させるために使用している
		const DWORD delay = p->delay_per_line > 0 ? p->delay_per_line : p->delay_per_char;
		SetTimer(p->hWnd, p->timer_id, delay, NULL);
	}
}

/*
 *	改行コードをLF(0x0a)だけにする
 *
 *	@param [in]	*src_		文字列へのポインタ
 *	@param [in] *len		入力文字列長(0のとき内部で文字列長を測る)
 *	@param [out] *len		出力文字列長
 *	@return					変換後文字列(mallocされた領域)
 */
static wchar_t *NormalizeLineBreak(const wchar_t *src, size_t *len)
{
	size_t src_len = *len;
	if (src_len == 0) {
		return NULL;
	}
	wchar_t *dest_top = (wchar_t *)malloc(sizeof(wchar_t) * src_len);
	if (dest_top == NULL) {
		return NULL;
	}

	// LF(0x0a),CR(0x0d)があるか調べる
	int cr_count = 0;
	int lf_count = 0;
	const wchar_t *p = src;
	for (size_t i = 0; i < src_len; i++) {
		wchar_t c = *p++;
		if (c == CR) {	// 0x0d
			cr_count++;
		} else if (c == LF) {	// 0x0d
			lf_count++;
		}
	}

	wchar_t *dest = dest_top;
	if (lf_count == 0 && cr_count != 0) {
		// LFなし、CRのみ
		// CRをLFに変換する
		for (size_t i = 0; i < src_len; i++) {
			wchar_t c = *src++;
			*dest++ = (c == CR) ? LF : c;
		}
	} else if (lf_count != 0 && cr_count != 0) {
		// CR、LFともあり
		// CRを捨てる
		for (size_t i = 0; i < src_len; i++) {
			wchar_t c = *src++;
			if (c == CR) {
				continue;
			}
			*dest++ = c;
		}
	} else {
		// CRのみ or 改行が全くない
		// 変換不要
		memcpy(dest , src, sizeof(wchar_t) * src_len);
		dest += src_len;
	}

	*len = dest - dest_top;
	return dest_top;
}

/**
 *	メモリにあるデータを送信する
 *	データは送信終了後にfree()される
 *
 *	@param	ptr		データへポインタ(malloc()された領域)
 *					送信後(中断後)、自動的にfree()される
 *	@param	len		データ長(byte)
 *					文字列(wchar_t)の場合もbyte数
 *	@param	type	文字列(wchar_t,LF or CRLF or バイナリ)
 */
SendMem *SendMemInit(void *ptr, size_t len, SendMemType type)
{
	if (sendmem_work != NULL) {
		// 送信中
		return NULL;
	}
	SendMem *p = (SendMem *)calloc(sizeof(*p), 1);
	if (p == NULL) {
		return NULL;
	}
	if (type == SendMemTypeTextCRLF || type == SendMemTypeTextLF) {
		// 改行コードを調整しておく
		size_t new_len = len / sizeof(wchar_t);
		p->send_ptr = (BYTE *)NormalizeLineBreak((wchar_t *)ptr, &new_len);
		p->send_len = new_len * sizeof(wchar_t);
		free(ptr);
	} else {
		p->send_ptr = (BYTE *)ptr;
		p->send_len = len;
	}
	if (p->send_ptr == NULL) {
		return NULL;
	}
	p->type = type;
	p->local_echo_enable = FALSE;
	p->delay_per_char = 0;  // (ms)
	p->delay_per_line = 0;  // (ms)
	p->cv_ = NULL;
	p->hWnd = HVTWin;		// delay時に使用するタイマー用
	p->timer_id = IdPasteDelayTimer;
	p->hWndParent_ = NULL;
	return p;
}

void SendMemInitEcho(SendMem *sm, BOOL echo)
{
	sm->local_echo_enable = echo;
}

void SendMemInitDelay(SendMem* sm, DWORD per_line, DWORD per_char)
{
	sm->delay_per_char = per_char;	// (ms)
	sm->delay_per_line = per_line;
}

// セットするとダイアログが出る
void SendMemInitDialog(SendMem *sm, HINSTANCE hInstance, HWND hWndParent, const char *UILanguageFile)
{
	sm->hInst_ = hInstance;
	sm->hWndParent_ = hWndParent;
	sm->UILanguageFile = _strdup(UILanguageFile);
}

void SendMemInitDialogCaption(SendMem *sm, const wchar_t *caption)
{
	if (sm->dialog_caption != NULL)
		free(sm->dialog_caption);
	sm->dialog_caption = _wcsdup(caption);
}

void SendMemInitDialogFilename(SendMem *sm, const wchar_t *filename)
{
	if (sm->filename != NULL)
		free(sm->filename);
	sm->filename = _wcsdup(filename);
}

extern "C" TComVar cv;
BOOL SendMemStart(SendMem *sm)
{
	// 送信中チェック
	if (sendmem_work != NULL) {
		return FALSE;
	}

	sm->cv_ = &cv;		// TODO なくしたい
	SendMemStart_i(sm);
	return TRUE;
}

void SendMemFinish(SendMem *sm)
{
	free(sm->UILanguageFile);
	free(sm);
}
