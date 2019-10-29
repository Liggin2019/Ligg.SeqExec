// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
// 
/// ==========================================================================


DWORD	Run(bool isExecCmd,LPCTSTR pszCmd );
DWORD   RunAsUser(bool isExecCmd,LPWSTR pszCmd,LPCWSTR pszDomain ,LPCWSTR pszUserName ,LPCWSTR pszPassword  );

BOOL CheckNetFxVersion(LPTSTR fxInstallerPath,char* netVersion);
BOOL IfNeedToInstallNetFx(char* fxSetupNpdKey,char* keyValueName,char* keyValue);

BOOL    IsTerminalServicesEnabled();
BOOL    ValidateProductSuite (LPSTR lpszSuiteToValidate);
void    SetTSInInstallMode();

void	SetBillBoardText(LPTSTR cap,LPTSTR msg);
void    ShowBillboard(DWORD * pdwThreadId, HANDLE * phThread);
void    TeardownBillboard(DWORD dwThreadId, HANDLE hThread);
DWORD   WINAPI StaticThreadProc( LPVOID lpParameter );
BOOL    CALLBACK BillboardProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK ProcDlgMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ProcDlgEncrpt(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ProcDlgPasswordVerification(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ProcDlgMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);  

void	InsertRegistryRun();
void	RemoveRegistryRun();
BOOL	SetWorkingDir();
//

HRESULT LastError();
BOOL    InitiateReboot();
BOOL    Reboot(DWORD dwResult);
BOOL    HandleResult(DWORD dwResult);

//Private message to tell the thread to destroy the window
const UINT  PWM_THREADDESTROYWND = WM_USER; 
//const TCHAR MUTEX_NAME[] = _T("Sequencing Exec Bootstrapper" );
// reg key for fx policy info used to detect if fx is installed, this key resides in HKEY_LOCAL_MACHINE
const TCHAR FX_REG_KEY[] = _T("SOFTWARE\\Microsoft\\.NETFramework\\policy\\");

const TCHAR FX_INSTALL_REG_KEY[] = _T("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP");

const TCHAR CURVER_REG_KEY[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

const TCHAR   TS_CHANGE_USER_TO_INSTALL[] = _T("change user /INSTALL");

const TCHAR    C_DOT_NET_INSTALLER[] = _T("InstallBootstrap");
