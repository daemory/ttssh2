/* Tera Term
 Copyright(C) 1994-1998 T. Teranishi
 All rights reserved. */
/* IPv6 modification is Copyright(C) 2000 Jun-ya Kato <kato@win6.jp> */

/* TTDLG.DLL, dialog boxes */
#include "teraterm.h"
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <commdlg.h>
#include "tttypes.h"
#include "ttlib.h"
#include "dlglib.h"
#include "ttcommon.h"
#ifdef TERATERM32
  #include "dlg_res.h"
#else
  #include "dlg_re16.h"
#endif

// Oniguruma: Regular expression library
#define ONIG_EXTERN extern
#include "oniguruma.h"
#undef ONIG_EXTERN

#ifndef NO_INET6
#include <winsock2.h>
static char FAR * ProtocolFamilyList[] = { "UNSPEC", "IPv6", "IPv4", NULL };
#endif /* NO_INET6 */

static HANDLE hInst;

#ifndef NO_I18N
static HFONT DlgAboutFont;
static HFONT DlgDirFont;
static HFONT DlgFontFont;
static HFONT DlgGenFont;
static HFONT DlgHostFont;
static HFONT DlgKeybFont;
static HFONT DlgSerialFont;
static HFONT DlgTcpipFont;
static HFONT DlgTermFont;
static HFONT DlgWinFont;
static HFONT DlgWinlistFont;

char UILanguageFile[MAX_PATH];
#endif

static PCHAR far NLListRcv[] = {"CR","CR+LF", "LF", NULL};
static PCHAR far NLList[] = {"CR","CR+LF", NULL};
static PCHAR far TermList[] =
  {"VT100","VT101","VT102","VT282","VT320","VT382",NULL};
static WORD TermJ_Term[] = {1,1,2,3,3,4,4,5,6};
static WORD Term_TermJ[] = {1,3,4,7,8,9};
static PCHAR far TermListJ[] =
  {"VT100","VT100J","VT101","VT102","VT102J","VT220J",
   "VT282","VT320","VT382",NULL};
static PCHAR far KanjiList[] = {"SJIS","EUC","JIS", "UTF-8", "UTF-8m", NULL};
static PCHAR far KanjiListSend[] = {"SJIS","EUC","JIS", "UTF-8", NULL};
static PCHAR far KanjiInList[] = {"^[$@","^[$B",NULL};
static PCHAR far KanjiOutList[] = {"^[(B","^[(J",NULL};
static PCHAR far KanjiOutList2[] = {"^[(B","^[(J","^[(H",NULL};
static PCHAR far RussList[] = {"Windows","KOI8-R","CP 866","ISO 8859-5",NULL};
static PCHAR far RussList2[] = {"Windows","KOI8-R",NULL};
static PCHAR far LocaleList[] = {"japanese","chinese", "chinese-simplified", "chinese-traditional", NULL};

BOOL CALLBACK TermDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
	PTTSet ts;
	WORD w;
	//  char Temp[HostNameMaxLength + 1]; // 81(yutaka)
	char Temp[81]; // 81(yutaka)
#ifndef NO_I18N
	char uimsg[MAX_UIMSG];
	LOGFONT logfont;
	HFONT font;
