#include <windows.h>
#include <string>


class ProcessHelper
{
public:
	DWORD	ProcessHelper::Run(bool isExecCmd,LPCTSTR pszCmd );
	DWORD   ProcessHelper::RunAsUser(bool isExecCmd,LPWSTR pszCmd,LPCWSTR pszDomain ,LPCWSTR pszUserName ,LPCWSTR pszPassword);
	bool	ProcessHelper::ExecCmd(LPTSTR arg);
};