/* Tera Term
 Copyright(C) 1994-1998 T. Teranishi
 All rights reserved. */

/* TERATERM.EXE, file transfer routines */
#ifdef __cplusplus
extern "C" {
#endif
typedef BOOL (FAR PASCAL *PGetSetupFname)
  (HWND HWin, WORD FuncId, PTTSet ts);
typedef BOOL (FAR PASCAL *PGetTransFname)
#ifdef I18N
  (PFileVar fv, PCHAR CurDir, WORD FuncId, LPLONG Option, PTTSet ts);
#else
  (PFileVar fv, PCHAR CurDir, WORD FuncId, LPLONG Option);
#endif
typedef BOOL (FAR PASCAL *PGetMultiFname)
#ifdef I18N
  (PFileVar fv, PCHAR CurDir, WORD FuncId, LPWORD Option, PTTSet ts);
#else
  (PFileVar fv, PCHAR CurDir, WORD FuncId, LPWORD Option);
#endif
typedef BOOL (FAR PASCAL *PGetGetFname)
  (HWND HWin, PFileVar fv);
#ifdef I18N
typedef void (FAR PASCAL *PSetFileVar) (PFileVar fv, PTTSet ts);
#else
typedef void (FAR PASCAL *PSetFileVar) (PFileVar fv);
#endif
typedef BOOL (FAR PASCAL *PGetXFname)
#ifdef I18N
  (HWND HWin, BOOL Receive, LPLONG Option, PFileVar fv, PCHAR CurDir, PTTSet ts);
#else
  (HWND HWin, BOOL Receive, LPLONG Option, PFileVar fv, PCHAR CurDir);
#endif
typedef void (FAR PASCAL *PProtoInit)
  (int Proto, PFileVar fv, PCHAR pv, PComVar cv, PTTSet ts);
typedef BOOL (FAR PASCAL *PProtoParse)
#ifdef I18N
  (int Proto, PFileVar fv, PCHAR pv, PComVar cv, PTTSet ts);
#else
  (int Proto, PFileVar fv, PCHAR pv, PComVar cv);
#endif
typedef void (FAR PASCAL *PProtoTimeOutProc)
  (int Proto, PFileVar fv, PCHAR pv, PComVar cv);
typedef BOOL (FAR PASCAL *PProtoCancel)
  (int Proto, PFileVar fv, PCHAR pv, PComVar cv);

extern PGetSetupFname GetSetupFname;
extern PGetTransFname GetTransFname;
extern PGetMultiFname GetMultiFname;
extern PGetGetFname GetGetFname;
extern PSetFileVar SetFileVar;
extern PGetXFname GetXFname;
extern PProtoInit ProtoInit;
extern PProtoParse ProtoParse;
extern PProtoTimeOutProc ProtoTimeOutProc;
extern PProtoCancel ProtoCancel;

BOOL LoadTTFILE();
BOOL FreeTTFILE();
BOOL NewFileVar(PFileVar *FV);
void FreeFileVar(PFileVar *FV);

void LogStart();
void Log1Byte(BYTE b);
void LogToFile();
BOOL CreateLogBuf();
void FreeLogBuf();
BOOL CreateBinBuf();
void FreeBinBuf();

void FileSendStart();
void FileSend();
void FLogChangeButton(BOOL Pause);
void FLogRefreshNum();
void FileTransEnd(WORD OpId);
int FSOut1(BYTE b);
int FSEcho1(BYTE b);

void ProtoEnd();
int ProtoDlgParse();
void ProtoDlgTimeOut();
void ProtoDlgCancel();
void KermitStart(int mode);
void XMODEMStart(int mode);
void ZMODEMStart(int mode);
void BPStart(int mode);
void QVStart(int mode);

extern PFileVar LogVar, SendVar, FileVar;
extern BOOL FileLog, BinLog, DDELog;

void logfile_lock_initialize(void);
void CommentLogToFile(char *buf, int size);

#ifdef __cplusplus
}
#endif