#endif

	switch (Message) {
	case WM_INITDIALOG:
		ts = (PTTSet)lParam;
		SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
		font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
		GetObject(font, sizeof(LOGFONT), &logfont);
		if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgTermFont, UILanguageFile)) {
			SendDlgItemMessage(Dialog, IDC_TERMWIDTHLABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMWIDTH, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMX, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMHEIGHT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMISWIN, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMRESIZE, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMNEWLINE, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMCRRCVLABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMCRRCV, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMCRSENDLABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMCRSEND, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMIDLABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMID, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMLOCALECHO, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMANSBACKTEXT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMANSBACK, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMAUTOSWITCH, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERMHELP, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			if (ts->Language==IdJapanese) {
				SendDlgItemMessage(Dialog, IDC_TERMKANJILABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKANJI, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKANA, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKANJISENDLABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKANJISEND, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKANASEND, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKINTEXT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKIN, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKOUTTEXT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMKOUT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_LOCALE_LABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_LOCALE_EDIT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_CODEPAGE_LABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_CODEPAGE_EDIT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			}
			if (ts->Language==IdRussian) {
				SendDlgItemMessage(Dialog, IDC_TERMRUSSCHARSET, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMRUSSHOSTLABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMRUSSHOST, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMRUSSCLIENTLABEL, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
				SendDlgItemMessage(Dialog, IDC_TERMRUSSCLIENT, WM_SETFONT, (WPARAM)DlgTermFont, MAKELPARAM(TRUE,0));
			}
		}
		else {
			DlgTermFont = NULL;
		}

		GetWindowText(Dialog, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_TITLE", uimsg, UILanguageFile);
		SetWindowText(Dialog, uimsg);
		GetDlgItemText(Dialog, IDC_TERMWIDTHLABEL, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_WIDTHLABEL", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMWIDTHLABEL, uimsg);
		GetDlgItemText(Dialog, IDC_TERMISWIN, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_ISWIN", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMISWIN, uimsg);
		GetDlgItemText(Dialog, IDC_TERMRESIZE, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_RESIZE", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMRESIZE, uimsg);
		GetDlgItemText(Dialog, IDC_TERMNEWLINE, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_NEWLINE", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMNEWLINE, uimsg);
		GetDlgItemText(Dialog, IDC_TERMCRRCVLABEL, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_CRRCV", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMCRRCVLABEL, uimsg);
		GetDlgItemText(Dialog, IDC_TERMCRSENDLABEL, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_CRSEND", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMCRSENDLABEL, uimsg);
		GetDlgItemText(Dialog, IDC_TERMIDLABEL, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_ID", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMIDLABEL, uimsg);
		GetDlgItemText(Dialog, IDC_TERMLOCALECHO, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_LOCALECHO", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMLOCALECHO, uimsg);
		GetDlgItemText(Dialog, IDC_TERMANSBACKTEXT, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_ANSBACK", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMANSBACKTEXT, uimsg);
		GetDlgItemText(Dialog, IDC_TERMAUTOSWITCH, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_TERM_AUTOSWITCH", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMAUTOSWITCH, uimsg);
		GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
		get_lang_msg("BTN_OK", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDOK, uimsg);
		GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
		get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDCANCEL, uimsg);
		GetDlgItemText(Dialog, IDC_TERMHELP, uimsg, sizeof(uimsg));
		get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
		SetDlgItemText(Dialog, IDC_TERMHELP, uimsg);
		if ( ts->Language==IdJapanese ) {
			GetDlgItemText(Dialog, IDC_TERMKANJILABEL, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_KANJI", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMKANJILABEL, uimsg);
			GetDlgItemText(Dialog, IDC_TERMKANA, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_KANA", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMKANA, uimsg);
			GetDlgItemText(Dialog, IDC_TERMKANJISENDLABEL, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_KANJISEND", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMKANJISENDLABEL, uimsg);
			GetDlgItemText(Dialog, IDC_TERMKANASEND, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_KANASEND", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMKANASEND, uimsg);
			GetDlgItemText(Dialog, IDC_TERMKINTEXT, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_KIN", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMKINTEXT, uimsg);
			GetDlgItemText(Dialog, IDC_TERMKOUTTEXT, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_KOUT", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMKOUTTEXT, uimsg);
			GetDlgItemText(Dialog, IDC_LOCALE_LABEL, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_LOCALE", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_LOCALE_LABEL, uimsg);
			GetDlgItemText(Dialog, IDC_CODEPAGE_LABEL, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_CODEPAGE", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_CODEPAGE_LABEL, uimsg);
		}
		else if ( ts->Language==IdRussian ) {
			GetDlgItemText(Dialog, IDC_TERMRUSSCHARSET, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_RUSSCHARSET", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMRUSSCHARSET, uimsg);
			GetDlgItemText(Dialog, IDC_TERMRUSSHOSTLABEL, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_RUSSHOST", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMRUSSHOSTLABEL, uimsg);
			GetDlgItemText(Dialog, IDC_TERMRUSSCLIENTLABEL, uimsg, sizeof(uimsg));
			get_lang_msg("DLG_TERM_RUSSCLIENT", uimsg, UILanguageFile);
			SetDlgItemText(Dialog, IDC_TERMRUSSCLIENTLABEL, uimsg);
		}
#endif

		SetDlgItemInt(Dialog,IDC_TERMWIDTH,ts->TerminalWidth,FALSE);
		SendDlgItemMessage(Dialog, IDC_TERMWIDTH, EM_LIMITTEXT,3, 0);

		SetDlgItemInt(Dialog,IDC_TERMHEIGHT,ts->TerminalHeight,FALSE);
		SendDlgItemMessage(Dialog, IDC_TERMHEIGHT, EM_LIMITTEXT,3, 0);

		SetRB(Dialog,ts->TermIsWin,IDC_TERMISWIN,IDC_TERMISWIN);
		SetRB(Dialog,ts->AutoWinResize,IDC_TERMRESIZE,IDC_TERMRESIZE);
		if ( ts->TermIsWin>0 )
			DisableDlgItem(Dialog,IDC_TERMRESIZE,IDC_TERMRESIZE);

		SetDropDownList(Dialog, IDC_TERMCRRCV, NLListRcv, ts->CRReceive); // add 'LF' (2007.1.21 yutaka)
		SetDropDownList(Dialog, IDC_TERMCRSEND, NLList, ts->CRSend);

		if ( ts->Language!=IdJapanese ) /* non-Japanese mode */
		{
			if ((ts->TerminalID>=1) &&
				(ts->TerminalID<=9))
				w = TermJ_Term[ts->TerminalID-1];
			else
				w = 1;
			SetDropDownList(Dialog, IDC_TERMID, TermList, w);
		}
		else
			SetDropDownList(Dialog, IDC_TERMID, TermListJ, ts->TerminalID);

		SetRB(Dialog,ts->LocalEcho,IDC_TERMLOCALECHO,IDC_TERMLOCALECHO);

		if ((ts->FTFlag & FT_BPAUTO)!=0)
			DisableDlgItem(Dialog,IDC_TERMANSBACKTEXT,IDC_TERMANSBACK);
		else {
			Str2Hex(ts->Answerback,Temp,ts->AnswerbackLen,
				sizeof(Temp)-1,FALSE);
			SetDlgItemText(Dialog, IDC_TERMANSBACK, Temp);
			SendDlgItemMessage(Dialog, IDC_TERMANSBACK, EM_LIMITTEXT,
				sizeof(Temp) - 1, 0);
		}

		SetRB(Dialog,ts->AutoWinSwitch,IDC_TERMAUTOSWITCH,IDC_TERMAUTOSWITCH);

		if (ts->Language==IdJapanese)
		{
			SetDropDownList(Dialog, IDC_TERMKANJI, KanjiList, ts->KanjiCode);
			if ( ts->KanjiCode!=IdJIS )
				DisableDlgItem(Dialog,IDC_TERMKANA,IDC_TERMKANA);
			SetRB(Dialog,ts->JIS7Katakana,IDC_TERMKANA,IDC_TERMKANA);
			SetDropDownList(Dialog, IDC_TERMKANJISEND, KanjiListSend, ts->KanjiCodeSend);
			if ( ts->KanjiCodeSend!=IdJIS )
				DisableDlgItem(Dialog,IDC_TERMKANASEND,IDC_TERMKOUT);
			SetRB(Dialog,ts->JIS7KatakanaSend,IDC_TERMKANASEND,IDC_TERMKANASEND);
			SetDropDownList(Dialog,IDC_TERMKIN,KanjiInList,ts->KanjiIn);
			if ((ts->TermFlag & TF_ALLOWWRONGSEQUENCE)!=0)
				SetDropDownList(Dialog,IDC_TERMKOUT,KanjiOutList2,ts->KanjiOut);
			else
				SetDropDownList(Dialog,IDC_TERMKOUT,KanjiOutList,ts->KanjiOut);

			// ���P�[���p�e�L�X�g�{�b�N�X
			SetDlgItemText(Dialog, IDC_LOCALE_EDIT, ts->Locale);
			SendDlgItemMessage(Dialog, IDC_LOCALE_EDIT, EM_LIMITTEXT, sizeof(ts->Locale), 0);

			SetDlgItemInt(Dialog, IDC_CODEPAGE_EDIT, ts->CodePage, FALSE);
			//SendDlgItemMessage(Dialog, IDC_CODEPAGE_EDIT, EM_LIMITTEXT, 16, 0);

		}
		else if (ts->Language==IdRussian)
		{
			SetDropDownList(Dialog,IDC_TERMRUSSHOST,RussList,ts->RussHost);
			SetDropDownList(Dialog,IDC_TERMRUSSCLIENT,RussList,ts->RussClient);
		}
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
	case IDOK:
		ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);

		if ( ts!=NULL )
		{
			ts->TerminalWidth = GetDlgItemInt(Dialog,IDC_TERMWIDTH,NULL,FALSE);
			if ( ts->TerminalWidth<1 ) ts->TerminalWidth = 1;
			if ( ts->TerminalWidth>500 ) ts->TerminalWidth = 500;

			ts->TerminalHeight = GetDlgItemInt(Dialog,IDC_TERMHEIGHT,NULL,FALSE);
			if ( ts->TerminalHeight<1 ) ts->TerminalHeight = 1;
			if ( ts->TerminalHeight>500 ) ts->TerminalHeight = 500;

			GetRB(Dialog,&ts->TermIsWin,IDC_TERMISWIN,IDC_TERMISWIN);
			GetRB(Dialog,&ts->AutoWinResize,IDC_TERMRESIZE,IDC_TERMRESIZE);

			ts->CRReceive = (WORD)GetCurSel(Dialog, IDC_TERMCRRCV);
			ts->CRSend = (WORD)GetCurSel(Dialog, IDC_TERMCRSEND);

			w = (WORD)GetCurSel(Dialog, IDC_TERMID);
			if ( ts->Language!=IdJapanese ) /* non-Japanese mode */
			{
				if ((w==0) || (w>6)) w = 1;
				w = Term_TermJ[w-1];
			}
			ts->TerminalID = w;

			GetRB(Dialog,&ts->LocalEcho,IDC_TERMLOCALECHO,IDC_TERMLOCALECHO);

			if ((ts->FTFlag & FT_BPAUTO)==0)
			{
				GetDlgItemText(Dialog, IDC_TERMANSBACK,Temp,sizeof(Temp));
				ts->AnswerbackLen =
					Hex2Str(Temp,ts->Answerback,sizeof(ts->Answerback));
			}

			GetRB(Dialog,&ts->AutoWinSwitch,IDC_TERMAUTOSWITCH,IDC_TERMAUTOSWITCH);

			if (ts->Language==IdJapanese)
			{
				BOOL ret;

				ts->KanjiCode = (WORD)GetCurSel(Dialog, IDC_TERMKANJI);
				GetRB(Dialog,&ts->JIS7Katakana,IDC_TERMKANA,IDC_TERMKANA);
				ts->KanjiCodeSend = (WORD)GetCurSel(Dialog, IDC_TERMKANJISEND);
				GetRB(Dialog,&ts->JIS7KatakanaSend,IDC_TERMKANASEND,IDC_TERMKANASEND);
				ts->KanjiIn = (WORD)GetCurSel(Dialog, IDC_TERMKIN);
				ts->KanjiOut = (WORD)GetCurSel(Dialog, IDC_TERMKOUT);

				GetDlgItemText(Dialog, IDC_LOCALE_EDIT, ts->Locale, sizeof(ts->Locale));
				ts->CodePage = GetDlgItemInt(Dialog, IDC_CODEPAGE_EDIT, &ret, FALSE);
			}
			else if (ts->Language==IdRussian)
			{
				ts->RussHost = (WORD)GetCurSel(Dialog, IDC_TERMRUSSHOST);
				ts->RussClient = (WORD)GetCurSel(Dialog, IDC_TERMRUSSCLIENT);
			}
		}
		EndDialog(Dialog, 1);
#ifndef NO_I18N
		if (DlgTermFont != NULL) {
			DeleteObject(DlgTermFont);
		}
#endif
		return TRUE;

	case IDCANCEL:
		EndDialog(Dialog, 0);
#ifndef NO_I18N
		if (DlgTermFont != NULL) {
			DeleteObject(DlgTermFont);
		}
#endif
		return TRUE;

	case IDC_TERMISWIN:
		GetRB(Dialog,&w,IDC_TERMISWIN,IDC_TERMISWIN);
		if ( w==0 )
			EnableDlgItem(Dialog,IDC_TERMRESIZE,IDC_TERMRESIZE);
		else
			DisableDlgItem(Dialog,IDC_TERMRESIZE,IDC_TERMRESIZE);
		break;

	case IDC_TERMKANJI:
		w = (WORD)GetCurSel(Dialog, IDC_TERMKANJI);
		if (w==IdJIS)
			EnableDlgItem(Dialog,IDC_TERMKANA,IDC_TERMKANA);
		else
			DisableDlgItem(Dialog,IDC_TERMKANA,IDC_TERMKANA);
		break;

	case IDC_TERMKANJISEND:
		w = (WORD)GetCurSel(Dialog, IDC_TERMKANJISEND);
		if (w==IdJIS)
			EnableDlgItem(Dialog,IDC_TERMKANASEND,IDC_TERMKOUT);
		else
			DisableDlgItem(Dialog,IDC_TERMKANASEND,IDC_TERMKOUT);
		break;

	case IDC_TERMHELP:
		PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
		break;
		}
	}
	return FALSE;
}

  void DispSample(HWND Dialog, PTTSet ts, int IAttr)
  {
    int i,x,y;
    COLORREF Text, Back;
    int DX[3];
    TEXTMETRIC Metrics;
    RECT Rect, TestRect;
    int FW,FH;
    HDC DC;

    DC = GetDC(Dialog);
    Text = RGB(ts->TmpColor[IAttr][0],
	       ts->TmpColor[IAttr][1],
	       ts->TmpColor[IAttr][2]);
    Text = GetNearestColor(DC, Text);
    Back = RGB(ts->TmpColor[IAttr][3],
	       ts->TmpColor[IAttr][4],
	       ts->TmpColor[IAttr][5]);
    Back = GetNearestColor(DC, Back);
    SetTextColor(DC, Text);
    SetBkColor(DC, Back);
    SelectObject(DC,ts->SampleFont);
    GetTextMetrics(DC, &Metrics);
    FW = Metrics.tmAveCharWidth;
    FH = Metrics.tmHeight;
    for (i = 0 ; i <= 2 ; i++) DX[i] = FW;
    GetClientRect(Dialog,&Rect);
    TestRect.left = Rect.left + (int)((Rect.right-Rect.left)*0.65);
    TestRect.right = Rect.left + (int)((Rect.right-Rect.left)*0.93);
    TestRect.top = Rect.top + (int)((Rect.bottom-Rect.top)*0.54);
#ifdef USE_NORMAL_BGCOLOR
    TestRect.bottom = Rect.top + (int)((Rect.bottom-Rect.top)*0.90);
#else
    TestRect.bottom = Rect.top + (int)((Rect.bottom-Rect.top)*0.94);
#endif
    x = (int)((TestRect.left+TestRect.right) / 2 - FW * 1.5);
    y = (TestRect.top+TestRect.bottom-FH) / 2;
    ExtTextOut(DC, x,y, ETO_CLIPPED | ETO_OPAQUE,
	       &TestRect, "ABC", 3, &(DX[0]));
    ReleaseDC(Dialog,DC);
  }

  void ChangeColor(HWND Dialog, PTTSet ts, int IAttr, int IOffset)
  {
    SetDlgItemInt(Dialog,IDC_WINRED,ts->TmpColor[IAttr][IOffset],FALSE);
    SetDlgItemInt(Dialog,IDC_WINGREEN,ts->TmpColor[IAttr][IOffset+1],FALSE);
    SetDlgItemInt(Dialog,IDC_WINBLUE,ts->TmpColor[IAttr][IOffset+2],FALSE);

    DispSample(Dialog,ts,IAttr);
  }

  void ChangeSB
    (HWND Dialog, PTTSet ts, int IAttr, int IOffset)
  {
    HWND HRed, HGreen, HBlue;

    HRed = GetDlgItem(Dialog, IDC_WINREDBAR);
    HGreen = GetDlgItem(Dialog, IDC_WINGREENBAR);
    HBlue = GetDlgItem(Dialog, IDC_WINBLUEBAR);
    
    SetScrollPos(HRed,SB_CTL,ts->TmpColor[IAttr][IOffset+0],TRUE);
    SetScrollPos(HGreen,SB_CTL,ts->TmpColor[IAttr][IOffset+1],TRUE);
    SetScrollPos(HBlue,SB_CTL,ts->TmpColor[IAttr][IOffset+2],TRUE);
    ChangeColor(Dialog,ts,IAttr,IOffset);
  }

  void RestoreVar(HWND Dialog, PTTSet ts, int *IAttr, int *IOffset)
  {
    WORD w;

    GetRB(Dialog,&w,IDC_WINTEXT,IDC_WINBACK);
    if (w==2) *IOffset = 3;
	 else *IOffset = 0;
    if ((ts!=NULL) && (ts->VTFlag>0))
    {
      *IAttr = GetCurSel(Dialog,IDC_WINATTR);
      if (*IAttr>0) (*IAttr)--;
    }
    else
      *IAttr = 0;
  }

BOOL CALLBACK WinDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PTTSet ts;
  HWND Wnd, HRed, HGreen, HBlue;
  int IAttr, IOffset;
  WORD i, pos, ScrollCode, NewPos;
  HDC DC;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      ts = (PTTSet)lParam;
      SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgWinFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_WINTITLELABEL, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINTITLE, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINCURSOR, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINBLOCK, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINVERT, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINHORZ, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINHIDETITLE, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINHIDEMENU, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINCOLOREMU, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINSCROLL1, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINSCROLL3, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINSCROLL2, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINCOLOR, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINTEXT, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINBACK, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINREV, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINATTRTEXT, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINATTR, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINREDLABEL, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINRED, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINGREENLABEL, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINGREEN, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINBLUELABEL, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINBLUE, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINUSENORMALBG, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINHELP, WM_SETFONT, (WPARAM)DlgWinFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgWinFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_WINTITLELABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_TITLELABEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINTITLELABEL, uimsg);
      GetDlgItemText(Dialog, IDC_WINCURSOR, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_CURSOR", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINCURSOR, uimsg);
      GetDlgItemText(Dialog, IDC_WINBLOCK, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_BLOCK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINBLOCK, uimsg);
      GetDlgItemText(Dialog, IDC_WINVERT, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_VERT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINVERT, uimsg);
      GetDlgItemText(Dialog, IDC_WINHORZ, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_HORZ", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINHORZ, uimsg);
      GetDlgItemText(Dialog, IDC_WINHIDETITLE, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_HIDETITLE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINHIDETITLE, uimsg);
      GetDlgItemText(Dialog, IDC_WINHIDEMENU, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_HIDEMENU", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINHIDEMENU, uimsg);
      GetDlgItemText(Dialog, IDC_WINCOLOREMU, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_COLOREMU", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINCOLOREMU, uimsg);
      GetDlgItemText(Dialog, IDC_WINSCROLL1, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_SCROLL1", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINSCROLL1, uimsg);
      GetDlgItemText(Dialog, IDC_WINSCROLL3, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_SCROLL3", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINSCROLL3, uimsg);
      GetDlgItemText(Dialog, IDC_WINCOLOR, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_COLOR", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINCOLOR, uimsg);
      GetDlgItemText(Dialog, IDC_WINTEXT, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_TEXT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINTEXT, uimsg);
      GetDlgItemText(Dialog, IDC_WINBACK, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_BG", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINBACK, uimsg);
      GetDlgItemText(Dialog, IDC_WINATTRTEXT, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_ATTRIB", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINATTRTEXT, uimsg);
      GetDlgItemText(Dialog, IDC_WINREV, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_REVERSE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINREV, uimsg);
      GetDlgItemText(Dialog, IDC_WINREDLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_R", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINREDLABEL, uimsg);
      GetDlgItemText(Dialog, IDC_WINGREENLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_G", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINGREENLABEL, uimsg);
      GetDlgItemText(Dialog, IDC_WINBLUELABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_B", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINBLUELABEL, uimsg);
      GetDlgItemText(Dialog, IDC_WINUSENORMALBG, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WIN_ALWAYSBG", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINUSENORMALBG, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_WINHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINHELP, uimsg);
#endif

      SetDlgItemText(Dialog, IDC_WINTITLE, ts->Title);
      SendDlgItemMessage(Dialog, IDC_WINTITLE, EM_LIMITTEXT,
			 sizeof(ts->Title)-1, 0);

      SetRB(Dialog,ts->HideTitle,IDC_WINHIDETITLE,IDC_WINHIDETITLE);
      SetRB(Dialog,ts->PopupMenu,IDC_WINHIDEMENU,IDC_WINHIDEMENU);
      if ( ts->HideTitle>0 )
	DisableDlgItem(Dialog,IDC_WINHIDEMENU,IDC_WINHIDEMENU);

      if (ts->VTFlag>0)
      {
#ifndef NO_I18N
	strcpy(uimsg, "Full &color");
	get_lang_msg("DLG_WIN_FULLCOLOR", uimsg, UILanguageFile);
	SetDlgItemText(Dialog, IDC_WINCOLOREMU,uimsg);
#else
	SetDlgItemText(Dialog, IDC_WINCOLOREMU,"Full &color");
#endif
	if ((ts->ColorFlag & CF_FULLCOLOR)!=0)
	  i = 1;
	else
	  i = 0;
	SetRB(Dialog,i,IDC_WINCOLOREMU,IDC_WINCOLOREMU);
	ShowDlgItem(Dialog,IDC_WINSCROLL1,IDC_WINSCROLL3);
	SetRB(Dialog,ts->EnableScrollBuff,IDC_WINSCROLL1,IDC_WINSCROLL1);
	SetDlgItemInt(Dialog,IDC_WINSCROLL2,ts->ScrollBuffSize,FALSE);

	// ���͍ő包���� 5 ���� 8 �֊g�� (2004.11.28 yutaka)
	SendDlgItemMessage(Dialog, IDC_WINSCROLL2, EM_LIMITTEXT, 8, 0);

	if ( ts->EnableScrollBuff==0 )
	  DisableDlgItem(Dialog,IDC_WINSCROLL2,IDC_WINSCROLL3);
	for (i = 0 ; i <= 1 ; i++)
	{
	  ts->TmpColor[0][i*3]	 = GetRValue(ts->VTColor[i]);
	  ts->TmpColor[0][i*3+1] = GetGValue(ts->VTColor[i]);
	  ts->TmpColor[0][i*3+2] = GetBValue(ts->VTColor[i]);
	  ts->TmpColor[1][i*3]	 = GetRValue(ts->VTBoldColor[i]);
	  ts->TmpColor[1][i*3+1] = GetGValue(ts->VTBoldColor[i]);
	  ts->TmpColor[1][i*3+2] = GetBValue(ts->VTBoldColor[i]);
	  ts->TmpColor[2][i*3]	 = GetRValue(ts->VTBlinkColor[i]);
	  ts->TmpColor[2][i*3+1] = GetGValue(ts->VTBlinkColor[i]);
	  ts->TmpColor[2][i*3+2] = GetBValue(ts->VTBlinkColor[i]);
      /* begin - ishizaki */
	  ts->TmpColor[3][i*3]	 = GetRValue(ts->URLColor[i]);
	  ts->TmpColor[3][i*3+1] = GetGValue(ts->URLColor[i]);
	  ts->TmpColor[3][i*3+2] = GetBValue(ts->URLColor[i]);
      /* end - ishizaki */
	}
	ShowDlgItem(Dialog,IDC_WINATTRTEXT,IDC_WINATTR);
#ifndef NO_I18N
	strcpy(uimsg, "Normal");
	get_lang_msg("DLG_TERM_NORMAL", uimsg, UILanguageFile);
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_ADDSTRING,
			   0, (LPARAM)uimsg);
	strcpy(uimsg, "Bold");
	get_lang_msg("DLG_TERM_BOLD", uimsg, UILanguageFile);
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_ADDSTRING,
			   0, (LPARAM)uimsg);
	strcpy(uimsg, "Blink");
	get_lang_msg("DLG_TERM_BLINK", uimsg, UILanguageFile);
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_ADDSTRING,
			   0, (LPARAM)uimsg);
#else
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_ADDSTRING,
			   0, (LPARAM)"Normal");
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_ADDSTRING,
			   0, (LPARAM)"Bold");
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_ADDSTRING,
			   0, (LPARAM)"Blink");
#endif
    /* begin - ishizaki */
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_ADDSTRING,
			   0, (LPARAM)"URL");
    /* end - ishizaki */
	SendDlgItemMessage(Dialog, IDC_WINATTR, CB_SETCURSEL,
			   0,0);
      }
      else {
	for (i = 0 ; i <=1 ; i++)
	{
	  ts->TmpColor[0][i*3]	 = GetRValue(ts->TEKColor[i]);
	  ts->TmpColor[0][i*3+1] = GetGValue(ts->TEKColor[i]);
	  ts->TmpColor[0][i*3+2] = GetBValue(ts->TEKColor[i]);
	}
	SetRB(Dialog,ts->TEKColorEmu,IDC_WINCOLOREMU,IDC_WINCOLOREMU);
      }
      SetRB(Dialog,1,IDC_WINTEXT,IDC_WINBACK);

      SetRB(Dialog,ts->CursorShape,IDC_WINBLOCK,IDC_WINHORZ);

      IAttr = 0;
      IOffset = 0;

      HRed = GetDlgItem(Dialog, IDC_WINREDBAR);
      SetScrollRange(HRed,SB_CTL,0,255,TRUE);

      HGreen = GetDlgItem(Dialog, IDC_WINGREENBAR);
      SetScrollRange(HGreen,SB_CTL,0,255,TRUE);

      HBlue = GetDlgItem(Dialog, IDC_WINBLUEBAR);
      SetScrollRange(HBlue,SB_CTL,0,255,TRUE);

#ifdef USE_NORMAL_BGCOLOR
      SetRB(Dialog,ts->UseNormalBGColor,IDC_WINUSENORMALBG,IDC_WINUSENORMALBG);
#endif
	  ChangeSB(Dialog,ts,IAttr,IOffset);

      return TRUE;

    case WM_COMMAND:
      ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
      RestoreVar(Dialog,ts,&IAttr,&IOffset);
      switch (LOWORD(wParam)) {
	case IDOK:
	  if ( ts!=NULL )
	  {
	    GetDlgItemText(Dialog,IDC_WINTITLE,ts->Title,sizeof(ts->Title));
	    GetRB(Dialog,&ts->HideTitle,IDC_WINHIDETITLE,IDC_WINHIDETITLE);
	    GetRB(Dialog,&ts->PopupMenu,IDC_WINHIDEMENU,IDC_WINHIDEMENU);
	    DC = GetDC(Dialog);
	    if (ts->VTFlag>0)
	    {
	      GetRB(Dialog,&i,IDC_WINCOLOREMU,IDC_WINCOLOREMU);
	      if (i!=0)
		ts->ColorFlag |= CF_FULLCOLOR;
	      else
		ts->ColorFlag &= ~(WORD)CF_FULLCOLOR;
	      GetRB(Dialog,&ts->EnableScrollBuff,IDC_WINSCROLL1,IDC_WINSCROLL1);
	      if ( ts->EnableScrollBuff>0 )
	      {
		ts->ScrollBuffSize =
		  GetDlgItemInt(Dialog,IDC_WINSCROLL2,NULL,FALSE);
	      }
	      for (i = 0 ; i <= 1 ; i++)
	      {
		ts->VTColor[i] =
		  RGB(ts->TmpColor[0][i*3],
		      ts->TmpColor[0][i*3+1],
		      ts->TmpColor[0][i*3+2]);
		ts->VTBoldColor[i] =
		  RGB(ts->TmpColor[1][i*3],
		      ts->TmpColor[1][i*3+1],
		      ts->TmpColor[1][i*3+2]);
		ts->VTBlinkColor[i] =
		  RGB(ts->TmpColor[2][i*3],
		      ts->TmpColor[2][i*3+1],
		      ts->TmpColor[2][i*3+2]);
        /* begin - ishizaki */
		ts->URLColor[i] =
		  RGB(ts->TmpColor[3][i*3],
		      ts->TmpColor[3][i*3+1],
		      ts->TmpColor[3][i*3+2]);
        /* end - ishizaki */
		ts->VTColor[i] = GetNearestColor(DC,ts->VTColor[i]);
		ts->VTBoldColor[i] = GetNearestColor(DC,ts->VTBoldColor[i]);
		ts->VTBlinkColor[i] = GetNearestColor(DC,ts->VTBlinkColor[i]);
        /* begin - ishizaki */
		ts->URLColor[i] = GetNearestColor(DC,ts->URLColor[i]);
        /* end - ishizaki */
	      }
	    }
	    else {
	      for (i = 0 ; i <= 1 ; i++)
	      {
		ts->TEKColor[i] =
		  RGB(ts->TmpColor[0][i*3],
		      ts->TmpColor[0][i*3+1],
		      ts->TmpColor[0][i*3+2]);
		ts->TEKColor[i] = GetNearestColor(DC,ts->TEKColor[i]);
	      }
	      GetRB(Dialog,&ts->TEKColorEmu,IDC_WINCOLOREMU,IDC_WINCOLOREMU);
	    }
	    ReleaseDC(Dialog,DC);

	    GetRB(Dialog,&ts->CursorShape,IDC_WINBLOCK,IDC_WINHORZ);

#ifdef USE_NORMAL_BGCOLOR
        GetRB(Dialog,&ts->UseNormalBGColor,IDC_WINUSENORMALBG,IDC_WINUSENORMALBG);
		// 2006/03/11 by 337
		if (ts->UseNormalBGColor) {
			ts->VTBoldColor[1] = 
			ts->VTBlinkColor[1] = 
			ts->URLColor[1] = 
				ts->VTColor[1];
		}
#endif
	  }
	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgWinFont != NULL) {
        DeleteObject(DlgWinFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgWinFont != NULL) {
        DeleteObject(DlgWinFont);
      }
#endif
	  return TRUE;

	case IDC_WINHIDETITLE:
	  GetRB(Dialog,&i,IDC_WINHIDETITLE,IDC_WINHIDETITLE);
	  if (i>0)
	    DisableDlgItem(Dialog,IDC_WINHIDEMENU,IDC_WINHIDEMENU);
	  else
	    EnableDlgItem(Dialog,IDC_WINHIDEMENU,IDC_WINHIDEMENU);
	  break;

	case IDC_WINSCROLL1:
	  if ( ts==NULL ) return TRUE;
	  GetRB(Dialog,&i,IDC_WINSCROLL1,IDC_WINSCROLL1);
	  if ( i>0 )
	    EnableDlgItem(Dialog,IDC_WINSCROLL2,IDC_WINSCROLL3);
	  else
	    DisableDlgItem(Dialog,IDC_WINSCROLL2,IDC_WINSCROLL3);
	  break;

	case IDC_WINTEXT:
	  if ( ts==NULL ) return TRUE;
	  IOffset = 0;
	  ChangeSB(Dialog,ts,IAttr,IOffset);
	  break;
	  
	case IDC_WINBACK:
	  if ( ts==NULL ) return TRUE;
	  IOffset = 3;
	  ChangeSB(Dialog,ts,IAttr,IOffset);
	  break;

	case IDC_WINREV:
	  if ( ts==NULL ) return TRUE;
	  i = ts->TmpColor[IAttr][0];
	  ts->TmpColor[IAttr][0] = ts->TmpColor[IAttr][3];
	  ts->TmpColor[IAttr][3] = i;
	  i = ts->TmpColor[IAttr][1];
	  ts->TmpColor[IAttr][1] = ts->TmpColor[IAttr][4];
	  ts->TmpColor[IAttr][4] = i;
	  i = ts->TmpColor[IAttr][2];
	  ts->TmpColor[IAttr][2] = ts->TmpColor[IAttr][5];
	  ts->TmpColor[IAttr][5] = i;

	  ChangeSB(Dialog,ts,IAttr,IOffset);
	  break;

	case IDC_WINATTR:
	  if ( ts!=NULL ) ChangeSB(Dialog,ts,IAttr,IOffset);
	  break;

	case IDC_WINHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
	  break;
      }
      break;

    case WM_PAINT:
      ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
      if ( ts==NULL ) return TRUE;
      RestoreVar(Dialog,ts,&IAttr,&IOffset);
      DispSample(Dialog,ts,IAttr);
      break;

    case WM_HSCROLL:
      ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
      if (ts==NULL) return TRUE;
      RestoreVar(Dialog,ts,&IAttr,&IOffset);
      HRed = GetDlgItem(Dialog, IDC_WINREDBAR);
      HGreen = GetDlgItem(Dialog, IDC_WINGREENBAR);
      HBlue = GetDlgItem(Dialog, IDC_WINBLUEBAR);
#ifdef TERATERM32
      Wnd = (HWND)lParam;
      ScrollCode = LOWORD(wParam);
      NewPos = HIWORD(wParam);
#else
      Wnd = (HWND)HIWORD(lParam);
      ScrollCode = wParam;
      NewPos = LOWORD(lParam);
#endif
      if ( Wnd == HRed ) i = IOffset;
      else if ( Wnd == HGreen ) i = IOffset + 1;
      else if ( Wnd == HBlue ) i = IOffset + 2;
      else return TRUE;
      pos = ts->TmpColor[IAttr][i];
      switch (ScrollCode) {
	case SB_BOTTOM:
	  pos = 255;
	  break;
	case SB_LINEDOWN:
	  if (pos<255) pos++;
	  break;
	case SB_LINEUP:
	  if (pos>0) pos--;
	  break;
	case SB_PAGEDOWN:
	  pos = pos + 16;
	  break;
	case SB_PAGEUP:
	  pos = pos - 16;
	  break;
	case SB_THUMBPOSITION:
	  pos = NewPos;
	  break;
	case SB_THUMBTRACK:
	  pos = NewPos;
	  break;
	case SB_TOP:
	  pos = 0;
	  break;
	default:
	  return TRUE;
      }
      if (pos > 255) pos = 255;
      ts->TmpColor[IAttr][i] = pos;
      SetScrollPos(Wnd,SB_CTL,pos,TRUE);
      ChangeColor(Dialog,ts,IAttr,IOffset);
      return FALSE;
  }
  return FALSE;
}

BOOL CALLBACK KeybDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PTTSet ts;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      ts = (PTTSet)lParam;
      SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgKeybFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_KEYBTRANS, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_KEYBBS, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_KEYBDEL, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_KEYBKEYBTEXT, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_KEYBKEYB, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_KEYBMETA, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_KEYBHELP, WM_SETFONT, (WPARAM)DlgKeybFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgKeybFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_KEYB_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_KEYBTRANS, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_KEYB_TRANSMIT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_KEYBTRANS, uimsg);
      GetDlgItemText(Dialog, IDC_KEYBBS, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_KEYB_BS", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_KEYBBS, uimsg);
      GetDlgItemText(Dialog, IDC_KEYBDEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_KEYB_DEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_KEYBDEL, uimsg);
      GetDlgItemText(Dialog, IDC_KEYBKEYBTEXT, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_KEYB_KEYB", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_KEYBKEYBTEXT, uimsg);
      GetDlgItemText(Dialog, IDC_KEYBMETA, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_KEYB_META", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_KEYBMETA, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_KEYBHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_KEYBHELP, uimsg);
#endif

      SetRB(Dialog,ts->BSKey-1,IDC_KEYBBS,IDC_KEYBBS);
      SetRB(Dialog,ts->DelKey,IDC_KEYBDEL,IDC_KEYBDEL);
      SetRB(Dialog,ts->MetaKey,IDC_KEYBMETA,IDC_KEYBMETA);
      if (ts->Language==IdRussian)
      {
	ShowDlgItem(Dialog,IDC_KEYBKEYBTEXT,IDC_KEYBKEYB);
	SetDropDownList(Dialog, IDC_KEYBKEYB, RussList2, ts->RussKeyb);
      }
      return TRUE;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
	  if ( ts!=NULL )
	  {
	    GetRB(Dialog,&ts->BSKey,IDC_KEYBBS,IDC_KEYBBS);
	    ts->BSKey++;
	    GetRB(Dialog,&ts->DelKey,IDC_KEYBDEL,IDC_KEYBDEL);
	    GetRB(Dialog,&ts->MetaKey,IDC_KEYBMETA,IDC_KEYBMETA);
	    if (ts->Language==IdRussian)
	      ts->RussKeyb = (WORD)GetCurSel(Dialog, IDC_KEYBKEYB);
	  }
	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgKeybFont != NULL) {
        DeleteObject(DlgKeybFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgKeybFont != NULL) {
        DeleteObject(DlgKeybFont);
      }
#endif
	  return TRUE;

	case IDC_KEYBHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
      }
  }
  return FALSE;
}

// �{�[���[�g�̏�����g������ (2005.11.30 yutaka)
static PCHAR far BaudList[] =
  {"110","300","600","1200","2400","4800","9600",
   "14400","19200","38400","57600","115200",
   "230400", "460800", "921600", NULL};
static PCHAR far DataList[] = {"7 bit","8 bit",NULL};
static PCHAR far ParityList[] = {"even","odd","none",NULL};
static PCHAR far StopList[] = {"1 bit","2 bit",NULL};
static PCHAR far FlowList[] = {"Xon/Xoff","hardware","none",NULL};

BOOL CALLBACK SerialDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PTTSet ts;
  int i, w;
  char Temp[7];
  char ComPortTable[MAXCOMPORT];
  int comports;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      ts = (PTTSet)lParam;
      SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgSerialFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_SERIALPORT_LABEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALPORT, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALBAUD_LEBAL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALBAUD, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALDATA_LABEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALDATA, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALPARITY_LABEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALPARITY, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALSTOP_LABEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALSTOP, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALFLOW_LABEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALFLOW, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALDELAY, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALDELAYCHAR_LABEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALDELAYCHAR, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALDELAYLINE_LABEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALDELAYLINE, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SERIALHELP, WM_SETFONT, (WPARAM)DlgSerialFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgSerialFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALPORT_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_PORT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALPORT_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALBAUD_LEBAL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_BAUD", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALBAUD_LEBAL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALDATA_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_DATA", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALDATA_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALPARITY_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_PARITY", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALPARITY_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALSTOP_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_STOP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALSTOP_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALFLOW_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_FLOW", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALFLOW_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALDELAY, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_DELAY", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALDELAY, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALDELAYCHAR_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_DELAYCHAR", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALDELAYCHAR_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALDELAYLINE_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_SERIAL_DELAYLINE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALDELAYLINE_LABEL, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_SERIALHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SERIALHELP, uimsg);
#endif

      strcpy(Temp,"COM");
      w = 0;

      if ((comports = DetectComPorts(ComPortTable, ts->MaxComPort)) > 0) {
	  for (i=0; i<comports; i++) {
	      uint2str(ComPortTable[i], &Temp[3], 2);
	      SendDlgItemMessage(Dialog, IDC_SERIALPORT, CB_ADDSTRING,
				0, (LPARAM)Temp);
	      if (ComPortTable[i] == ts->ComPort) {
		  w = i;
	      }
	  }
      } else if (comports == 0) {
	  DisableDlgItem(Dialog, IDC_SERIALPORT, IDC_SERIALPORT);
	  DisableDlgItem(Dialog, IDC_SERIALPORT_LABEL, IDC_SERIALPORT_LABEL);
      } else {
	for (i=1; i<=ts->MaxComPort; i++) {
	    uint2str(i,&Temp[3],2);
	    SendDlgItemMessage(Dialog, IDC_SERIALPORT, CB_ADDSTRING,
				0, (LPARAM)Temp);
	}
	if (ts->ComPort<=ts->MaxComPort)
	    w = ts->ComPort-1;

      }
      SendDlgItemMessage(Dialog, IDC_SERIALPORT, CB_SETCURSEL, w, 0);

      SetDropDownList(Dialog, IDC_SERIALBAUD, BaudList, ts->Baud);
      SetDropDownList(Dialog, IDC_SERIALDATA, DataList, ts->DataBit);
      SetDropDownList(Dialog, IDC_SERIALPARITY, ParityList, ts->Parity);
      SetDropDownList(Dialog, IDC_SERIALSTOP, StopList, ts->StopBit);
      SetDropDownList(Dialog, IDC_SERIALFLOW, FlowList, ts->Flow);

      SetDlgItemInt(Dialog,IDC_SERIALDELAYCHAR,ts->DelayPerChar,FALSE);
      SendDlgItemMessage(Dialog, IDC_SERIALDELAYCHAR, EM_LIMITTEXT,4, 0);

      SetDlgItemInt(Dialog,IDC_SERIALDELAYLINE,ts->DelayPerLine,FALSE);
      SendDlgItemMessage(Dialog, IDC_SERIALDELAYLINE, EM_LIMITTEXT,4, 0);

      return TRUE;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
	  if ( ts!=NULL )
	  {
	    memset(Temp, 0, sizeof(Temp));
	    GetDlgItemText(Dialog, IDC_SERIALPORT, Temp, sizeof(Temp)-1);
	    if (strncmp(Temp, "COM", 3) == 0 && Temp[3] != '\0') {
	      ts->ComPort = (WORD)atoi(&Temp[3]);
	    } else {
	      ts->ComPort = 0;
	    }

	    ts->Baud = (WORD)GetCurSel(Dialog, IDC_SERIALBAUD);
	    ts->DataBit = (WORD)GetCurSel(Dialog, IDC_SERIALDATA);
	    ts->Parity = (WORD)GetCurSel(Dialog, IDC_SERIALPARITY);
	    ts->StopBit = (WORD)GetCurSel(Dialog, IDC_SERIALSTOP);
	    ts->Flow = (WORD)GetCurSel(Dialog, IDC_SERIALFLOW);

	    ts->DelayPerChar = GetDlgItemInt(Dialog,IDC_SERIALDELAYCHAR,NULL,FALSE);

	    ts->DelayPerLine = GetDlgItemInt(Dialog,IDC_SERIALDELAYLINE,NULL,FALSE);

	    ts->PortType = IdSerial;

	    // �{�[���[�g���ύX����邱�Ƃ�����̂ŁA�^�C�g���ĕ\����
	    // ���b�Z�[�W���΂��悤�ɂ����B (2007.7.21 maya)
	    PostMessage(GetParent(Dialog),WM_USER_CHANGETITLE,0,0);
	  }

	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgSerialFont != NULL) {
        DeleteObject(DlgSerialFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgSerialFont != NULL) {
        DeleteObject(DlgSerialFont);
      }
#endif
	  return TRUE;

	case IDC_SERIALHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
      }
  }
  return FALSE;
}

BOOL CALLBACK TCPIPDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PTTSet ts;
  char EntName[7];
  char TempHost[HostNameMaxLength+1];
  UINT i, Index;
  WORD w;
  BOOL Ok;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      ts = (PTTSet)lParam;
      SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgTcpipFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_TCPIPHOSTLIST, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPHOST, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPADD, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPLIST, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPUP, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPREMOVE, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPDOWN, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPHISTORY, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPAUTOCLOSE, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPPORTLABEL, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPPORT, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPTELNET, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPTERMTYPELABEL, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPTERMTYPE, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_TCPIPHELP, WM_SETFONT, (WPARAM)DlgTcpipFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgTcpipFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPHOSTLIST, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_HOSTLIST", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPHOSTLIST, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPADD, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_ADD", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPADD, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPUP, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_UP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPUP, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPREMOVE, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_REMOVE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPREMOVE, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPDOWN, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_DOWN", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPDOWN, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPHISTORY, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_HISTORY", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPHISTORY, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPAUTOCLOSE, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_AUTOCLOSE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPAUTOCLOSE, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPPORTLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_PORT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPPORTLABEL, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPTELNET, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_TELNET", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPTELNET, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPTERMTYPELABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_TCPIP_TERMTYPE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPTERMTYPELABEL, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_TCPIPHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_TCPIPHELP, uimsg);
#endif

      SendDlgItemMessage(Dialog, IDC_TCPIPHOST, EM_LIMITTEXT,
			 HostNameMaxLength-1, 0);

      strcpy(EntName,"Host");

      i = 1;
      do {
	uint2str(i,&EntName[4],2);
	GetPrivateProfileString("Hosts",EntName,"",
				TempHost,sizeof(TempHost),ts->SetupFName);
	if (strlen(TempHost) > 0)
	  SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_ADDSTRING,
			     0, (LPARAM)TempHost);
	i++;
      } while ((i <= 99) && (strlen(TempHost)>0));

      /* append a blank item to the bottom */
      TempHost[0] = 0;
      SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_ADDSTRING,
		         0, (LPARAM)TempHost);

      SetRB(Dialog,ts->HistoryList,IDC_TCPIPHISTORY,IDC_TCPIPHISTORY);

      SetRB(Dialog,ts->AutoWinClose,IDC_TCPIPAUTOCLOSE,IDC_TCPIPAUTOCLOSE);

      SetDlgItemInt(Dialog,IDC_TCPIPPORT,ts->TCPPort,FALSE);

      SetRB(Dialog,ts->Telnet,IDC_TCPIPTELNET,IDC_TCPIPTELNET);

      SetDlgItemText(Dialog, IDC_TCPIPTERMTYPE, ts->TermType);

      SendDlgItemMessage(Dialog, IDC_TCPIPTERMTYPE, EM_LIMITTEXT,
			 sizeof(ts->TermType)-1, 0);

      // SSH�ڑ��̂Ƃ��ɂ� TERM �𑗂�̂ŁAtelnet�������ł� disabled �ɂ��Ȃ��B(2005.11.3 yutaka)
      EnableDlgItem(Dialog,IDC_TCPIPTERMTYPELABEL,IDC_TCPIPTERMTYPE);

      return TRUE;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
	  if (ts!=NULL)
	  {
	    WritePrivateProfileString("Hosts",NULL,NULL,ts->SetupFName);

	    Index = SendDlgItemMessage(Dialog,IDC_TCPIPLIST,LB_GETCOUNT,0,0);
	    if (Index==(UINT)LB_ERR)
	      Index = 0;
	    else
	      Index--;
	    if (Index>99) Index = 99;

	    strcpy(EntName,"Host");
	    for (i = 1 ; i <= Index ; i++)
	    {
	      SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_GETTEXT,
				 i-1, (LPARAM)TempHost);
	      uint2str(i,&EntName[4],2);
	      WritePrivateProfileString("Hosts",EntName,TempHost,ts->SetupFName);
	    }

	    GetRB(Dialog,&ts->HistoryList,IDC_TCPIPHISTORY,IDC_TCPIPHISTORY);

	    GetRB(Dialog,&ts->AutoWinClose,IDC_TCPIPAUTOCLOSE,IDC_TCPIPAUTOCLOSE);

	    ts->TCPPort = GetDlgItemInt(Dialog,IDC_TCPIPPORT,&Ok,FALSE);
	    if (! Ok) ts->TCPPort = ts->TelPort;

	    GetRB(Dialog,&ts->Telnet,IDC_TCPIPTELNET,IDC_TCPIPTELNET);

	    GetDlgItemText(Dialog, IDC_TCPIPTERMTYPE, ts->TermType,
			   sizeof(ts->TermType));
	  }
	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgTcpipFont != NULL) {
        DeleteObject(DlgTcpipFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgTcpipFont != NULL) {
        DeleteObject(DlgTcpipFont);
      }
#endif
	  return TRUE;

	case IDC_TCPIPHOST:
#ifdef TERATERM32
	  if (HIWORD(wParam)==EN_CHANGE)
#else
	  if (HIWORD(lParam)==EN_CHANGE)
#endif
	  {
	    GetDlgItemText(Dialog, IDC_TCPIPHOST, TempHost, sizeof(TempHost));
	    if (strlen(TempHost)==0)
	      DisableDlgItem(Dialog,IDC_TCPIPADD,IDC_TCPIPADD);
	    else
	      EnableDlgItem(Dialog,IDC_TCPIPADD,IDC_TCPIPADD);
	  }
	  break;

	case IDC_TCPIPADD:
	  GetDlgItemText(Dialog, IDC_TCPIPHOST, TempHost, sizeof(TempHost));
	  if (strlen(TempHost)>0)
	  {
	    Index = SendDlgItemMessage(Dialog,IDC_TCPIPLIST,LB_GETCURSEL,0,0);
	    if (Index==(UINT)LB_ERR) Index = 0;

	    SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_INSERTSTRING,
			       Index, (LPARAM)TempHost);

	    SetDlgItemText(Dialog, IDC_TCPIPHOST, 0);
	    SetFocus(GetDlgItem(Dialog, IDC_TCPIPHOST));
	  }
	  break;

	case IDC_TCPIPLIST:
#ifdef TERATERM32
	  if (HIWORD(wParam)==LBN_SELCHANGE)
#else
	  if (HIWORD(lParam)==LBN_SELCHANGE)
#endif
	  {
	    i = SendDlgItemMessage(Dialog,IDC_TCPIPLIST,LB_GETCOUNT,0,0);
	    Index = SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_GETCURSEL, 0, 0);
	    if ((i<=1) || (Index==(UINT)LB_ERR) ||
		(Index==i-1))
	      DisableDlgItem(Dialog,IDC_TCPIPUP,IDC_TCPIPDOWN);
	    else {
	      EnableDlgItem(Dialog,IDC_TCPIPREMOVE,IDC_TCPIPREMOVE);
	      if (Index==0)
		DisableDlgItem(Dialog,IDC_TCPIPUP,IDC_TCPIPUP);
	      else
		EnableDlgItem(Dialog,IDC_TCPIPUP,IDC_TCPIPUP);
	      if (Index>=i-2)
		DisableDlgItem(Dialog,IDC_TCPIPDOWN,IDC_TCPIPDOWN);
	      else
		EnableDlgItem(Dialog,IDC_TCPIPDOWN,IDC_TCPIPDOWN);
	    }
	  }
	  break;

	case IDC_TCPIPUP:
	case IDC_TCPIPDOWN:
	  i = SendDlgItemMessage(Dialog,IDC_TCPIPLIST,LB_GETCOUNT,0,0);
	  Index = SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_GETCURSEL, 0, 0);
	  if (Index==(UINT)LB_ERR) return TRUE;
	  if (LOWORD(wParam)==IDC_TCPIPDOWN) Index++;
	  if ((Index==0) || (Index>=i-1)) return TRUE;
	  SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_GETTEXT,
			     Index, (LPARAM)TempHost);
	  SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_DELETESTRING,
			     Index, 0);
	  SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_INSERTSTRING,
			     Index-1, (LPARAM)TempHost);
	  if (LOWORD(wParam)==IDC_TCPIPUP) Index--;
	  SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_SETCURSEL,Index,0);
	  if (Index==0)
	    DisableDlgItem(Dialog,IDC_TCPIPUP,IDC_TCPIPUP);
	  else
	    EnableDlgItem(Dialog,IDC_TCPIPUP,IDC_TCPIPUP);
	  if (Index>=i-2)
	    DisableDlgItem(Dialog,IDC_TCPIPDOWN,IDC_TCPIPDOWN);
	  else
	    EnableDlgItem(Dialog,IDC_TCPIPDOWN,IDC_TCPIPDOWN);
	  SetFocus(GetDlgItem(Dialog, IDC_TCPIPLIST));
	  break;

	case IDC_TCPIPREMOVE:
	  i = SendDlgItemMessage(Dialog,IDC_TCPIPLIST,LB_GETCOUNT,0,0);
	  Index = SendDlgItemMessage(Dialog,IDC_TCPIPLIST,LB_GETCURSEL, 0, 0);
	  if ((Index==(UINT)LB_ERR) ||
	      (Index==i-1)) return TRUE;
	  SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_GETTEXT,
			     Index, (LPARAM)TempHost);
	  SendDlgItemMessage(Dialog, IDC_TCPIPLIST, LB_DELETESTRING,
			     Index, 0);
	  SetDlgItemText(Dialog, IDC_TCPIPHOST, TempHost);
	  DisableDlgItem(Dialog,IDC_TCPIPUP,IDC_TCPIPDOWN);
	  SetFocus(GetDlgItem(Dialog, IDC_TCPIPHOST));
	  break;

	case IDC_TCPIPTELNET:
	  GetRB(Dialog,&w,IDC_TCPIPTELNET,IDC_TCPIPTELNET);
	  if (w==1)
	  {
	    EnableDlgItem(Dialog,IDC_TCPIPTERMTYPELABEL,IDC_TCPIPTERMTYPE);
	    ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
	    if (ts!=NULL)
	      SetDlgItemInt(Dialog,IDC_TCPIPPORT,ts->TelPort,FALSE);
	  }
	  else 
	  {
	    // SSH�ڑ��̂Ƃ��ɂ� TERM �𑗂�̂ŁAtelnet�������ł� disabled �ɂ��Ȃ��B(2005.11.3 yutaka)
	    EnableDlgItem(Dialog,IDC_TCPIPTERMTYPELABEL,IDC_TCPIPTERMTYPE);
	  }
	  break;

	case IDC_TCPIPHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
	  break;
      }
  }
  return FALSE;
}

BOOL CALLBACK HostDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PGetHNRec GetHNRec;
  char EntName[7];
  char TempHost[HostNameMaxLength+1];
  WORD i, j, w;
  BOOL Ok;
  char ComPortTable[MAXCOMPORT];
  int comports;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      GetHNRec = (PGetHNRec)lParam;
      SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgHostFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_HOSTTCPIP, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTNAMELABEL, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTNAME, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTTELNET, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTTCPPORTLABEL, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTTCPPORT, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTTCPPROTOCOLLABEL, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTTCPPROTOCOL, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTSERIAL, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTCOMLABEL, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTCOM, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_HOSTHELP, WM_SETFONT, (WPARAM)DlgHostFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgHostFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_HOST_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_HOSTNAMELABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_HOST_TCPIP_HOST", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_HOSTNAMELABEL, uimsg);
      GetDlgItemText(Dialog, IDC_HOSTTCPPORTLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_HOST_TCPIP_PORT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_HOSTTCPPORTLABEL, uimsg);
      GetDlgItemText(Dialog, IDC_HOSTTCPPROTOCOLLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_HOST_TCPIP_PROTOCOL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_HOSTTCPPROTOCOLLABEL, uimsg);
      GetDlgItemText(Dialog, IDC_HOSTSERIAL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_HOST_SERIAL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_HOSTSERIAL, uimsg);
      GetDlgItemText(Dialog, IDC_HOSTCOMLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_HOST_SERIAL_PORT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_HOSTCOMLABEL, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_HOSTHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_HOSTHELP, uimsg);
#endif

      if ( GetHNRec->PortType==IdFile )
	GetHNRec->PortType = IdTCPIP;

      strcpy(EntName,"Host");

      i = 1;
      do {
	uint2str(i,&EntName[4],2);
	GetPrivateProfileString("Hosts",EntName,"",
				TempHost,sizeof(TempHost),GetHNRec->SetupFN);
	if ( strlen(TempHost) > 0 )
	  SendDlgItemMessage(Dialog, IDC_HOSTNAME, CB_ADDSTRING,
			     0, (LPARAM)TempHost);
	i++;
      } while ((i <= 99) && (strlen(TempHost)>0));

      SendDlgItemMessage(Dialog, IDC_HOSTNAME, EM_LIMITTEXT,
			 HostNameMaxLength-1, 0);

      SendDlgItemMessage(Dialog, IDC_HOSTNAME, CB_SETCURSEL,0,0);

      SetRB(Dialog,GetHNRec->Telnet,IDC_HOSTTELNET,IDC_HOSTTELNET);
      SendDlgItemMessage(Dialog, IDC_HOSTTCPPORT, EM_LIMITTEXT,5,0);
      SetDlgItemInt(Dialog,IDC_HOSTTCPPORT,GetHNRec->TCPPort,FALSE);
#ifndef NO_INET6
      for (i=0; ProtocolFamilyList[i]; ++i)
      {
        SendDlgItemMessage(Dialog, IDC_HOSTTCPPROTOCOL, CB_ADDSTRING,
			   0, (LPARAM)ProtocolFamilyList[i]);
      }
      SendDlgItemMessage(Dialog, IDC_HOSTTCPPROTOCOL, EM_LIMITTEXT,
                         ProtocolFamilyMaxLength-1, 0);
      SendDlgItemMessage(Dialog, IDC_HOSTTCPPROTOCOL, CB_SETCURSEL,0,0);
#endif /* NO_INET6 */

      j = 0;
      w = 1;
      strcpy(EntName,"COM");
      if ((comports=DetectComPorts(ComPortTable, GetHNRec->MaxComPort)) >= 0) {
	for (i=0; i<comports; i++) {
	  if (((GetCOMFlag() >> (ComPortTable[i]-1)) & 1)==0) {
	    uint2str(ComPortTable[i], &EntName[3], 2);
	    SendDlgItemMessage(Dialog, IDC_HOSTCOM, CB_ADDSTRING,
			       0, (LPARAM)EntName);
	    j++;
	    if (GetHNRec->ComPort==ComPortTable[i]) w = j;
	  }
	}
      } else {
	for (i=1; i<=GetHNRec->MaxComPort ;i++)
	{
	  if (((GetCOMFlag() >> (i-1)) & 1)==0)
	  {
	    uint2str(i,&EntName[3],2);
	    SendDlgItemMessage(Dialog, IDC_HOSTCOM, CB_ADDSTRING,
			       0, (LPARAM)EntName);
	    j++;
	    if (GetHNRec->ComPort==i) w = j;
	  }
	}
      }
      if (j>0)
	SendDlgItemMessage(Dialog, IDC_HOSTCOM, CB_SETCURSEL,w-1,0);
      else { /* All com ports are already used */
	GetHNRec->PortType = IdTCPIP;
	DisableDlgItem(Dialog,IDC_HOSTSERIAL,IDC_HOSTSERIAL);
      }

      SetRB(Dialog,GetHNRec->PortType,IDC_HOSTTCPIP,IDC_HOSTSERIAL);

      if ( GetHNRec->PortType==IdTCPIP )
	DisableDlgItem(Dialog,IDC_HOSTCOMLABEL,IDC_HOSTCOM);
#ifndef INET6
      else
	DisableDlgItem(Dialog,IDC_HOSTNAMELABEL,IDC_HOSTTCPPORT);
#else
      else {
        DisableDlgItem(Dialog,IDC_HOSTNAMELABEL,IDC_HOSTTCPPORT);
        DisableDlgItem(Dialog,IDC_HOSTTCPPROTOCOLLABEL,IDC_HOSTTCPPROTOCOL);
      }
#endif /* NO_INET6 */
 
      return TRUE;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  GetHNRec = (PGetHNRec)GetWindowLong(Dialog,DWL_USER);
	  if ( GetHNRec!=NULL )
	  {
#ifndef NO_INET6
            char afstr[BUFSIZ];
#endif /* NO_INET6 */
	    GetRB(Dialog,&GetHNRec->PortType,IDC_HOSTTCPIP,IDC_HOSTSERIAL);
	    if ( GetHNRec->PortType==IdTCPIP )
	      GetDlgItemText(Dialog, IDC_HOSTNAME, GetHNRec->HostName, HostNameMaxLength);
	    else
	      GetHNRec->HostName[0] = 0;
	    GetRB(Dialog,&GetHNRec->Telnet,IDC_HOSTTELNET,IDC_HOSTTELNET);
	    i = GetDlgItemInt(Dialog,IDC_HOSTTCPPORT,&Ok,FALSE);
	    if (Ok) GetHNRec->TCPPort = i;
#ifndef NO_INET6
#define getaf(str) \
((strcmp((str), "IPv6") == 0) ? AF_INET6 : \
 ((strcmp((str), "IPv4") == 0) ? AF_INET : AF_UNSPEC))
            memset(afstr, 0, sizeof(afstr));
            GetDlgItemText(Dialog, IDC_HOSTTCPPROTOCOL, afstr, sizeof(afstr));
            GetHNRec->ProtocolFamily = getaf(afstr);
#endif /* NO_INET6 */
	    memset(EntName,0,sizeof(EntName));
	    GetDlgItemText(Dialog, IDC_HOSTCOM, EntName, sizeof(EntName)-1);
	    if (strncmp(EntName, "COM", 3) == 0 && EntName[3] != '\0') {
	      GetHNRec->ComPort = (BYTE)(EntName[3])-0x30;
	      if (strlen(EntName)>4)
		GetHNRec->ComPort = GetHNRec->ComPort*10 + (BYTE)(EntName[4])-0x30;
	      if (GetHNRec->ComPort > GetHNRec->MaxComPort)
		GetHNRec->ComPort = 1;
	    }
	    else {
	      GetHNRec->ComPort = 1;
	    }
	  }
	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgHostFont != NULL) {
        DeleteObject(DlgHostFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgHostFont != NULL) {
        DeleteObject(DlgHostFont);
      }
#endif
	  return TRUE;

	case IDC_HOSTTCPIP:
	  EnableDlgItem(Dialog,IDC_HOSTNAMELABEL,IDC_HOSTTCPPORT);
#ifndef NO_INET6
          EnableDlgItem(Dialog,IDC_HOSTTCPPROTOCOLLABEL,IDC_HOSTTCPPROTOCOL);
#endif /* NO_INET6 */
	  DisableDlgItem(Dialog,IDC_HOSTCOMLABEL,IDC_HOSTCOM);
	  return TRUE;

	case IDC_HOSTSERIAL:
	  EnableDlgItem(Dialog,IDC_HOSTCOMLABEL,IDC_HOSTCOM);
	  DisableDlgItem(Dialog,IDC_HOSTNAMELABEL,IDC_HOSTTCPPORT);
#ifndef NO_INET6
          DisableDlgItem(Dialog,IDC_HOSTTCPPROTOCOLLABEL,IDC_HOSTTCPPROTOCOL);
#endif /* NO_INET6 */
	  break;

	case IDC_HOSTTELNET:
	  GetRB(Dialog,&i,IDC_HOSTTELNET,IDC_HOSTTELNET);
	  if ( i==1 )
	  {
	    GetHNRec = (PGetHNRec)GetWindowLong(Dialog,DWL_USER);
	    if ( GetHNRec!=NULL )
	      SetDlgItemInt(Dialog,IDC_HOSTTCPPORT,GetHNRec->TelPort,FALSE);
	  }
	  break;

	case IDC_HOSTHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
      }
  }
  return FALSE;
}

