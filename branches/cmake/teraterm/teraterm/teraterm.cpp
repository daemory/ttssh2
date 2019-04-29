/*
 * Copyright (C) 1994-1998 T. Teranishi
 * (C) 2006-2018 TeraTerm Project
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

/* TERATERM.EXE, main */

#include <crtdbg.h>
#include <tchar.h>
#include "teraterm.h"
#include "tttypes.h"
#include "commlib.h"
#include "ttwinman.h"
#include "buffer.h"
#include "vtterm.h"
#include "vtwin.h"
#include "clipboar.h"
#include "ttftypes.h"
#include "filesys.h"
#include "telnet.h"
#include "tektypes.h"
#include "tekwin.h"
#include "ttdde.h"
#include "keyboard.h"
#include "dllutil.h"
#include "compat_win.h"
#include "compat_w95.h"
#include "dlglib.h"
#include "teraterml.h"



typedef struct {
	const TCHAR *FaceName;
	bool found;
} EnumFontInfo;

static int CALLBACK EnumFontExProc(
	ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm,
	int nFontType, LPARAM lParam)
{
	EnumFontInfo *info = (EnumFontInfo *)lParam;
	if (nFontType == DEVICE_FONTTYPE) {
		// �ڑ����ꂽ�f�o�C�X(�v�����^�Ȃ�)���̃t�H���g
		return 1;
	}
	const TCHAR *FaceName = lpelf->elfLogFont.lfFaceName;
	if (_tcscmp(info->FaceName, FaceName) == 0) {
		info->found = true;
		return 0;
	}
	return 1;
}

BOOL isExistFont(const TCHAR *FaceName)
{
	HDC hDC = GetDC(NULL);
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfCharSet = DEFAULT_CHARSET;// SHIFTJIS_CHARSET;
	lf.lfPitchAndFamily = 0;

	EnumFontInfo info;
	info.FaceName = FaceName;
	info.found = false;
	EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC)EnumFontExProc, (LPARAM)&info, 0);
	ReleaseDC(NULL, hDC);
	return info.found;
}

static BOOL AddFontFlag;
static TCHAR TSpecialFont[MAX_PATH];

static void LoadSpecialFont()
{
	if (!isExistFont(_T("Tera Special"))) {
		int r;

		if (GetModuleFileName(NULL, TSpecialFont,_countof(TSpecialFont)) == 0) {
			AddFontFlag = FALSE;
			return;
		}
		*_tcsrchr(TSpecialFont, _T('\\')) = 0;
		_tcscat_s(TSpecialFont, _T("\\TSPECIAL1.TTF"));

		if (pAddFontResourceEx != NULL) {
			// teraterm.exe�݂̂ŗL���ȃt�H���g�ƂȂ�B
			// remove���Ȃ��Ă��I�������OS����Ȃ��Ȃ�
			r = pAddFontResourceEx(TSpecialFont, FR_PRIVATE, NULL);
		} else {
			// �V�X�e���S�̂Ŏg����t�H���g�ƂȂ�
			// remove���Ȃ���OS���͂񂾂܂܂ƂȂ�
			r = AddFontResource(TSpecialFont);
		}
		if (r != 0) {
			AddFontFlag = TRUE;
		}
	}
}

static void UnloadSpecialFont()
{
	if (AddFontFlag) {
		if (pRemoveFontResourceEx != NULL) {
			pRemoveFontResourceEx(TSpecialFont, FR_PRIVATE, NULL);
		} else {
			RemoveFontResource(TSpecialFont);
		}
	}
}

static void init()
{
	DLLInit();
	WinCompatInit();
	if (pSetThreadDpiAwarenessContext) {
		pSetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	}
	LoadSpecialFont();
}

