#include "teraterm.h"
#include "tttypes.h"
#include "ttplugin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define ORDER 9999

static HANDLE hInst; /* Instance handle of TTX*.DLL */

typedef struct {
	PReadIniFile origReadIniFile;
	PWriteIniFile origWriteIniFile;
	wchar_t origIniFileName[MAXPATHLEN];
} TInstVar;

static TInstVar *pvar;
static TInstVar InstVar;

static void PASCAL TTXReadIniFile(const wchar_t *fn, PTTSet ts) {
	wcscpy_s(pvar->origIniFileName, _countof(pvar->origIniFileName), fn);
	(pvar->origReadIniFile)(fn, ts);
}

static void PASCAL TTXWriteIniFile(const wchar_t *fn, PTTSet ts) {
	CopyFileW(pvar->origIniFileName, fn, TRUE);
	(pvar->origWriteIniFile)(fn, ts);
}

static void PASCAL TTXGetSetupHooks(TTXSetupHooks *hooks) {
	if (pvar->origIniFileName[0] == 0) {
		pvar->origReadIniFile = *hooks->ReadIniFile;
		*hooks->ReadIniFile = TTXReadIniFile;
	}
	else {
		pvar->origWriteIniFile = *hooks->WriteIniFile;
		*hooks->WriteIniFile = TTXWriteIniFile;
	}
}

static TTXExports Exports = {
	sizeof(TTXExports),
	ORDER,

	NULL,	// TTXInit,
	NULL,	// TTXGetUIHooks,
	TTXGetSetupHooks,
	NULL,	// TTXOpenTCP,
	NULL,	// TTXCloseTCP,
	NULL,	// TTXSetWinSize,
	NULL,	// TTXModifyMenu,
	NULL,	// TTXModifyPopupMenu,
	NULL,	// TTXProcessCommand,
	NULL,	// TTXEnd,
	NULL,	// TTXSetCommandLine,
	NULL,	// TTXOpenFile,
	NULL	// TTXCloseFile
};

BOOL __declspec(dllexport) PASCAL TTXBind(WORD Version, TTXExports *exports) {
	int size = sizeof(Exports) - sizeof(exports->size);

	if (size > exports->size) {
		size = exports->size;
	}
	memcpy((char *)exports + sizeof(exports->size),
		(char *)&Exports + sizeof(exports->size),
		size);
	return TRUE;
}

BOOL WINAPI DllMain(HANDLE hInstance,
		    ULONG ul_reason_for_call,
		    LPVOID lpReserved)
{
	switch( ul_reason_for_call ) {
		case DLL_THREAD_ATTACH:
			/* do thread initialization */
			break;
		case DLL_THREAD_DETACH:
			/* do thread cleanup */
			break;
		case DLL_PROCESS_ATTACH:
			/* do process initialization */
			hInst = hInstance;
			pvar = &InstVar;
			break;
		case DLL_PROCESS_DETACH:
			/* do process cleanup */
			break;
	}
	return TRUE;
}