BOOL CALLBACK DirDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PCHAR CurDir;
  char HomeDir[MAXPATHLEN];
  char TmpDir[MAXPATHLEN];
  RECT R;
  HDC TmpDC;
  SIZE s;
  HWND HDir, HOk, HCancel, HHelp;
  POINT D, B;
  int WX, WY, WW, WH, CW, DW, DH, BW, BH;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  char uimsg2[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgDirFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_DIRCURRENT_LABEL, WM_SETFONT, (WPARAM)DlgDirFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_DIRCURRENT, WM_SETFONT, (WPARAM)DlgDirFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_DIRNEW_LABEL, WM_SETFONT, (WPARAM)DlgDirFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_DIRNEW, WM_SETFONT, (WPARAM)DlgDirFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgDirFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgDirFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_DIRHELP, WM_SETFONT, (WPARAM)DlgDirFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgDirFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_DIR_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_DIRCURRENT_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_DIR_CURRENT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_DIRCURRENT_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_DIRNEW_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_DIR_NEW", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_DIRNEW_LABEL, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_DIRHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_DIRHELP, uimsg);
#endif

      CurDir = (PCHAR)(lParam);
      SetWindowLong(Dialog, DWL_USER, lParam);

      SetDlgItemText(Dialog, IDC_DIRCURRENT, CurDir);
      SendDlgItemMessage(Dialog, IDC_DIRNEW, EM_LIMITTEXT,
			 MAXPATHLEN-1, 0);

// adjust dialog size
      // get size of current dir text
      HDir = GetDlgItem(Dialog, IDC_DIRCURRENT);
      GetWindowRect(HDir,&R);
      D.x = R.left;
      D.y = R.top;
      ScreenToClient(Dialog,&D);
      DH = R.bottom-R.top;  
      TmpDC = GetDC(Dialog);
#ifdef TERATERM32
      GetTextExtentPoint32(TmpDC,CurDir,strlen(CurDir),&s);
#else
      GetTextExtentPoint(TmpDC,CurDir,strlen(CurDir),&s);
#endif
      ReleaseDC(Dialog,TmpDC);
      DW = s.cx + s.cx/10;

      // get button size
      HOk = GetDlgItem(Dialog, IDOK);
      HCancel = GetDlgItem(Dialog, IDCANCEL);
      HHelp = GetDlgItem(Dialog, IDC_DIRHELP);
      GetWindowRect(HHelp,&R);
      B.x = R.left;
      B.y = R.top;
      ScreenToClient(Dialog,&B);
      BW = R.right-R.left;
      BH = R.bottom-R.top;

      // calc new dialog size
      GetWindowRect(Dialog,&R);
      WX = R.left;
      WY = R.top;
      WW = R.right-R.left;
      WH = R.bottom-R.top;
      GetClientRect(Dialog,&R);
      CW = R.right-R.left;
      if (D.x+DW < CW) DW = CW-D.x;
      WW = WW + D.x+DW - CW;

      // resize current dir text
      MoveWindow(HDir,D.x,D.y,DW,DH,TRUE);
      // move buttons
      MoveWindow(HOk,(D.x+DW-4*BW)/2,B.y,BW,BH,TRUE);
      MoveWindow(HCancel,(D.x+DW-BW)/2,B.y,BW,BH,TRUE);
      MoveWindow(HHelp,(D.x+DW+2*BW)/2,B.y,BW,BH,TRUE);
      // resize edit box
      HDir = GetDlgItem(Dialog, IDC_DIRNEW);
      GetWindowRect(HDir,&R);
      D.x = R.left;
      D.y = R.top;
      ScreenToClient(Dialog,&D);
      DW = R.right-R.left;
      if (DW<s.cx) DW = s.cx;
	MoveWindow(HDir,D.x,D.y,DW,R.bottom-R.top,TRUE);
      // resize dialog
      MoveWindow(Dialog,WX,WY,WW,WH,TRUE);

      return TRUE;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  CurDir = (PCHAR)GetWindowLong(Dialog,DWL_USER);
	  if ( CurDir!=NULL )
	  {
	    _getcwd(HomeDir,sizeof(HomeDir));
	    _chdir(CurDir);
	    GetDlgItemText(Dialog, IDC_DIRNEW, TmpDir,
			   sizeof(TmpDir));
	    if ( strlen(TmpDir)>0 )
	    {
	      if (_chdir(TmpDir) != 0)
	      {
#ifndef NO_I18N
		strcpy(uimsg2, "Tera Term: Error");
		get_lang_msg("MSG_TT_ERROR", uimsg2, UILanguageFile);
		strcpy(uimsg, "Cannot find directory");
		get_lang_msg("MSG_FIND_DIR_ERROR", uimsg, UILanguageFile);
		MessageBox(Dialog,uimsg,uimsg2,MB_ICONEXCLAMATION);
#else
		MessageBox(Dialog,"Cannot find directory",
		  "Tera Term: Error",MB_ICONEXCLAMATION);
#endif
		_chdir(HomeDir);
		return TRUE;
	      }
	      _getcwd(CurDir,MAXPATHLEN);
	    }
	    _chdir(HomeDir);
	  }
	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgDirFont != NULL) {
        DeleteObject(DlgDirFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgDirFont != NULL) {
        DeleteObject(DlgDirFont);
      }
#endif
	  return TRUE;

	case IDC_DIRHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
      }
  }
  return FALSE;
}

