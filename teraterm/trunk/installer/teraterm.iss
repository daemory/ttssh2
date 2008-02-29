#define AppName "UTF-8 TeraTerm Pro with TTSSH2"
#define AppVer "4.58"

[Setup]
AppCopyright=TeraTerm Project
AppName={#AppName}
AppVerName={#AppName} {#AppVer}
LicenseFile=..\release\license.txt
DefaultDirName={pf}\teraterm
DefaultGroupName={#AppName}
ShowLanguageDialog=yes
AllowNoIcons=true
UninstallDisplayIcon={app}\ttermpro.exe
AppMutex=TeraTermProAppMutex, TeraTermProMacroAppMutex, TeraTermProKeycodeAppMutex, TeraTermMenuAppMutex, CygTermAppMutex
OutputBaseFilename=teraterm_utf8-{#AppVer}
PrivilegesRequired=none

[Languages]
Name: en; MessagesFile: compiler:Default.isl
Name: ja; MessagesFile: compiler:Languages\Japanese.isl

[Dirs]
Name: {app}\Collector; Components: Collector
Name: {app}\theme; Components: TeraTerm
Name: {app}\theme\scale; Components: TeraTerm
Name: {app}\theme\tile; Components: TeraTerm
Name: {app}\plugin; Components: TeraTerm
Name: {app}\lang; Components: TeraTerm

[Files]
Source: ..\visualc\bin\release\ttermpro.exe; DestDir: {app}; Components: TeraTerm; Flags: ignoreversion
Source: ..\visualc\bin\release\ttpcmn.dll; DestDir: {app}; Components: TeraTerm
Source: ..\visualc\bin\release\ttpdlg.dll; DestDir: {app}; Components: TeraTerm
Source: ..\visualc\bin\release\ttpfile.dll; DestDir: {app}; Components: TeraTerm
Source: ..\visualc\bin\release\ttpset.dll; DestDir: {app}; Components: TeraTerm
Source: ..\visualc\bin\release\ttptek.dll; DestDir: {app}; Components: TeraTerm
Source: ..\release\TERATERM.INI; DestDir: {app}; Components: TeraTerm; Flags: onlyifdoesntexist uninsneveruninstall; Permissions: authusers-modify
Source: ..\release\TSPECIAL1.TTF; DestDir: {fonts}; Components: TeraTerm; Attribs: readonly; Flags: overwritereadonly uninsneveruninstall; FontInstall: Tera Special; Check: isPowerUsersMore
Source: ..\..\doc\en\teraterm.chm; DestDir: {app}; Components: TeraTerm
Source: ..\..\doc\jp\teratermj.chm; DestDir: {app}; Components: TeraTerm
Source: ..\release\license.txt; DestDir: {app}; Components: TeraTerm
Source: ..\release\FUNCTION.CNF; DestDir: {app}; Components: TeraTerm
Source: ..\release\IBMKEYB.CNF; DestDir: {app}; Components: TeraTerm
Source: ..\release\KEYBOARD.CNF; DestDir: {app}; Components: TeraTerm; Flags: onlyifdoesntexist uninsneveruninstall; Permissions: authusers-modify
Source: ..\release\NT98KEYB.CNF; DestDir: {app}; Components: TeraTerm
Source: ..\release\PC98KEYB.CNF; DestDir: {app}; Components: TeraTerm
Source: ..\visualc\bin\release\keycode.exe; DestDir: {app}; Components: TeraTerm
Source: ..\visualc\bin\release\ttpmacro.exe; DestDir: {app}; Components: TeraTerm; Flags: ignoreversion
Source: ..\release\delpassw.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\dialup.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\login.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\mpause.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\random.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\screencapture.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\ssh2login.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\wait_regex.ttl; DestDir: {app}; Components: TeraTerm
Source: ..\release\lang\Japanese.lng; DestDir: {app}\lang; Components: TeraTerm; Attribs: readonly; Flags: uninsremovereadonly overwritereadonly
Source: ..\release\lang\German.lng; DestDir: {app}\lang; Components: TeraTerm; Attribs: readonly; Flags: uninsremovereadonly overwritereadonly
Source: ..\..\ttssh2\ttxssh\Release\ttxssh.dll; DestDir: {app}; Components: TTSSH; Flags: ignoreversion
Source: ..\release\ssh_known_hosts; DestDir: {app}; Components: TTSSH; Flags: onlyifdoesntexist uninsneveruninstall; Permissions: authusers-modify
Source: ..\cygterm\cygterm.exe; DestDir: {app}; Components: cygterm
Source: ..\cygterm\cygterm.cfg; DestDir: {app}; Components: cygterm; Flags: onlyifdoesntexist uninsneveruninstall; Permissions: authusers-modify
Source: ..\cygterm\cyglaunch.exe; DestDir: {app}; Components: cygterm
Source: ..\release\LogMeTT_license.txt; DestDir: {app}; Components: LogMeTT
Source: ..\release\LogMeTT_README.txt; DestDir: {app}; Components: LogMeTT
Source: ..\release\LogMeTTc.exe; DestDir: {app}; Components: LogMeTT; Flags: ignoreversion
Source: ..\release\LogMeTT.exe; DestDir: {app}; Components: LogMeTT; Flags: ignoreversion
Source: ..\release\logmett.chm; DestDir: {app}; Components: LogMeTT
Source: ..\release\runltt.exe; DestDir: {app}; Components: LogMeTT
Source: ..\release\KeyFile.ini; DestDir: {app}; Components: LogMeTT
Source: ..\release\TTLEdit.exe; DestDir: {app}; Components: LogMeTT; Flags: ignoreversion
Source: ..\release\SynComp.pro; DestDir: {app}; Components: LogMeTT
Source: ..\release\ttmacro.tpl; DestDir: {app}; Components: LogMeTT
Source: ..\source\ttmenu\Release\ttpmenu.exe; DestDir: {app}; Components: TeraTerm_Menu; Flags: ignoreversion
Source: ..\release\ttmenu_readme-j.txt; DestDir: {app}; Components: TeraTerm_Menu
Source: ..\..\TTProxy\Release\TTXProxy.dll; DestDir: {app}; Components: TTProxy; Flags: ignoreversion
Source: ..\release\theme\Advanced.sample; DestDir: {app}\theme\; Components: TeraTerm
Source: ..\release\theme\Scale.INI; DestDir: {app}\theme\; Components: TeraTerm
Source: ..\release\theme\Tile.INI; DestDir: {app}\theme\; Components: TeraTerm
Source: ..\release\theme\scale\23.jpg; DestDir: {app}\theme\scale; Components: TeraTerm
Source: ..\release\theme\scale\43.jpg; DestDir: {app}\theme\scale; Components: TeraTerm
Source: ..\release\theme\tile\03.jpg; DestDir: {app}\theme\tile; Components: TeraTerm
Source: ..\release\theme\tile\44.jpg; DestDir: {app}\theme\tile; Components: TeraTerm
Source: ..\release\plugin\ttAKJpeg.dll; DestDir: {app}\plugin\; Components: TeraTerm
Source: ..\release\plugin\ttAKJpeg.txt; DestDir: {app}\plugin\; Components: TeraTerm
Source: ..\release\Collector\Collector.exe; DestDir: {app}\Collector\; Components: Collector
Source: ..\release\Collector\collector.ini; DestDir: {app}\Collector\; Components: Collector
Source: ..\release\Collector\Collector_org.exe; DestDir: {app}\Collector\; Components: Collector
Source: ..\release\Collector\hthook.dll; DestDir: {app}\Collector\; Components: Collector
Source: ..\release\Collector\mfc70.dll; DestDir: {app}\Collector\; Components: Collector
Source: ..\release\Collector\msvcr70.dll; DestDir: {app}\Collector\; Components: Collector
Source: ..\release\Collector\readme.txt; DestDir: {app}\Collector\; Components: Collector
Source: ..\visualc\bin\release\ttxkanjimenu.dll; DestDir: {app}\; Components: TTXKanjiMenu; Flags: ignoreversion

[Types]
Name: standard; Description: {cm:type_standard}
Name: full; Description: {cm:type_full}
Name: compact; Description: {cm:type_compact}
Name: custom; Description: {cm:type_custom}; Flags: iscustom

[Components]
Name: TeraTerm; Description: TeraTerm & Macro; Flags: fixed; Types: custom compact full standard
Name: TTSSH; Description: TTSSH; Types: compact full standard
Name: cygterm; Description: CygTerm+; Types: full standard; Check: not isIA64
Name: LogMeTT; Description: LogMeTT & TTLEdit; Types: full standard
Name: TeraTerm_Menu; Description: TeraTerm Menu; Types: full
Name: TTProxy; Description: TTProxy; Types: full standard
Name: Collector; Description: Collector; Types: full
Name: TTXKanjiMenu; Description: TTXKanjiMenu; Types: full

[Icons]
Name: {group}\UTF-8 TeraTerm Pro; Filename: {app}\ttermpro.exe; WorkingDir: {app}; IconFilename: {app}\ttermpro.exe; IconIndex: 0; Components: TeraTerm; Flags: createonlyiffileexists
Name: {group}\{cm:UninstallProgram,{#AppName}}; Filename: {uninstallexe}; Components: TeraTerm; Flags: createonlyiffileexists
Name: {group}\LogMeTT; Filename: {app}\LogMeTT.exe; WorkingDir: {app}; IconFilename: {app}\logMeTT.exe; IconIndex: 0; Components: LogMeTT; Flags: createonlyiffileexists
Name: {group}\TTLEdit; Filename: {app}\TTLEdit.exe; WorkingDir: {app}; IconFilename: {app}\TTLEdit.exe; IconIndex: 0; Components: LogMeTT; Flags: createonlyiffileexists
Name: {group}\TeraTerm Menu; Filename: {app}\ttpmenu.exe; WorkingDir: {app}; IconFilename: {app}\ttpmenu.exe; IconIndex: 0; Components: TeraTerm_Menu; Flags: createonlyiffileexists
Name: {group}\Collector; Filename: {app}\Collector\Collector.exe; WorkingDir: {app}\Collector; IconFilename: {app}\Collector\Collector.exe; IconIndex: 0; Components: Collector; Flags: createonlyiffileexists
Name: {userdesktop}\UTF-8 TeraTerm Pro; Filename: {app}\ttermpro.exe; WorkingDir: {app}; IconFilename: {app}\ttermpro.exe; Components: TeraTerm; Tasks: desktopicon; IconIndex: 0; Flags: createonlyiffileexists
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\UTF-8 TeraTerm Pro; Filename: {app}\ttermpro.exe; WorkingDir: {app}; IconFilename: {app}\ttermpro.exe; Components: TeraTerm; Tasks: quicklaunchicon; IconIndex: 0; Flags: createonlyiffileexists
Name: {userstartup}\TeraTerm Menu; Filename: {app}\ttpmenu.exe; WorkingDir: {app}; IconFilename: {app}\ttpmenu.exe; Components: TeraTerm_Menu; IconIndex: 0; Tasks: startupttmenuicon; Flags: createonlyiffileexists
Name: {userstartup}\Collector; Filename: {app}\collector\collector.exe; WorkingDir: {app}\Collector; IconFilename: {app}\collector\collector.exe; Components: Collector; Tasks: startupcollectoricon; IconIndex: 0; Flags: createonlyiffileexists

[INI]
Filename: {app}\teraterm.ini; Section: Tera Term; Key: SaveVTWinPos; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: SaveVTWinPos; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: Xterm256Color; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: Xterm256Color; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: Aixterm16Color; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: Aixterm16Color; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: PcBoldColor; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: PcBoldColor; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: ConfirmChangePaste; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: ConfirmChangePaste; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: LogHideDialog; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: LogHideDialog; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: MaximizedBugTweak; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: MaximizedBugTweak; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: KillFocusCursor; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: KillFocusCursor; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: MouseEventTracking; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: MouseEventTracking; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: HostDialogOnStartup; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: HostDialogOnStartup; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: TranslateWheelToCursor; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: TranslateWheelToCursor; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: ConfirmFileDragAndDrop; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: ConfirmFileDragAndDrop; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: ZmodemRcvCommand; String: rz; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: ZmodemRcvCommand; String: rz; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: XmodemRcvCommand; String: ; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: XmodemRcvCommand; String: ; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: TelAutoDetect; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: TelAutoDetect; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: SelectOnlyByLButton; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: SelectOnlyByLButton; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: ClearComBuffOnOpen; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: ClearComBuffOnOpen; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: DisableAppCursor; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: DisableAppCursor; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: DisableAppKeypad; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: DisableAppKeypad; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: TTSSH; Key: CheckAuthListFirst; String: 0; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: TTSSH; Key: CheckAuthListFirst; String: 0; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: MaxBroadcatHistory; String: 99; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: MaxBroadcatHistory; String: 99; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: TelKeepAliveInterval; String: 300; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: TelKeepAliveInterval; String: 300; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: VTCompatTab; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: VTCompatTab; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: FileSendFilter; String: ; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: FileSendFilter; String: ; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: LogAutoStart; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: LogAutoStart; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: LogDefaultPath; String: ; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: LogDefaultPath; String: ; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: AcceptBroadcast; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: AcceptBroadcast; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: DisableAcceleratorSendBreak; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: DisableAcceleratorSendBreak; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: ConfirmPasteMouseRButton; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: ConfirmPasteMouseRButton; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: BroadcastCommandHistory; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: BroadcastCommandHistory; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: ConnectingTimeout; String: 0; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: ConnectingTimeout; String: 0; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: LogDefaultName; String: teraterm.log; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: LogDefaultName; String: teraterm.log; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: TTSSH; Key: RememberPassword; String: 1; Flags: createkeyifdoesntexist; Components: TTSSH
Filename: {userdocs}\teraterm.ini; Section: TTSSH; Key: RememberPassword; String: 1; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TTSSH
Filename: {app}\teraterm.ini; Section: Tera Term; Key: LogTimestamp; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: LogTimestamp; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: AlphaBlend; String: 255; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: AlphaBlend; String: 255; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: UseNormalBGColor; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: UseNormalBGColor; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: HistoryList; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: HistoryList; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: LogTypePlainText; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: LogTypePlainText; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: URLColor; String: 0,255,0,255,255,255; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: URLColor; String: 0,255,0,255,255,255; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: EnableClickableUrl; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: EnableClickableUrl; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: DisablePasteMouseRButton; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: DisablePasteMouseRButton; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: BG; Key: BGEnable; String: off; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: BG; Key: BGEnable; String: off; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: BG; Key: BGUseAlphaBlendAPI; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: BG; Key: BGUseAlphaBlendAPI; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: BG; Key: BGSPIPath; String: plugin; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: BG; Key: BGSPIPath; String: plugin; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: BG; Key: BGFastSizeMove; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: BG; Key: BGFastSizeMove; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: BG; Key: BGNoFrame; String: on; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: BG; Key: BGNoFrame; String: on; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: BG; Key: BGThemeFile; String: theme\*.ini; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: BG; Key: BGThemeFile; String: theme\*.ini; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: Tera Term; Key: ViewlogEditor; String: notepad.exe; Flags: createkeyifdoesntexist; Components: TeraTerm
Filename: {userdocs}\teraterm.ini; Section: Tera Term; Key: ViewlogEditor; String: notepad.exe; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TeraTerm
Filename: {app}\teraterm.ini; Section: TTSSH; Key: KeyboardInteractive; String: 0; Flags: createkeyifdoesntexist; Components: TTSSH
Filename: {userdocs}\teraterm.ini; Section: TTSSH; Key: KeyboardInteractive; String: 0; Flags: createkeyifdoesntexist; Check: isUserIniExists; Components: TTSSH

[Registry]
Root: HKCU; Subkey: Software\Classes\Folder\shell\cygterm; ValueType: string; ValueData: Cy&gterm Here; Flags: uninsdeletekey; Check: isMinimumOfWin2K; Components: cygterm; Tasks: cygtermhere
Root: HKCU; Subkey: Software\Classes\Folder\shell\cygterm\command; ValueType: string; ValueData: """{app}\cyglaunch.exe"" -nocd -nols -d \""%L\"""; Flags: uninsdeletekey; Check: isMinimumOfWin2K; Components: cygterm; Tasks: cygtermhere
Root: HKCR; Subkey: Folder\shell\cygterm; ValueType: string; ValueData: Cy&gterm Here; Flags: uninsdeletekey; Check: not isMinimumOfWin2K; Components: cygterm; Tasks: cygtermhere
Root: HKCR; Subkey: Folder\shell\cygterm\command; ValueType: string; ValueData: """{app}\cyglaunch.exe"" -nocd -nols -d \""%L\"""; Flags: uninsdeletekey; Check: not isMinimumOfWin2K; Components: cygterm; Tasks: cygtermhere

[Tasks]
Name: desktopicon; Description: {cm:task_desktopicon}; Components: TeraTerm
Name: quicklaunchicon; Description: {cm:task_quicklaunchicon}; Components: TeraTerm
Name: startupttmenuicon; Description: {cm:task_startupttmenuicon}; Components: TeraTerm_Menu
Name: startupcollectoricon; Description: {cm:task_startupcollectoricon}; Components: Collector
Name: cygtermhere; Description: {cm:task_cygtermhere}; Components: cygterm; Flags: unchecked

[Run]
Filename: {app}\ttermpro.exe; Flags: nowait postinstall skipifsilent unchecked; Description: {cm:launch_teraterm}; Components: TeraTerm
Filename: {app}\LogMeTT.exe; Flags: nowait postinstall skipifsilent unchecked; Description: {cm:launch_logmett}; Components: LogMeTT
Filename: {app}\ttpmenu.exe; Flags: nowait postinstall skipifsilent unchecked; Description: {cm:launch_ttmenu}; Components: TeraTerm_Menu
Filename: {app}\Collector\Collector.exe; Flags: nowait postinstall skipifsilent unchecked; Description: {cm:launch_collector}; Components: Collector

[CustomMessages]
en.task_desktopicon=Create TeraTerm shortcut to &Desktop
en.task_quicklaunchicon=Create TeraTerm shortcut to &Quick Launch
en.task_startupttmenuicon=Create TeraTerm &Menu shortcut to Startup
en.task_startupcollectoricon=Create &Collector shortcut to Startup
en.task_cygtermhere=Add "Cy&gterm Here" to Context menu
ja.task_desktopicon=デスクトップに TeraTerm のショートカットを作る(&D)
ja.task_quicklaunchicon=クイック起動に TeraTerm のショートカットを作る(&Q)
ja.task_startupttmenuicon=スタートアップに TeraTerm &Menu のショートカットを作る
ja.task_startupcollectoricon=スタートアップに &Collector のショートカットを作る
ja.task_cygtermhere=コンテキストメニューに "Cy&gterm Here" を追加する
en.type_standard=Standard installation
en.type_full=Full installation
en.type_compact=Compact installation
en.type_custom=Custom installation
ja.type_standard=標準インストール
ja.type_full=フルインストール
ja.type_compact=コンパクトインストール
ja.type_custom=カスタムインストール
en.launch_teraterm=Launch &TeraTerm Pro
en.launch_logmett=Launch &LogMeTT
en.launch_ttmenu=Launch TeraTerm &Menu
en.launch_collector=Launch &Collector
ja.launch_teraterm=今すぐ &TeraTerm Pro を実行する
ja.launch_logmett=今すぐ &LogMeTT を実行する
ja.launch_ttmenu=今すぐ TeraTerm &Menu を実行する
ja.launch_collector=今すぐ &Collector を実行する
en.msg_language_caption=Select Language
en.msg_language_description=Which language shoud be used?
en.msg_language_subcaption=Select the language of application's menu and dialog, then click Next.
en.msg_language_none=&English
en.msg_language_japanese=&Japanese
en.msg_language_german=&German
ja.msg_language_caption=言語の選択
ja.msg_language_description=ユーザーインターフェースの言語を選択してください。
ja.msg_language_subcaption=アプリケーションのメニューやダイアログ等の表示言語を選択して、「次へ」をクリックしてください。
ja.msg_language_none=英語(&E)
ja.msg_language_japanese=日本語(&J)
ja.msg_language_german=ドイツ語(&G)

[Code]
var
  UILangFilePage: TInputOptionWizardPage;


function isIA64 : Boolean;
begin
  if ProcessorArchitecture = paIA64 then
    Result := True
  else
    Result := False;
end;

function isMinimumOfWin2K : Boolean;
var
  Version: TWindowsVersion;
begin;
  GetWindowsVersionEx(Version);
  if Version.Major >= 5 then
    Result := True
  else
    Result := False;
end;

function isPowerUsersMore : Boolean;
begin;
  if not UsingWinNT() then begin
    Result := True;
  end else begin
    if isMinimumOfWin2K() then begin
      if IsAdminLoggedOn() or IsPowerUserLoggedOn() then begin
        Result := True;
      end else begin
        Result := False
      end;
    end else begin
      Result := True;
    end;
  end;
end;

function GetDefaultIniFilename : String;
begin
  Result := ExpandConstant('{app}') + '\TERATERM.INI';
end;

function isGetDefaultIniExists : Boolean;
var
  iniFile: String;
begin
  iniFile  := GetDefaultIniFilename();
  if FileExists(iniFile) then
    Result := True
  else
    Result := False;
end;

function GetUserIniFilename : String;
begin
  Result := ExpandConstant('{userdocs}') + '\TERATERM.INI';
end;

function isUserIniExists : Boolean;
var
  iniFile: String;
begin
  iniFile  := GetUserIniFilename();
  if FileExists(iniFile) then
    Result := True
  else
    Result := False;
end;

procedure SetIniFile(iniFile: String);
var
  Language : String;
  Locale   : String;
  CodePage : integer;
  VTFont   : String;
  TEKFont  : String;

begin
  Language := GetIniString('Tera Term', 'Language', '', iniFile);
  Locale   := GetIniString('Tera Term', 'Locale', '', iniFile);
  CodePage := GetIniInt('Tera Term', 'CodePage', 0, 0, 0, iniFile);
  VTFont   := GetIniString('Tera Term', 'VTFont', '', iniFile);
  TEKFont  := GetIniString('Tera Term', 'TEKFont', '', iniFile);

  case GetUILanguage and $3FF of
  $04: // Chinese
    begin
      if Length(Language) = 0 then
        SetIniString('Tera Term', 'Language', 'Japanese', iniFile);
      if Length(Locale) = 0 then
        SetIniString('Tera Term', 'Locale', 'chs', iniFile);
      if CodePage = 0 then
        SetIniInt('Tera Term', 'CodePage', 936, iniFile);
      if Length(VTFont) = 0 then
        SetIniString('Tera Term', 'VTFont', 'Terminal,0,-12,255', iniFile);
      if Length(TEKFont) = 0 then
        SetIniString('Tera Term', 'TEKFont', 'Terminal,0,-8,255', iniFile);
    end;
  $11: // Japanese
    begin
      if Length(Language) = 0 then
        SetIniString('Tera Term', 'Language', 'Japanese', iniFile);
      if Length(Locale) = 0 then
        SetIniString('Tera Term', 'Locale', 'japanese', iniFile);
      if CodePage = 0 then
        SetIniInt('Tera Term', 'CodePage', 932, iniFile);
      if Length(VTFont) = 0 then
        SetIniString('Tera Term', 'VTFont', 'ＭＳ 明朝,0,-16,128', iniFile);
      if Length(TEKFont) = 0 then
        SetIniString('Tera Term', 'TEKFont', 'Terminal,0,-8,128', iniFile);
    end;
  $19: // Russian
    begin
      if Length(Language) = 0 then
        SetIniString('Tera Term', 'Language', 'Russian', iniFile);
      if Length(Locale) = 0 then
        SetIniString('Tera Term', 'Locale', 'russian', iniFile);
      if CodePage = 0 then
        SetIniInt('Tera Term', 'CodePage', 1251, iniFile);
      if Length(VTFont) = 0 then
        SetIniString('Tera Term', 'VTFont', 'Terminal,0,-12,255', iniFile);
      if Length(TEKFont) = 0 then
        SetIniString('Tera Term', 'TEKFont', 'Terminal,0,-8,255', iniFile);
    end;
  else // Other
    begin

      if GetUILanguage = $409 then begin // en-US

        if Length(Language) = 0 then
          SetIniString('Tera Term', 'Language', 'Japanese', iniFile);
        if Length(Locale) = 0 then
          SetIniString('Tera Term', 'Locale', 'american', iniFile);
        if CodePage = 0 then
          SetIniInt('Tera Term', 'CodePage', 65001, iniFile);

      end else begin // Other

        if Length(Language) = 0 then
          SetIniString('Tera Term', 'Language', 'English', iniFile);
        if Length(Locale) = 0 then
          SetIniString('Tera Term', 'Locale', 'english', iniFile);
        if CodePage = 0 then
          SetIniInt('Tera Term', 'CodePage', 1252, iniFile);

      end;

      if Length(VTFont) = 0 then
        SetIniString('Tera Term', 'VTFont', 'Terminal,0,-12,255', iniFile);
      if Length(TEKFont) = 0 then
        SetIniString('Tera Term', 'TEKFont', 'Terminal,0,-8,255', iniFile);
    end;
  end;

  case UILangFilePage.SelectedValueIndex of
    1:
      SetIniString('Tera Term', 'UILanguageFile', 'lang\Japanese.lng', iniFile);
    2:
      SetIniString('Tera Term', 'UILanguageFile', 'lang\German.lng', iniFile);
    else
      SetIniString('Tera Term', 'UILanguageFile', '', iniFile);
  end;

end;

procedure InitializeWizard;
var
  UILangFilePageCaption     : String;
  UILangFilePageDescription : String;
  UILangFilePageSubCaption  : String;
  UILangFilePageNone        : String;
  UILangFilePageJapanese    : String;
  UILangFilePageGerman      : String;
begin
  UILangFilePageCaption     := CustomMessage('msg_language_caption');
  UILangFilePageDescription := CustomMessage('msg_language_description');
  UILangFilePageSubCaption  := CustomMessage('msg_language_subcaption');
  UILangFilePageNone        := CustomMessage('msg_language_none');
  UILangFilePageJapanese    := CustomMessage('msg_language_japanese');
  UILangFilePageGerman      := CustomMessage('msg_language_german');

  UILangFilePage := CreateInputOptionPage(wpSelectComponents,
    UILangFilePageCaption, UILangFilePageDescription,
    UILangFilePageSubCaption, True, False);
  UILangFilePage.Add(UILangFilePageNone);
  UILangFilePage.Add(UILangFilePageJapanese);
  UILangFilePage.Add(UILangFilePageGerman);
  case ActiveLanguage of
    'ja':
      UILangFilePage.SelectedValueIndex := 1;
    else
      UILangFilePage.SelectedValueIndex := 0;
  end;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
  iniFile : String;
begin
  case CurPageID of
    wpSelectComponents:
      begin

        if isUserIniExists() then
        begin
          iniFile := GetIniString('Tera Term', 'UILanguageFile', '', GetUserIniFilename());
          if iniFile = 'lang\Japanese.lng' then
            UILangFilePage.SelectedValueIndex := 1
          else if iniFile = 'lang\German.lng' then
            UILangFilePage.SelectedValueIndex := 2
          else
            UILangFilePage.SelectedValueIndex := 0;
        end if isGetDefaultIniExists() then begin
          iniFile := GetIniString('Tera Term', 'UILanguageFile', '', GetDefaultIniFilename());
          if iniFile = 'lang\Japanese.lng' then
            UILangFilePage.SelectedValueIndex := 1
          else if iniFile = 'lang\German.lng' then
            UILangFilePage.SelectedValueIndex := 2
          else
            UILangFilePage.SelectedValueIndex := 0;
        end;

      end;
  end;
  Result := True;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  iniFile : String;
begin
  case CurStep of
    ssDone:
      begin
        iniFile := GetDefaultIniFilename();
        SetIniFile(iniFile);

        if isUserIniExists() then begin
          iniFile := GetUserIniFilename();
          SetIniFile(iniFile);
        end;

        if not IsTaskSelected('cygtermhere') then
        begin;
          RegDeleteKeyIncludingSubkeys(HKEY_CURRENT_USER, 'Software\Classes\Folder\shell\cygterm');
          RegDeleteKeyIncludingSubkeys(HKEY_CLASSES_ROOT, 'Folder\shell\cygterm');
        end;

      end; // ssDone
   end; // case CurStep of
end; // CurStepChanged

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  ini     : array[0..4] of String;
  buf     : String;
  conf    : String;
  confmsg : String;
  app     : String;
  userdoc : String;
  i, j, res : integer;
begin
  case CurUninstallStep of
    usPostUninstall:
      begin
        ini[0] := '\TERATERM.INI';
        ini[1] := '\KEYBOARD.CNF';
        ini[2] := '\ssh_known_hosts';
        ini[3] := '\cygterm.cfg';
        ini[4] := '\broadcast.log';

        case ActiveLanguage of
        'en': conf := 'Are you sure that you want to delete %s ?';
        'ja': conf := '%s を削除しますか？';
        end;

        app     := ExpandConstant('{app}');
        userdoc := ExpandConstant('{userdocs}');

        // delete config files
        for i := 0 to 4 do
        begin
          buf := app + ini[i];
          if FileExists(buf) then begin
            confmsg := Format(conf, [buf]);
            res := MsgBox(confmsg, mbInformation, MB_YESNO or MB_DEFBUTTON2);
            if res = IDYES then
              DeleteFile(buf);
          end;
          buf := userdoc + ini[i];
          if FileExists(buf) then begin
            confmsg := Format(conf, [buf]);
            res := MsgBox(confmsg, mbInformation, MB_YESNO or MB_DEFBUTTON2);
            if res = IDYES then
              DeleteFile(buf);
          end;
        end;

        // delete registory
        if RegKeyExists(HKEY_CURRENT_USER, 'Software\ShinpeiTools\TTermMenu') then begin
          confmsg := Format(conf, ['HKEY_CURRENT_USER' + '\Software\ShinpeiTools\TTermMenu']);
          res := MsgBox(confmsg, mbInformation, MB_YESNO or MB_DEFBUTTON2);
          if res = IDYES then begin
            RegDeleteKeyIncludingSubkeys(HKEY_CURRENT_USER, 'Software\ShinpeiTools\TTermMenu');
            RegDeleteKeyIfEmpty(HKEY_CURRENT_USER, 'Software\ShinpeiTools');
          end;
        end;

        // directory is deleted only if empty
        RemoveDir(app);
      end;
  end;
end;

[InstallDelete]
Name: {app}\OpenSSH-LICENCE.txt; Type: files
Name: {app}\cygterm-README.txt; Type: files
Name: {app}\cygterm-README-j.txt; Type: files
Name: {app}\keycode.txt; Type: files
Name: {app}\keycodej.txt; Type: files
Name: {app}\RE.txt; Type: files
Name: {app}\RE-ja.txt; Type: files
Name: {app}\ssh2_readme.txt; Type: files
Name: {app}\ssh2_readme-j.txt; Type: files
Name: {app}\utf8_readme.txt; Type: files
Name: {app}\utf8_readme-j.txt; Type: files
Name: {app}\OpenSSH-LICENSE.txt; Type: files
Name: {app}\OpenSSL-LICENSE.txt; Type: files
Name: {group}\TeraTerm Document.lnk; Type: files
Name: {group}\TeraTerm Document(Japanese).lnk; Type: files
Name: {group}\TTSSH Document.lnk; Type: files
Name: {group}\TTSSH Document(Japanese).lnk; Type: files
Name: {app}\LogMeTT.hlp; Type: files
Name: {app}\macro.hlp; Type: files
Name: {app}\macroj.hlp; Type: files
Name: {app}\ttermp.hlp; Type: files
Name: {app}\ttermpj.hlp; Type: files
Name: {app}\copyfont.bat; Type: files
Name: {app}\copyfont.pif; Type: files
Name: {app}\libeay.txt; Type: files

[_ISToolPreCompile]
Name: convtext.bat
Name: makechm.bat
Name: build.bat; Parameters: rebuild
