
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "..\Share\CommonDefine.h"
#include "ProcessHelper.h"


bool ProcessHelper::ExecCmd(LPTSTR cmdStr)
{
	BOOL  isRunOk   = false ;
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL; 
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile ="cmd.exe";
	ShExecInfo.lpParameters =cmdStr;	
	ShExecInfo.lpDirectory = NULL;
	if(strstr(cmdStr, "/k") != NULL)
	{
		ShExecInfo.nShow = SW_SHOW;
	}
	else
	{
		ShExecInfo.nShow = SW_HIDE;
	}
	
	ShExecInfo.hInstApp = NULL;
	isRunOk =ShellExecuteEx(&ShExecInfo);

	if(isRunOk)
	{
		WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
	}
	else
	{
		return false;
	}
	return true;
}

DWORD ProcessHelper::Run(bool isExecCmd, LPCTSTR cmdStr)
{
	BOOL  isRunOk   = false ;
	STARTUPINFO  startupInfo; 
	DWORD  exitCodeDw;
	SECURITY_ATTRIBUTES saProcess, saThread ;
	PROCESS_INFORMATION process_info ;
	ZeroMemory(&startupInfo , sizeof(startupInfo)) ;
	startupInfo.cb = sizeof(startupInfo) ;
	

	saProcess.nLength = sizeof(saProcess) ;
	saProcess.lpSecurityDescriptor = NULL ;
	saProcess.bInheritHandle = TRUE ;

	saThread.nLength = sizeof(saThread) ;
	saThread.lpSecurityDescriptor = NULL ;
	saThread.bInheritHandle = FALSE ;

	if(isExecCmd)
	{
		TCHAR cmdStr1[836];
		_sntprintf(cmdStr1,LENGTH(cmdStr1)-1, _T("%s %s"), _T("cmd.exe"),cmdStr);

		if(strstr(cmdStr, "/k") != NULL)
		{
			startupInfo.wShowWindow=SW_SHOW;
		}
		else
		{
			startupInfo.wShowWindow=SW_HIDE;
		}
		
		isRunOk = CreateProcess(
			NULL, 
			cmdStr1, 
			NULL, 
			NULL, 
			TRUE, 
			NULL, 
			NULL, 
			NULL, 
			&startupInfo, 
			&process_info) ;
	}
	else
	{
		isRunOk = CreateProcess(NULL, 
			(LPTSTR)cmdStr, 
			&saProcess, 
			&saThread, 
			FALSE, 
			DETACHED_PROCESS, 
			NULL, 
			NULL, 
			&startupInfo, 
			&process_info) ;
	}

	if (isRunOk)
	{
		CloseHandle( process_info.hThread ) ;
		WaitForSingleObject( process_info.hProcess, INFINITE ) ;
		GetExitCodeProcess( process_info.hProcess, &exitCodeDw ) ;
		CloseHandle( process_info.hProcess ) ;
	}
	else
	{
		throw(cmdStr);
	}

	return exitCodeDw;
}

DWORD ProcessHelper::RunAsUser(bool isExecCmd,LPWSTR cmdStr,LPCWSTR pszDomain ,LPCWSTR pszUserName ,LPCWSTR pszPassword  )
{
	BOOL  isRunOk   = false ;
	DWORD  exitCodeDw;
	STARTUPINFOW  startupInfo = {0};
	PROCESS_INFORMATION process_info={0};
	startupInfo.cb = sizeof(startupInfo);

	WCHAR cmdExe[10];
	swprintf(cmdExe,L"%S","cmd.exe");
	if(isExecCmd)
	{
		//TCHAR cmdStrSz[LENGTH(cmdStr)];
		WCHAR cmdArgW[3];
	    swprintf(cmdArgW,L"%S","/k");
		if(wcsstr(cmdStr, cmdArgW) != NULL)
		{
			startupInfo.wShowWindow=SW_SHOW;
		}
		else
		{
			startupInfo.wShowWindow= SW_HIDE;
		}

		isRunOk = CreateProcessWithLogonW(pszUserName,
			pszDomain,
			pszPassword, 
			0,
			cmdExe,
			cmdStr,
			CREATE_UNICODE_ENVIRONMENT, 
			NULL, 
			NULL, 
			&startupInfo, 
			&process_info);
	}
	else
	{
		isRunOk = CreateProcessWithLogonW(pszUserName,
			pszDomain,
			pszPassword, 
			0,
			NULL,
			cmdStr,
			CREATE_UNICODE_ENVIRONMENT, 
			NULL, 
			NULL, 
			&startupInfo, 
			&process_info);
	}

	//CREATE_UNICODE_ENVIRONMENT, 
	if(isRunOk)
	{
		CloseHandle( process_info.hThread ) ;
		WaitForSingleObject( process_info.hProcess, INFINITE ) ;
		GetExitCodeProcess( process_info.hProcess, &exitCodeDw ) ;
		CloseHandle( process_info.hProcess ) ;
	}
	else
	{
		throw(cmdStr);
	}
	return exitCodeDw;
}

	