// ���s�t�@�C������o�[�W�������𓾂� (2005.2.28 yutaka)
static void get_file_version(char *exefile, int *major, int *minor, int *release, int *build)
{
	typedef struct {
		WORD wLanguage;
		WORD wCodePage;
	} LANGANDCODEPAGE, *LPLANGANDCODEPAGE;
	LPLANGANDCODEPAGE lplgcode;
	UINT unLen;
	DWORD size;
	char *buf = NULL;
	BOOL ret;
	int i;
	char fmt[80];
	char *pbuf;

	size = GetFileVersionInfoSize(exefile, NULL);
	if (size == 0) {
		goto error;
	}
	buf = malloc(size);
	ZeroMemory(buf, size);

	if (GetFileVersionInfo(exefile, 0, size, buf) == FALSE) {
		goto error;
	}

	ret = VerQueryValue(buf,
			"\\VarFileInfo\\Translation", 
			(LPVOID *)&lplgcode, &unLen);
	if (ret == FALSE)
		goto error;

	for (i = 0 ; i < (int)(unLen / sizeof(LANGANDCODEPAGE)) ; i++) {
		_snprintf(fmt, sizeof(fmt), "\\StringFileInfo\\%04x%04x\\FileVersion", 
			lplgcode[i].wLanguage, lplgcode[i].wCodePage);
		VerQueryValue(buf, fmt, &pbuf, &unLen);
		if (unLen > 0) { // get success
			int n, a, b, c, d;

			n = sscanf(pbuf, "%d, %d, %d, %d", &a, &b, &c, &d);
			if (n == 4) { // convert success
				*major = a;
				*minor = b;
				*release = c;
				*build = d;
				break;
			}
		}
	}

	free(buf);
	return;

error:
	free(buf);
	*major = *minor = *release = *build = 0;
}