// Tera Term main engine
static BOOL OnIdle(LONG lCount)
{
	static int Busy = 2;
	int Change, nx, ny;
	BOOL Size;

	if (lCount==0) Busy = 2;

	if (cv.Ready)
	{
		/* Sender */
		CommSend(&cv);

		/* Parser */
		if ((cv.HLogBuf!=NULL) && (cv.LogBuf==NULL))
			cv.LogBuf = (PCHAR)GlobalLock(cv.HLogBuf);

		if ((cv.HBinBuf!=NULL) && (cv.BinBuf==NULL))
			cv.BinBuf = (PCHAR)GlobalLock(cv.HBinBuf);

		if ((TelStatus==TelIdle) && cv.TelMode)
			TelStatus = TelIAC;

		if (TelStatus != TelIdle)
		{
			ParseTel(&Size,&nx,&ny);
			if (Size) {
				LockBuffer();
				ChangeTerminalSize(nx,ny);
				UnlockBuffer();
			}
		}
		else {
			if (cv.ProtoFlag) Change = ProtoDlgParse();
			else {
				switch (ActiveWin) {
				case IdVT:
					Change =  ((CVTWindow*)pVTWin)->Parse();
					// TEK window�̃A�N�e�B�u���� pause ���g���ƁACPU�g�p��100%�ƂȂ�
					// ���ۂւ̎b��Ώ��B(2006.2.6 yutaka)
					// �҂����Ԃ��Ȃ����A�R���e�L�X�g�X�C�b�`�����ɂ���B(2006.3.20 yutaka)
					Sleep(0);
					break;

				case IdTEK:
					if (pTEKWin != NULL) {
						Change = ((CTEKWindow*)pTEKWin)->Parse();
						// TEK window�̃A�N�e�B�u���� pause ���g���ƁACPU�g�p��100%�ƂȂ�
						// ���ۂւ̎b��Ώ��B(2006.2.6 yutaka)
						Sleep(1);
					}
					else {
						Change = IdVT;
					}
					break;

				default:
					Change = 0;
				}

				switch (Change) {
					case IdVT:
						VTActivate();
						break;
					case IdTEK:
						((CVTWindow*)pVTWin)->OpenTEK();
						break;
				}
			}
		}

		if (cv.LogBuf!=NULL)
		{
			if (FileLog) {
				LogToFile();
			}
			if (DDELog && AdvFlag) {
				DDEAdv();
			}
			GlobalUnlock(cv.HLogBuf);
			cv.LogBuf = NULL;
		}

		if (cv.BinBuf!=NULL)
		{
			if (BinLog) {
				LogToFile();
			}
			GlobalUnlock(cv.HBinBuf);
			cv.BinBuf = NULL;
		}

		/* Talker */
		switch (TalkStatus) {
		case IdTalkCB:
			CBSend();
			break; /* clip board */
		case IdTalkFile:
			FileSend();
			break; /* file */
		}

		/* Receiver */
		if (DDELog && cv.DCount >0) {
			// ���O�o�b�t�@���܂�DDE�N���C�A���g�֑����Ă��Ȃ��ꍇ�́A
			// TCP�p�P�b�g�̎�M���s��Ȃ��B
			// �A�����Ď�M���s���ƁA���O�o�b�t�@�����E���h���r���ɂ�薢���M�̃f�[�^��
			// �㏑�����Ă��܂��\��������B(2007.6.14 yutaka)

		} else {
			CommReceive(&cv);
		}

	}

	if (cv.Ready &&
	    (cv.RRQ || (cv.OutBuffCount>0) || (cv.InBuffCount>0) || (cv.FlushLen>0) || (cv.LCount>0) || (cv.BCount>0) || (cv.DCount>0)) ) {
		Busy = 2;
	}
	else {
		Busy--;
	}

	return (Busy>0);
}

BOOL CallOnIdle(LONG lCount)
{
	return OnIdle(lCount);
}

HINSTANCE GetInstance()
{
	return hInst;
}

static HWND main_window;
HWND GetHWND()
{
	return main_window;
}

static HWND hModalWnd;

void AddModalHandle(HWND hWnd)
{
	hModalWnd = hWnd;
}

void RemoveModalHandle(HWND hWnd)
{
	hModalWnd = 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
                   LPSTR lpszCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	LONG lCount = 0;
	DWORD SleepTick = 1;
	init();
	hInst = hInstance;
	CVTWindow *m_pMainWnd = new CVTWindow();
	pVTWin = m_pMainWnd;
	main_window = m_pMainWnd->m_hWnd;
	SetDialogFont(ts.SetupFName, ts.UILanguageFile, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (hModalWnd != 0) {
			if (IsDialogMessage(hModalWnd, &msg)) {
				continue;
			}
		}

		bool message_processed = false;

		if (m_pMainWnd->m_hAccel != NULL) {
			if (!MetaKey(ts.MetaKey)) {
				// matakey��������Ă��Ȃ�
				if (TranslateAccelerator(m_pMainWnd->m_hWnd , m_pMainWnd->m_hAccel, &msg)) {
					// �A�N�Z�����[�^�[�L�[����������
					message_processed = true;
				}
			}
		}

		if (!message_processed) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		while (!PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) {
			// ���b�Z�[�W���Ȃ�
			if (!OnIdle(lCount)) {
				// idle�s�v
				if (SleepTick < 500) {	// �ő� 501ms����
					SleepTick += 2;
				}
				lCount = 0;
				Sleep(SleepTick);
			} else {
				// �vidle
				SleepTick = 0;
				lCount++;
			}
		}
	}
	delete m_pMainWnd;
	m_pMainWnd = NULL;

	UnloadSpecialFont();
	DLLExit();

    return msg.wParam;
}