//
// static text�ɏ����ꂽURL���_�u���N���b�N����ƁA�u���E�U���N������悤�ɂ���B
// based on sakura editor 1.5.2.1 # CDlgAbout.cpp
// (2005.4.7 yutaka)
//

typedef struct {
	WNDPROC proc;
	BOOL mouseover;
	HFONT font;
	HWND hWnd;
	int timer_done;
} url_subclass_t;

static url_subclass_t author_url_class, forum_url_class;

// static text�Ɋ��蓖�Ă�v���V�[�W��
static LRESULT CALLBACK UrlWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	url_subclass_t *parent = (url_subclass_t *)GetWindowLongPtr( hWnd, GWLP_USERDATA );
	HDC hdc;
	POINT pt;
	RECT rc;

	switch (msg) {
#if 0
	case WM_SETCURSOR:
		{
		// �J�[�\���`��ύX
		HCURSOR hc;

		hc = (HCURSOR)LoadImage(NULL,
				MAKEINTRESOURCE(IDC_HAND),
				IMAGE_CURSOR,
				0,
				0,
				LR_DEFAULTSIZE | LR_SHARED);
		if (hc != NULL) {
			SetClassLongPtr(hWnd, GCLP_HCURSOR, (LONG_PTR)hc);
		}
		return (LRESULT)0;
		}
#endif

	case WM_LBUTTONDBLCLK:
		{
		char url[128];

		// get URL
		SendMessage(hWnd, WM_GETTEXT , sizeof(url), (LPARAM)url);
		// kick WWW browser
	    ShellExecute(NULL, NULL, url, NULL, NULL,SW_SHOWNORMAL);
		}
		break;

	case WM_MOUSEMOVE:
		{
		BOOL bHilighted;
		pt.x = LOWORD( lParam );
		pt.y = HIWORD( lParam );
		GetClientRect( hWnd, &rc );
		bHilighted = PtInRect( &rc, pt );

		if (parent->mouseover != bHilighted) {
			parent->mouseover = bHilighted;
			InvalidateRect( hWnd, NULL, TRUE );
			if (parent->timer_done == 0) {
				parent->timer_done = 1;
				SetTimer( hWnd, 1, 200, NULL );
			}
		}

		}
		break;

	case WM_TIMER:
		// URL�̏�Ƀ}�E�X�J�[�\��������Ȃ�A�V�X�e���J�[�\����ύX����B
		if (author_url_class.mouseover || forum_url_class.mouseover) {
			HCURSOR hc;
			//SetCapture(hWnd);

			hc = (HCURSOR)LoadImage(NULL,
					MAKEINTRESOURCE(IDC_HAND),
					IMAGE_CURSOR,
					0,
					0,
					LR_DEFAULTSIZE | LR_SHARED);

			SetSystemCursor(CopyCursor(hc), 32512 /* OCR_NORMAL */);    // ���
			SetSystemCursor(CopyCursor(hc), 32513 /* OCR_IBEAM */);     // I�r�[��

		} else {
			//ReleaseCapture();
			// �}�E�X�J�[�\�������ɖ߂��B
			SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);

		}

		// �J�[�\�����E�B���h�E�O�ɂ���ꍇ�ɂ� WM_MOUSEMOVE �𑗂�
		GetCursorPos( &pt );
		ScreenToClient( hWnd, &pt );
		GetClientRect( hWnd, &rc );
		if( !PtInRect( &rc, pt ) )
			SendMessage( hWnd, WM_MOUSEMOVE, 0, MAKELONG( pt.x, pt.y ) );
		break;

	case WM_PAINT: 
		{
		// �E�B���h�E�̕`��
		PAINTSTRUCT ps;
		HFONT hFont;
		HFONT hOldFont;
		TCHAR szText[512];

		hdc = BeginPaint( hWnd, &ps );

		// ���݂̃N���C�A���g��`�A�e�L�X�g�A�t�H���g���擾����
		GetClientRect( hWnd, &rc );
		GetWindowText( hWnd, szText, 512 );
		hFont = (HFONT)SendMessage( hWnd, WM_GETFONT, (WPARAM)0, (LPARAM)0 );

		// �e�L�X�g�`��
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, parent->mouseover ? RGB( 0x84, 0, 0 ): RGB( 0, 0, 0xff ) );
		hOldFont = (HFONT)SelectObject( hdc, (HGDIOBJ)hFont );
		TextOut( hdc, 2, 0, szText, lstrlen( szText ) );
		SelectObject( hdc, (HGDIOBJ)hOldFont );

		// �t�H�[�J�X�g�`��
		if( GetFocus() == hWnd )
			DrawFocusRect( hdc, &rc );

		EndPaint( hWnd, &ps );
		return 0;
		}

	case WM_ERASEBKGND:
		hdc = (HDC)wParam;
		GetClientRect( hWnd, &rc );

		// �w�i�`��
		if( parent->mouseover ){
			// �n�C���C�g���w�i�`��
			SetBkColor( hdc, RGB( 0xff, 0xff, 0 ) );
			ExtTextOut( hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
		}else{
			// �e��WM_CTLCOLORSTATIC�𑗂��Ĕw�i�u���V���擾���A�w�i�`�悷��
			HBRUSH hbr;
			HBRUSH hbrOld;
			hbr = (HBRUSH)SendMessage( GetParent( hWnd ), WM_CTLCOLORSTATIC, wParam, (LPARAM)hWnd );
			hbrOld = (HBRUSH)SelectObject( hdc, hbr );
			FillRect( hdc, &rc, hbr );
			SelectObject( hdc, hbrOld );
		}
		return (LRESULT)1;

	case WM_DESTROY:
		// ��n��
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG_PTR)parent->proc );
		if( parent->font != NULL )
			DeleteObject( parent->font );

		// �}�E�X�J�[�\�������ɖ߂��B
		SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
		return (LRESULT)0;
	}

	return CallWindowProc( parent->proc, hWnd, msg, wParam, lParam );
}

// static text�Ƀv���V�[�W����ݒ肵�A�T�u�N���X������B
static void do_subclass_window(HWND hWnd, url_subclass_t *parent)
{
	HFONT hFont;
	LOGFONT lf;
	LONG_PTR lptr;

	//SetCapture(hWnd);

	if (!IsWindow(hWnd))
		return;

	// �e�̃v���V�[�W�����T�u�N���X����Q�Ƃł���悤�ɁA�|�C���^��o�^���Ă����B
	lptr = SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)parent );
	// �T�u�N���X�̃v���V�[�W����o�^����B
	parent->proc = (WNDPROC)SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG_PTR)UrlWndProc);

	// ������t����
	hFont = (HFONT)SendMessage( hWnd, WM_GETFONT, (WPARAM)0, (LPARAM)0 );
	GetObject( hFont, sizeof(lf), &lf );
	lf.lfUnderline = TRUE;
	parent->font = hFont = CreateFontIndirect( &lf ); // �s�v�ɂȂ�����폜���邱��
	if (hFont != NULL)
		SendMessage( hWnd, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE );

	parent->hWnd = hWnd;
	parent->timer_done = 0;
}


#ifdef WATCOM
  #pragma off (unreferenced);
#endif
BOOL CALLBACK AboutDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
#ifdef WATCOM
  #pragma on (unreferenced);
#endif
{
	int a, b, c, d;
	char buf[30];
	HDC hdc;
	HWND hwnd;
	RECT r;
	DWORD dwExt;
	WORD w, h;
	POINT point;
#ifndef NO_I18N
	char uimsg[MAX_UIMSG];
	LOGFONT logfont;
	HFONT font;
#endif

	switch (Message) {
	case WM_INITDIALOG:

#ifndef NO_I18N
		font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
		GetObject(font, sizeof(LOGFONT), &logfont);
		if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgAboutFont, UILanguageFile)) {
			SendDlgItemMessage(Dialog, IDC_TT_PRO, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TT_VERSION, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_INLUCDE_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_ONIGURUMA_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TT23_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TT23DATE_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_TERANISHI_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_PROJECT_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_RIGHTS_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_AUTHOR_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_AUTHOR_URL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_FORUM_LABEL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDC_FORUM_URL, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
			SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgAboutFont, MAKELPARAM(TRUE,0));
		}
		else {
			DlgAboutFont = NULL;
		}

		GetWindowText(Dialog, uimsg, sizeof(uimsg));
		get_lang_msg("DLG_ABOUT_TITLE", uimsg, UILanguageFile);
		SetWindowText(Dialog, uimsg);
#endif

		// TeraTerm�̃o�[�W������ݒ肷�� (2005.2.28 yutaka)
		// __argv[0]�ł� WinExec() �����v���Z�X����Q�Ƃł��Ȃ��悤�Ȃ̂ō폜�B(2005.3.14 yutaka)
		get_file_version("ttermpro.exe", &a, &b, &c, &d);
		_snprintf(buf, sizeof(buf), "Version %d.%d", a, b);
		SendMessage(GetDlgItem(Dialog, IDC_TT_VERSION), WM_SETTEXT, 0, (LPARAM)buf);

		// Oniguruma�̃o�[�W������ݒ肷�� 
		// �o�[�W�����̎擾�� onig_version() ���Ăяo���̂��K�؂����A���̂��߂����Ƀ��C�u������
		// �����N�������Ȃ������̂ŁA�ȉ��̂悤�ɂ����BOniguruma�̃o�[�W�������オ�����ꍇ�A
		// �r���h�G���[�ƂȂ邩������Ȃ��B
		// (2005.10.8 yutaka)
		// ���C�u�����������N���A���K�̎菇�Ńo�[�W�����𓾂邱�Ƃɂ����B
		// (2006.7.24 yutaka)
		_snprintf(buf, sizeof(buf), "Oniguruma %s", onig_version());
		SendMessage(GetDlgItem(Dialog, IDC_ONIGURUMA_LABEL), WM_SETTEXT, 0, (LPARAM)buf);

		// static text �̃T�C�Y��ύX (2007.4.16 maya)
		hwnd = GetDlgItem(Dialog, IDC_AUTHOR_URL);
		hdc = GetDC(hwnd);
		SelectObject(hdc, DlgAboutFont);
		GetDlgItemText(Dialog, IDC_AUTHOR_URL, uimsg, sizeof(uimsg));
		dwExt = GetTabbedTextExtent(hdc,uimsg,strlen(uimsg),0,NULL);
		w = LOWORD(dwExt) + 5; // �����኱����Ȃ��̂ŕ␳
		h = HIWORD(dwExt);
		GetWindowRect(hwnd, &r);
		point.x = r.left;
		point.y = r.top;
		ScreenToClient(Dialog, &point);
		MoveWindow(hwnd, point.x, point.y, w, h, TRUE);

		hwnd = GetDlgItem(Dialog, IDC_FORUM_URL);
		hdc = GetDC(hwnd);
		SelectObject(hdc, DlgAboutFont);
		GetDlgItemText(Dialog, IDC_FORUM_URL, uimsg, sizeof(uimsg));
		dwExt = GetTabbedTextExtent(hdc,uimsg,strlen(uimsg),0,NULL);
		w = LOWORD(dwExt) + 5; // �����኱����Ȃ��̂ŕ␳
		h = HIWORD(dwExt);
		GetWindowRect(hwnd, &r);
		point.x = r.left;
		point.y = r.top;
		ScreenToClient(Dialog, &point);
		MoveWindow(hwnd, point.x, point.y, w, h, TRUE);

		// static text���T�u�N���X������B�������Atabstop, notify�v���p�e�B��L���ɂ��Ă����Ȃ���
		// ���b�Z�[�W���E���Ȃ��B(2005.4.5 yutaka)
		do_subclass_window(GetDlgItem(Dialog, IDC_AUTHOR_URL), &author_url_class);
		do_subclass_window(GetDlgItem(Dialog, IDC_FORUM_URL), &forum_url_class);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(Dialog, 1);
			return TRUE;

		case IDCANCEL:
			EndDialog(Dialog, 0);
			return TRUE;
		}
#ifndef NO_I18N
		if (DlgAboutFont != NULL) {
			DeleteObject(DlgAboutFont);
		}
#endif
		break;
	}
	return FALSE;
}

static PCHAR far LangList[] = {"English","Japanese","Russian",NULL};

BOOL CALLBACK GenDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PTTSet ts;
  WORD w;
  char Temp[7];
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      ts = (PTTSet)lParam;
      SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgGenFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_GENPORT_LABEL, WM_SETFONT, (WPARAM)DlgGenFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_GENPORT, WM_SETFONT, (WPARAM)DlgGenFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_GENLANGLABEL, WM_SETFONT, (WPARAM)DlgGenFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_GENLANG, WM_SETFONT, (WPARAM)DlgGenFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgGenFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgGenFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_GENHELP, WM_SETFONT, (WPARAM)DlgGenFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgGenFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_GEN_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_GENPORT_LABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_GEN_PORT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_GENPORT_LABEL, uimsg);
      GetDlgItemText(Dialog, IDC_GENLANGLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_GEN_LANG", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_GENLANGLABEL, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_GENHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_GENHELP, uimsg);
#endif

      SendDlgItemMessage(Dialog, IDC_GENPORT, CB_ADDSTRING,
			 0, (LPARAM)"TCP/IP");
      strcpy(Temp,"COM");
      for (w=1;w<=ts->MaxComPort;w++)
      {
	uint2str(w,&Temp[3],3);
	SendDlgItemMessage(Dialog, IDC_GENPORT, CB_ADDSTRING,
			   0, (LPARAM)Temp);
      }
      if (ts->PortType==IdSerial)
      {
	if (ts->ComPort <= ts->MaxComPort)
	  w = ts->ComPort;
	else
	  w = 1; // COM1
      }
      else
	w = 0; // TCP/IP
      SendDlgItemMessage(Dialog, IDC_GENPORT, CB_SETCURSEL,w,0);

      if ((ts->MenuFlag & MF_NOLANGUAGE)==0)
      {
	ShowDlgItem(Dialog,IDC_GENLANGLABEL,IDC_GENLANG);
	SetDropDownList(Dialog, IDC_GENLANG, LangList, ts->Language);
      }
      return TRUE;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
	  if (ts!=NULL)
	  {
	    w = (WORD)GetCurSel(Dialog, IDC_GENPORT);
	    if (w>1)
	    {
	      ts->PortType = IdSerial;
	      ts->ComPort = w-1;
	    }
	    else
	      ts->PortType = IdTCPIP;
	    if ((ts->MenuFlag & MF_NOLANGUAGE)==0)
	      ts->Language = (WORD)GetCurSel(Dialog, IDC_GENLANG);
	  }

	  // TTXKanjiMenu �̂��߂ɁAOK �������Ƀ��j���[�ĕ`���
	  // ���b�Z�[�W���΂��悤�ɂ����B (2007.7.14 maya)
	  PostMessage(GetParent(Dialog),WM_USER_CHANGEMENU,0,0);

	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgGenFont != NULL) {
        DeleteObject(DlgGenFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgGenFont != NULL) {
        DeleteObject(DlgGenFont);
      }
#endif
	  return TRUE;

	case IDC_GENHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
      }
  }
  return FALSE;
}

BOOL CALLBACK WinListDlg(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  PBOOL Close;
  int n;
  HWND Hw;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      Close = (PBOOL)lParam;
      SetWindowLong(Dialog, DWL_USER, lParam);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgWinlistFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_WINLISTLABEL, WM_SETFONT, (WPARAM)DlgWinlistFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINLISTLIST, WM_SETFONT, (WPARAM)DlgWinlistFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgWinlistFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgWinlistFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINLISTCLOSE, WM_SETFONT, (WPARAM)DlgWinlistFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_WINLISTHELP, WM_SETFONT, (WPARAM)DlgWinlistFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgWinlistFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WINLIST_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_WINLISTLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WINLIST_LABEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINLISTLABEL, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WINLIST_OPEN", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_WINLISTCLOSE, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_WINLSIT_CLOSEWIN", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINLISTCLOSE, uimsg);
      GetDlgItemText(Dialog, IDC_WINLISTHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_WINLISTHELP, uimsg);
#endif

      SetWinList(GetParent(Dialog),Dialog,IDC_WINLISTLIST);
      return TRUE;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  n = SendDlgItemMessage(Dialog,IDC_WINLISTLIST,
	    LB_GETCURSEL, 0, 0);
	  if (n!=CB_ERR)
	    SelectWin(n);
	  EndDialog(Dialog, 1);
#ifndef NO_I18N
      if (DlgWinlistFont != NULL) {
        DeleteObject(DlgWinlistFont);
      }
#endif
	  return TRUE;

	case IDCANCEL:
	  EndDialog(Dialog, 0);
#ifndef NO_I18N
      if (DlgWinlistFont != NULL) {
        DeleteObject(DlgWinlistFont);
      }
#endif
	  return TRUE;
	case IDC_WINLISTLIST:
#ifdef TERATERM32
	  if (HIWORD(wParam)==LBN_DBLCLK)
#else
	  if (HIWORD(lParam)==LBN_DBLCLK)
#endif
	    PostMessage(Dialog,WM_COMMAND,IDOK,0);
	  break;
	case IDC_WINLISTCLOSE:
	  n = SendDlgItemMessage(Dialog,IDC_WINLISTLIST,
	    LB_GETCURSEL, 0, 0);
	  if (n==CB_ERR) break;
	  Hw = GetNthWin(n);
	  if (Hw!=GetParent(Dialog))
	  {
	    if (! IsWindowEnabled(Hw))
	    {
	      MessageBeep(0);
	      break;
	    }
	    SendDlgItemMessage(Dialog,IDC_WINLISTLIST,
	      LB_DELETESTRING,n,0);
	    PostMessage(Hw,WM_SYSCOMMAND,SC_CLOSE,0);
#ifndef NO_I18N
        if (DlgWinlistFont != NULL) {
          DeleteObject(DlgWinlistFont);
        }
#endif
	  }
	  else {
	    Close = (PBOOL)GetWindowLong(Dialog,DWL_USER);
	    if (Close!=NULL) *Close = TRUE;
	    EndDialog(Dialog, 1);
#ifndef NO_I18N
        if (DlgWinlistFont != NULL) {
          DeleteObject(DlgWinlistFont);
        }
#endif
	    return TRUE;
	  }
	  break;
	case IDC_WINLISTHELP:
	  PostMessage(GetParent(Dialog),WM_USER_DLGHELP2,0,0);
      }
  }
  return FALSE;
}

BOOL FAR PASCAL SetupTerminal(HWND WndParent, PTTSet ts)
{
#ifndef TERATERM32
  DLGPROC TermProc;
  BOOL Ok;
#endif

  int i;

  if (ts->Language==IdJapanese) // Japanese mode
    i = IDD_TERMDLGJ;
  else if (ts->Language==IdRussian) // Russian mode
    i = IDD_TERMDLGR;
  else
    i = IDD_TERMDLG;

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(i),
      WndParent, TermDlg, (LPARAM)ts);
#else
  TermProc = MakeProcInstance(TermDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(i),
    WndParent, TermProc, (LPARAM)ts);
  FreeProcInstance(TermProc);
  return Ok;
#endif
}

BOOL FAR PASCAL SetupWin(HWND WndParent, PTTSet ts)
{
#ifndef TERATERM32
  DLGPROC WinProc;
  BOOL Ok;
#endif

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_WINDLG),
      WndParent, WinDlg, (LPARAM)ts);
#else
  WinProc = MakeProcInstance(WinDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_WINDLG),
    WndParent, WinProc, (LPARAM)ts);
  FreeProcInstance(WinProc);
  return Ok;
#endif
}

BOOL FAR PASCAL SetupKeyboard(HWND WndParent, PTTSet ts)
{
#ifndef TERATERM32
  DLGPROC KeybProc;
  BOOL Ok;
#endif

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_KEYBDLG),
      WndParent, KeybDlg, (LPARAM)ts);
#else
  KeybProc = MakeProcInstance(KeybDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_KEYBDLG),
    WndParent, KeybProc, (LPARAM)ts);
  FreeProcInstance(KeybProc);
  return Ok;
#endif
}

BOOL FAR PASCAL SetupSerialPort(HWND WndParent, PTTSet ts)
{
#ifndef TERATERM32
  DLGPROC SerialProc;
  BOOL Ok;
#endif

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_SERIALDLG),
      WndParent, SerialDlg, (LPARAM)ts);
#else
  SerialProc = MakeProcInstance(SerialDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_SERIALDLG),
    WndParent, SerialProc, (LPARAM)ts);
  FreeProcInstance(SerialProc);
  return Ok;
#endif
}

BOOL FAR PASCAL SetupTCPIP(HWND WndParent, PTTSet ts)
{
#ifndef TERATERM32
  DLGPROC TCPIPProc;
  BOOL Ok;
#endif

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_TCPIPDLG),
      WndParent, TCPIPDlg, (LPARAM)ts);
#else
  TCPIPProc = MakeProcInstance(TCPIPDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_TCPIPDLG),
    WndParent, TCPIPProc, (LPARAM)ts);
  FreeProcInstance(TCPIPProc);
  return Ok;
#endif
}

BOOL FAR PASCAL GetHostName(HWND WndParent, PGetHNRec GetHNRec)
{
#ifndef TERATERM32
  DLGPROC HostProc;
  BOOL Ok;
#endif

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_HOSTDLG),
      WndParent, HostDlg, (LPARAM)GetHNRec);
#else
  HostProc = MakeProcInstance(HostDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_HOSTDLG),
    WndParent, HostProc, (LPARAM)GetHNRec);
  FreeProcInstance(HostProc);
  return Ok;
#endif
}

BOOL FAR PASCAL ChangeDirectory(HWND WndParent, PCHAR CurDir)
{
#ifndef TERATERM32
  DLGPROC DirProc;
  BOOL Ok;
#endif

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_DIRDLG),
      WndParent, DirDlg, (LPARAM)CurDir);
#else
  DirProc = MakeProcInstance(DirDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_DIRDLG),
    WndParent, DirProc, (LPARAM)CurDir);
  FreeProcInstance(DirProc);
  return Ok;
#endif
}

BOOL FAR PASCAL AboutDialog(HWND WndParent)
{
#ifndef TERATERM32
  DLGPROC AboutProc;
  BOOL Ok;
#endif
#ifdef TERATERM32
  return
    (BOOL)DialogBox(hInst,
      MAKEINTRESOURCE(IDD_ABOUTDLG),
      WndParent, AboutDlg);
#else
  AboutProc = MakeProcInstance(AboutDlg, hInst);
  Ok = (BOOL)DialogBox(hInst,
    MAKEINTRESOURCE(IDD_ABOUTDLG),
    WndParent, AboutProc);
  FreeProcInstance(AboutProc);
  return Ok;
#endif
}

BOOL CALLBACK TFontHook(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
  LPCHOOSEFONT cf;
  PTTSet ts;
#ifndef NO_I18N
  char uimsg[MAX_UIMSG];
  LOGFONT logfont;
  HFONT font;
#endif

  switch (Message) {
    case WM_INITDIALOG:
      cf = (LPCHOOSEFONT)lParam;
      ts = (PTTSet)(cf->lCustData);
      SetWindowLong(Dialog, DWL_USER, (LPARAM)ts);

#ifndef NO_I18N
      font = (HFONT)SendMessage(Dialog, WM_GETFONT, 0, 0);
      GetObject(font, sizeof(LOGFONT), &logfont);
      if (get_lang_font("DLG_SYSTEM_FONT", Dialog, &logfont, &DlgFontFont, UILanguageFile)) {
        SendDlgItemMessage(Dialog, IDC_FONTLABEL, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SIZELABEL, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_FONTBOLD, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_FONTCHARSET1, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_FONTCHARSET2, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_SAMPLE, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDOK, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDCANCEL, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
        SendDlgItemMessage(Dialog, IDC_FONTHELP, WM_SETFONT, (WPARAM)DlgFontFont, MAKELPARAM(TRUE,0));
      }
      else {
        DlgFontFont = NULL;
      }

      GetWindowText(Dialog, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_FONT_TITLE", uimsg, UILanguageFile);
      SetWindowText(Dialog, uimsg);
      GetDlgItemText(Dialog, IDC_FONTLABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_FONT_FONT", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_FONTLABEL, uimsg);
      GetDlgItemText(Dialog, IDC_SIZELABEL, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_FONT_SIZE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SIZELABEL, uimsg);
      GetDlgItemText(Dialog, IDC_FONTBOLD, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_FONT_BOLD", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_FONTBOLD, uimsg);
      GetDlgItemText(Dialog, IDC_FONTCHARSET1, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_FONT_CHARSET", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_FONTCHARSET1, uimsg);
      GetDlgItemText(Dialog, IDC_SAMPLE, uimsg, sizeof(uimsg));
      get_lang_msg("DLG_FONT_SAMPLE", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_SAMPLE, uimsg);
      GetDlgItemText(Dialog, IDOK, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_OK", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDOK, uimsg);
      GetDlgItemText(Dialog, IDCANCEL, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_CANCEL", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDCANCEL, uimsg);
      GetDlgItemText(Dialog, IDC_FONTHELP, uimsg, sizeof(uimsg));
      get_lang_msg("BTN_HELP", uimsg, UILanguageFile);
      SetDlgItemText(Dialog, IDC_FONTHELP, uimsg);
#endif

      ShowDlgItem(Dialog,IDC_FONTBOLD,IDC_FONTBOLD);
      SetRB(Dialog,ts->EnableBold,IDC_FONTBOLD,IDC_FONTBOLD);
      if (ts->Language==IdRussian)
      {
	ShowDlgItem(Dialog,IDC_FONTCHARSET1,IDC_FONTCHARSET2);
	SetDropDownList(Dialog,IDC_FONTCHARSET2,RussList,ts->RussFont);
      }
      SetFocus(GetDlgItem(Dialog,1136));
      break;
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
	case IDOK:
	  ts = (PTTSet)GetWindowLong(Dialog,DWL_USER);
	  if (ts!=NULL)
	  {
	    GetRB(Dialog,&ts->EnableBold,IDC_FONTBOLD,IDC_FONTBOLD);
	    if (ts->Language==IdRussian)
	      ts->RussFont = (WORD)GetCurSel(Dialog, IDC_FONTCHARSET2);
	  }
#ifndef NO_I18N
      if (DlgFontFont != NULL) {
        DeleteObject(DlgFontFont);
      }
#endif
	  break;
	case IDCANCEL:
#ifndef NO_I18N
      if (DlgFontFont != NULL) {
        DeleteObject(DlgFontFont);
      }
#endif
	  break;
      }
  }
  return FALSE;
}

#ifndef TERATERM32
  typedef UINT (CALLBACK *LPCFHOOKPROC)(HWND,UINT,WPARAM,LPARAM);
#endif

BOOL FAR PASCAL ChooseFontDlg(HWND WndParent, LPLOGFONT LogFont, PTTSet ts)
{
  CHOOSEFONT cf;
  BOOL Ok;

  memset(&cf, 0, sizeof(CHOOSEFONT));
  cf.lStructSize = sizeof(CHOOSEFONT);
  cf.hwndOwner = WndParent;
  cf.lpLogFont = LogFont;
  cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT |
	     CF_FIXEDPITCHONLY | CF_SHOWHELP | CF_ENABLETEMPLATE;
  if (ts!=NULL)
  {
    cf.Flags = cf.Flags | CF_ENABLEHOOK;
#ifdef TERATERM32
    cf.lpfnHook = (LPCFHOOKPROC)(&TFontHook);
#else
    cf.lpfnHook = (LPCFHOOKPROC)MakeProcInstance(TFontHook, hInst);
#endif
    cf.lCustData = (DWORD)ts;
  }
  cf.lpTemplateName = MAKEINTRESOURCE(IDD_FONTDLG);
  cf.nFontType = REGULAR_FONTTYPE;
  cf.hInstance = hInst;
  Ok = ChooseFont(&cf);
#ifndef TERATERM32
  FreeProcInstance(cf.lpfnHook);
#endif
  return Ok;
}

BOOL FAR PASCAL SetupGeneral(HWND WndParent, PTTSet ts)
{
#ifndef TERATERM32
  DLGPROC GenProc;
  BOOL Ok;
#endif

#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_GENDLG),
      WndParent, (DLGPROC)&GenDlg, (LPARAM)ts);
#else
  GenProc = MakeProcInstance(GenDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_GENDLG),
    WndParent, GenProc, (LPARAM)ts);
  FreeProcInstance(GenProc);
  return Ok;
#endif
}

BOOL FAR PASCAL WindowWindow(HWND WndParent, PBOOL Close)
{
#ifndef TERATERM32
  DLGPROC WinListProc;
  BOOL Ok;
#endif

  *Close = FALSE;
#ifdef TERATERM32
  return
    (BOOL)DialogBoxParam(hInst,
      MAKEINTRESOURCE(IDD_WINLISTDLG),
      WndParent,
      (DLGPROC)&WinListDlg, (LPARAM)Close);
#else
  WinListProc = MakeProcInstance(WinListDlg, hInst);
  Ok = (BOOL)DialogBoxParam(hInst,
    MAKEINTRESOURCE(IDD_WINLISTDLG),
    WndParent, WinListProc, (LPARAM)Close);
  FreeProcInstance(WinListProc);
  return Ok;
#endif
}

#ifdef TERATERM32
#ifdef WATCOM
  #pragma off (unreferenced);
#endif
BOOL WINAPI DllMain(HANDLE hInstance,
		    ULONG ul_reason_for_call,
		    LPVOID lpReserved)
#ifdef WATCOM
  #pragma on (unreferenced);
#endif
{
#ifndef NO_I18N
  PMap pm;
  HANDLE HMap = NULL;
#endif

  hInst = hInstance;
  switch (ul_reason_for_call) {
    case DLL_THREAD_ATTACH:
      /* do thread initialization */
      break;
    case DLL_THREAD_DETACH:
      /* do thread cleanup */
      break;
    case DLL_PROCESS_ATTACH:
      /* do process initialization */
#ifndef NO_I18N
      HMap = CreateFileMapping(
        (HANDLE) 0xFFFFFFFF, NULL, PAGE_READONLY,
        0, sizeof(TMap), TT_FILEMAPNAME);
      if (HMap != NULL) {
        pm = (PMap)MapViewOfFile(
        HMap,FILE_MAP_READ,0,0,0);
        if (pm != NULL) {
          strncpy(UILanguageFile, pm->ts.UILanguageFile, sizeof(UILanguageFile)-1);
          UILanguageFile[sizeof(UILanguageFile)-1] = 0;
	    }
      }
#endif
      break;
    case DLL_PROCESS_DETACH:
      /* do process cleanup */
      break;
  }
  return TRUE;
}
#else
#ifdef WATCOM
#pragma off (unreferenced);
#endif
int CALLBACK LibMain(HANDLE hInstance, WORD wDataSegment,
		     WORD wHeapSize, LPSTR lpszCmdLine)
#ifdef WATCOM
#pragma on (unreferenced);
#endif
{
  hInst = hInstance;
  return (1);
}
#endif

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.26  2007/07/20 21:55:27  maya
 * �V���A���|�[�g�_�C�A���O�Ń{�[���[�g���ύX����邱�Ƃ�����̂ŁA�I�����Ƀ^�C�g����ύX����悤�ɂ����B
 *
 * Revision 1.25  2007/07/14 00:22:52  maya
 * ���ꂪ���{��̂Ƃ��̂� TTXKanjiMenu ���\�������悤�ɂ����B
 *
 * Revision 1.24  2007/07/05 12:15:48  doda
 * ���p�\��COM�|�[�g���������A�V�����ڑ��_�C�A���O�ŃV���A���|�[�g�̑I���𖳌��������B
 *
 * Revision 1.23  2007/07/02 10:50:43  doda
 * ���o�[�W�����Ƃ̊Ԃł͋��L�������ɂ��ݒ�̋��L���o���Ȃ����߁A
 * �t�@�C���}�b�s���O�I�u�W�F�N�g����ύX�����B
 *
 * Revision 1.22  2007/06/06 14:04:03  maya
 * �v���v���Z�b�T�ɂ��\���̂��ς���Ă��܂��̂ŁAINET6 �� I18N �� #define ���t�]�������B
 *
 * Revision 1.21  2007/04/16 15:29:46  maya
 * �ύX��̃t�H���g�T�C�Y���擾����悤�ɕύX�����B
 *
 * Revision 1.20  2007/04/16 08:45:23  maya
 * URL ���܂ރe�L�X�g�̃T�C�Y�𓮓I�ɕύX����悤�ɂ����B
 *
 * Revision 1.19  2007/03/04 18:00:33  doda
 * New connection$B$*$h$S(BSerial port setup$B%@%$%"%m%0$G!"MxMQ2DG=$J%7%j%"%k%]!<%H$N$_$rI=<($9$k$h$&$K$7$?!#(B
 *
 * Revision 1.18  2007/01/21 16:18:37  maya
 * �\�����b�Z�[�W�̓ǂݍ��ݑΉ�
 *
 * Revision 1.17  2007/01/21 15:18:38  yutakapon
 * Terminal setup�_�C�A���O�� New-line �� Receive �� "LF" ��ǉ������B
 * ��M���̉��s�R�[�h�� LF �̏ꍇ�́A�T�[�o���� LF �݂̂������Ă����
 * ���肵�ACR+LF�Ƃ��Ĉ����悤�ɂ���B
 *
 * Revision 1.16  2007/01/04 15:11:46  maya
 * �\�����b�Z�[�W�̓ǂݍ��ݑΉ�
 *
 * Revision 1.15  2006/11/23 02:19:14  maya
 * �\�����b�Z�[�W������t�@�C������ǂݍ��݂ރR�[�h�̍쐬���J�n�����B
 *
 * Revision 1.14  2006/07/24 14:19:18  yutakakn
 * Oniguruma 4.2.0�֍����ւ������Ƃɍ��킹�āA�o�[�W�����\���̂���������ύX�����B
 *
 * Revision 1.13  2006/03/12 14:27:41  yutakakn
 *   �EAdditional settings�_�C�A���O�ɂ�����E�B���h�E�̔������ύX�𑦍��ɔ��f������悤�ɂ����iteraterm.ini �� AlphaBlend=256 �̏ꍇ�̂݁j�B
 *   �E�����̔w�i�F���X�N���[���̔w�i�F�ƈ�v������p�b�`�̃o�O���C�������B�p�b�`�쐬�Ɋ��ӂ��܂���337��
 *
 * Revision 1.12  2006/03/02 16:15:49  yutakakn
 *   �E�����̔w�i�F���X�N���[���̔w�i�F�ƈ�v������悤�ɂ����B����ɂƂ��Ȃ�Window setup�_�C�A���O��"Always use Normal text's BG"�`�F�b�N�{�b�N�X��ǉ������B�܂��Ateraterm.ini��UseNormalBGColor�G���g����ǉ������B�p�b�`�쐬�Ɋ��ӂ��܂���337��
 *
 * Revision 1.11  2006/02/18 08:40:07  yutakakn
 *   �E�R���p�C���� Visual Studio 2005 Standard Edition �ɐ؂�ւ����B
 *   �Estricmp()��_stricmp()�֒u������
 *   �Estrnicmp()��_strnicmp()�֒u������
 *   �Estrdup()��_strdup()�֒u������
 *   �Echdir()��_chdir()�֒u������
 *   �Egetcwd()��_getcwd()�֒u������
 *   �Estrupr()��_strupr()�֒u������
 *   �Etime_t��64�r�b�g���ɂƂ��Ȃ��x�����b�Z�[�W��}�~����
 *   �ETeraTerm Menu���r���h�G���[�ƂȂ錻�ۂɑΏ�����
 *   �EOniguruma 4.0.1�֍����ւ���
 *
 * Revision 1.10  2005/11/30 15:32:12  yutakakn
 * �V���A���ڑ���COM�ő�|�[�g��99�܂Ŋg�������B
 * �{�[���[�g��230400, 460800, 921600��ǉ������B
 *
 * Revision 1.9  2005/11/03 13:34:26  yutakakn
 *   �Eteraterm.ini��ۑ�����Ƃ��ɏ������݂ł��邩�ǂ����̔��ʂ�ǉ������B
 *   �ETCP/IP setup�_�C�A���O��"Term type"����ɗL���Ƃ���悤�ɂ����B
 *
 * Revision 1.8  2005/10/08 14:56:06  yutakakn
 * Oniguruma�̃o�[�W�������\���������ύX�B
 *
 * Revision 1.7  2005/10/05 17:01:41  yutakakn
 * Oniguruma�̃o�[�W�������o�[�W�������ɒǉ��B
 *
 * Revision 1.6  2005/04/07 14:13:23  yutakakn
 * �EAdditional settings�ł̃}�E�X�J�[�\����ʂ�ݒ莞�ɕύX�����悤�ɂ����B
 * �E�o�[�W�������_�C�A���O��URL���_�u���N���b�N����ƁA�u���E�U���N�������悤�ɂ����B
 *
 * Revision 1.5  2005/04/03 13:42:07  yutakakn
 * URL��������_�u���N���b�N����ƃu���E�U���N�����邵������ǉ��i�΍莁�p�b�`���x�[�X�j�B
 *
 * Revision 1.4  2005/03/14 13:29:40  yutakakn
 * 2�߈ȍ~�̃v���Z�X�ŁATeraTerm�o�[�W�������������擾����Ȃ����ւ̑Ώ��B
 *
 * Revision 1.3  2005/02/28 14:30:35  yutakakn
 * �o�[�W�����_�C�A���O�ɕ\������TeraTerm�̃o�[�W�������Attermpro.exe��
 * �o�[�W������񂩂�擾����悤�ɂ����B
 *
 * Revision 1.2  2004/11/28 13:14:51  yutakakn
 * �X�N���[���o�b�t�@�̓��͌�����5����8�֊g�������B
 * 
 */