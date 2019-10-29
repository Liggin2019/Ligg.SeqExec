
// includes
#include <windows.h>


//for _ASSERTE
#include <crtdbg.h>
#include <tchar.h>
#include <stdio.h>
#include "lm.h"


#include "CSingleInstance.hxx"  // CSingleInstance implementation
#include "CError.h"             // CError definition
#include "resource.h"           // string defines
#include "SetupCodes.h"         // setup-related error codes
#include "CSettings.h"          // ini-based app globals/setting

#include "main.h"
#include "..\Share\Encrpt\EncryptHelper.h" 
#include "..\Share\CommonDefine.h" 
#include "..\Share\ProcessHelper.h" 
#include "..\Share\StringHelper.h" 


CSettings settings;
DWORD threadNoDw = 0;
HANDLE threadHandle = NULL;
TCHAR windowsDir[512];
TCHAR windowsSystemDir[512];
TCHAR message[255];     
HWND billBoard;
int windowCount = 0;

TCHAR curDlgCaption[64];  
TCHAR curDlgMsg[255];  

ProcessHelper processHelper;
StringHelper stringHelper;

// ==========================================================================
// WinMain(): application entry point
// ==========================================================================

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	UINT    uRetCode = 0;        // bootstrapper return code
	BOOL    isFxReboot = FALSE;  // reboot indicated due to fx install
	BOOL    isAppReboot = FALSE; // reboot indicated after host app install
	BOOL    isAppInstallSucceeded = TRUE;

	//##for testing
	/*if(ip != NULL) 
	{
	MessageBox(NULL, "test11", "Test", MB_OK | MB_ICONINFORMATION);
	}*/


	TCHAR mutexName[MAX_PATH+1];
	GetModuleFileName(settings.GetHInstance(), mutexName, LENGTH(mutexName));
	for(int i = 0;i < LENGTH(mutexName);++i)
	{
		if(mutexName[i]==':')
		{
			mutexName[i]='-';
		}
		else if(mutexName[i]=='\\')
		{
			mutexName[i]='-';
		}
	}

	CSingleInstance si(mutexName);
	// initialize hInstance in global settings
	settings.SetHInstance(hInstance);
	try 
	{
		// validate single instance if we are not alone, throw an error
		if( !si.IsUnique() )
		{
			CError se( 0, ERR_NOT_SINGLE_INSTANCE, MB_ICONERROR, COR_NOT_SINGLE_INSTANCE );
			throw( se );
		}

		// if there was a problem creating mutex, throw an error
		else if( !si.IsHandleOK() )
		{
			CError se(0,ERR_SINGLE_INSTANCE_FAIL,  MB_ICONERROR, COR_SINGLE_INSTANCE_FAIL );
			//throw( se );
			se.ShowMessage();
		}

		if (!GetWindowsDirectory(windowsDir, LENGTH(windowsDir)))
		{
			HandleResult(GetLastError());
		}
		if (!GetSystemDirectory(windowsSystemDir, LENGTH(windowsSystemDir)))
		{
			HandleResult(GetLastError());
		}

		SetWorkingDir();
		RemoveRegistryRun();

		//put ourselves in install mode, if running on Terminal Server.
		SetTSInInstallMode();

		settings.Parse();

		if(_tcsicmp(settings.StartPolicy,"1") ==0)
		{
			//input password
			bool b=DialogBox(hInstance, MAKEINTRESOURCE(DIALOG_PASSWORDVERIFICATION), NULL, ProcDlgPasswordVerification);
			if(!b)
			{
				return 0;
			}
		}

		if(!settings.IsDefaultMode)
		{
			bool ifClickNext=DialogBox(hInstance, MAKEINTRESOURCE(DIALOG_MAIN), NULL, ProcDlgMain);
			if(!ifClickNext) return 0;
		}

		WCHAR domain_w[MAX_INI_DECRPTED_STR_LEN+1];
		swprintf(domain_w,L"%S",settings.Domain);
		WCHAR id_w[MAX_INI_DECRPTED_STR_LEN+1];
		swprintf(id_w,L"%S",settings.Id);
		WCHAR password_w[MAX_INI_DECRPTED_STR_LEN+1];
		swprintf(password_w,L"%S",(settings.Password));

		bool isProcessRunByAdmin=false;
		if(settings.IsProcessRunByAdmin)
		{
			isProcessRunByAdmin=true;
		}
		//=======================================================================
		//** begin to do sequencing Exec
		//=======================================================================
		int i;
		DWORD resultDw;
		int resultInt= 0;
		for(i = 0;i < settings.StepNo;++i)
		{
			char stepNo[3];
			sprintf(stepNo,"%d",i);
			char step[8];
			sprintf(step,"%s%s","Step-",stepNo);
			_sntprintf(settings.CurStepDes, LENGTH(settings.CurStepDes)-1, _T("%s"), step);
			_sntprintf(settings.CurMsgCaptionText, LENGTH(settings.CurMsgCaptionText)-1, _T("%s %s"), settings.CurStepDes,"Error");

			if(!settings.IsQuietMode&&!settings.IsUnselected[i])
			{
				ShowBillboard(&threadNoDw, &threadHandle);	
				TCHAR billBdMsg[MAX_INI_BILLBOARD_MSG_STR_LEN];

				if(!_tcsicmp(settings.BillboardMsg[i],_T("")) ==0)
				{
					_sntprintf(billBdMsg, LENGTH(billBdMsg)-1,"%s",settings.BillboardMsg[i]);
				}
				else
				{
					TCHAR plzWait[15];
					::LoadString(settings.GetHInstance(), 
						MSG_PLZ_WAIT, 
						plzWait, 
						LENGTH(plzWait) ) ;

					_sntprintf(billBdMsg, LENGTH(billBdMsg)-1,"%s..., %s!",settings.StepDes[i],plzWait);
				}

				SetBillBoardText(settings.BillboardTitle,billBdMsg);
			}

			if((_tcsicmp(settings.RunType[i],_T("Run")) == 0)&&(!settings.IsUnselected[i]))
			{
				TCHAR cmdLine[MAX_PATH+MAX_INI_STEP_RUN_OPTION_STR_LEN+MAX_INI_STEP_ARGS_STR_LEN+5+1];
				_sntprintf(cmdLine, LENGTH(cmdLine)-1,"%s %s %s",settings.FilePath[i],settings.RunOption[i],settings.Args[i]);
				if(settings.RunByAdmin[i])
				{
					if(isProcessRunByAdmin)
					{
						resultDw = processHelper.Run(false,cmdLine);
					}
					else
					{
						WCHAR cmdLine_w[MAX_PATH+MAX_INI_STEP_RUN_OPTION_STR_LEN+MAX_INI_STEP_ARGS_STR_LEN+5+1];
						swprintf(cmdLine_w,L"%S",cmdLine);
						resultDw = processHelper.RunAsUser(false,cmdLine_w,domain_w,id_w,password_w);
					}
				}
				else
				{
					resultDw = processHelper.Run(false,cmdLine);
				}
			}

			else if((_tcsicmp(settings.RunType[i],_T("ExecCmd")) == 0)&&(!settings.IsUnselected[i]))
			{
				TCHAR cmdLine[MAX_INI_STEP_RUN_OPTION_STR_LEN+MAX_INI_STEP_ARGS_STR_LEN+1+1];
				char* runOption=settings.RunOption[i];
				if(_tcsicmp(settings.RunOption[i],_T("")) ==0)
				{
					sprintf(runOption,"%s","/c");
				}
				_sntprintf(cmdLine, LENGTH(cmdLine)-1,"%s %s",runOption,settings.Args[i]);

				if(settings.RunByAdmin[i])
				{
					if(isProcessRunByAdmin)
					{
						///resultDw = ExecCmd(cmdLine);
						resultDw =processHelper.Run(true,cmdLine);
					}
					else
					{
						WCHAR cmdLine_w[MAX_INI_STEP_RUN_OPTION_STR_LEN+MAX_INI_STEP_ARGS_STR_LEN+3+1];
						swprintf(cmdLine_w,L"%S",cmdLine);
						resultDw = processHelper.RunAsUser(true,cmdLine_w,domain_w,id_w,password_w);
					}
				}
				else
				{
					resultDw =processHelper.Run(true,cmdLine);
				}
			}

			else if((_tcsicmp(settings.RunType[i],_T("RunCmd")) == 0)&&(!settings.IsUnselected[i]))
			{
				char* runOption=settings.RunOption[i];
				if(_tcsicmp(settings.RunOption[i],_T("")) ==0)
				{
					sprintf(runOption,"%s","/c");
				}
				TCHAR cmdLine[MAX_PATH+MAX_INI_STEP_RUN_OPTION_STR_LEN+MAX_INI_STEP_ARGS_STR_LEN+3+1];
				_sntprintf(cmdLine, LENGTH(cmdLine)-1,"%s %s %s",runOption,settings.AbsoluteFilePath[i],settings.Args[i]);

				if(settings.RunByAdmin[i])
				{
					if(isProcessRunByAdmin)
					{
						resultDw =processHelper.Run(true,cmdLine);
					}
					else
					{
						WCHAR cmdLine_w[MAX_INI_STEP_RUN_OPTION_STR_LEN+MAX_INI_STEP_ARGS_STR_LEN+3+1];
						swprintf(cmdLine_w,L"%S",cmdLine);
						resultDw = processHelper.RunAsUser(true,cmdLine_w,domain_w,id_w,password_w);
					}
				}
				else
				{
					resultDw =processHelper.Run(true,cmdLine);
				}
			}

			else if((_tcsicmp(settings.RunType[i],_T("GetDesEncryptCode")) == 0)&&(!settings.IsUnselected[i]))
			{
				bool b=DialogBox(hInstance, MAKEINTRESOURCE(DIALOG_ENCRPT), NULL, ProcDlgEncrpt);
			}

			else if((_tcsicmp(settings.RunType[i],_T("CheckAdmin")) == 0)&&(!settings.IsUnselected[i]))
			{
				if(!isProcessRunByAdmin)
				{
					CError se(0,ERR_INSUFFICIENT_PRIVILEGES,  MB_ICONERROR, COR_SINGLE_INSTANCE_FAIL );
					throw( se );
				}
			}

			else if((_tcsicmp(settings.RunType[i],_T("InstallMsi")) == 0)&&(!settings.IsUnselected[i]))
			{
				TCHAR msiCmdLine[50] = _T("Msiexec.exe");

				_sntprintf(msiCmdLine, LENGTH(msiCmdLine)-1,"%s %s",msiCmdLine,settings.RunOption[i]);
				TCHAR msiInstallCmd[MAX_PATH + LENGTH(msiCmdLine) + 2];
				_sntprintf(msiInstallCmd, LENGTH(msiInstallCmd)-1,"%s\\%s %s %s",windowsSystemDir,msiCmdLine,settings.FilePath[i],settings.Args[i]);

				if(settings.RunByAdmin[i])
				{
					if(isProcessRunByAdmin)
					{
						resultDw = processHelper.Run(false,msiInstallCmd);
					}
					else
					{
						WCHAR cmdLine_w[MAX_PATH + LENGTH(msiCmdLine) + 2];
						swprintf(cmdLine_w,L"%S",msiInstallCmd);
						resultDw = processHelper.RunAsUser(false,cmdLine_w,domain_w,id_w,password_w);
					}
				}
				else
				{
					resultDw = processHelper.Run(false,msiInstallCmd);
				}

				if ( ERROR_SUCCESS_REBOOT_REQUIRED == resultDw ||ERROR_SUCCESS == resultDw )
				{
					isAppReboot = (resultDw == ERROR_SUCCESS_REBOOT_REQUIRED) ? true : isAppReboot;
				} 
				else if ( resultDw == ERROR_INSTALL_USEREXIT)
				{
					isAppInstallSucceeded = FALSE;
				}
				else 
				{
					// we display the error msg here and do not rethrow this is because we need to continue with a system reboot in the event 
					//that fx was installed successfully before msi-install failure
					CError se( 0,ERR_FILE_HAS_PROBLEM,  MB_ICONERROR, resultDw );
					se.ShowMessage();
					isAppInstallSucceeded = FALSE;
				}
			}

			else if((_tcsicmp(settings.RunType[i],_T("ShowNetFxDownloadUrl")) == 0)&&(!settings.IsUnselected[i]))
			{
				char * runOptionString=settings.RunOption[i];
				char * runOptionStringArry[16];
				int returnCount = 0;
				stringHelper.Split(runOptionString,"^",runOptionStringArry,&returnCount);
				if(returnCount==1)
				{
					runOptionStringArry[1]=new char[1];
					runOptionStringArry[2]=new char[1];
					sprintf(runOptionStringArry[1],"%s","");
					sprintf(runOptionStringArry[2],"%s","");
				}
				else if(returnCount==2)
				{
					runOptionStringArry[2]=new char[1];
					sprintf(runOptionStringArry[2],"%s","");
				}
				if (IfNeedToInstallNetFx(runOptionStringArry[0],runOptionStringArry[1],runOptionStringArry[2]) ==true)	
				{
					_sntprintf(curDlgCaption, LENGTH(curDlgCaption)-1, _T("%s %s %s"), ".Net Framework",runOptionStringArry[0],
						"was needed to install to run this app! ");
					_sntprintf(curDlgMsg, LENGTH(curDlgMsg)-1, _T("%s"), settings.Args[i]);
					bool b=DialogBox(hInstance, MAKEINTRESOURCE(DIALOG_MSG), NULL, ProcDlgMsg);
				}
				/*}*/

			}

			else if((_tcsicmp(settings.RunType[i],_T("InstallNetFx")) == 0)&&(!settings.IsUnselected[i]))
			{

				char * argsString=settings.Args[i];
				char * argsStringArry[16];
				int returnCount = 0;
				stringHelper.Split(argsString,"^",argsStringArry,&returnCount);
				if(returnCount==1)
				{
					argsStringArry[1]=new char[1];
					argsStringArry[2]=new char[1];
					sprintf(argsStringArry[1],"%s","");
					sprintf(argsStringArry[2],"%s","");
				}
				else if(returnCount==2)
				{
					argsStringArry[2]=new char[1];
					sprintf(argsStringArry[2],"%s","");
				}
				if (IfNeedToInstallNetFx(argsStringArry[0],argsStringArry[1],argsStringArry[2]) ==true)	
				{
					TCHAR* fxInstallerPath=settings.AbsoluteFilePath[i];

					// 2 add'l chars for zero-term and space embedded between app name & cmd-line
					TCHAR fxInstallCmd[MAX_PATH + LENGTH(settings.AbsoluteFilePath[i]) + LENGTH(settings.RunOption[i])+ 5];
					if(strstr(settings.RunOption[i], "^") != NULL)
					{
						char * runOptionString=settings.RunOption[i];
						char * runOptionStringArry[16];
						int returnCount = 0;
						stringHelper.Split(runOptionString,"^",runOptionStringArry,&returnCount);
						_sntprintf(fxInstallCmd, LENGTH(fxInstallCmd)-1, 
							_T("%s %s"), 
							_T("/c"),
							runOptionStringArry[1]);
					}
					else
					{
						// build fully-qualified path to dotnetfx.exe
						_sntprintf(fxInstallCmd, 
							LENGTH(fxInstallCmd)-1, 
							_T("%s %s %s"), 
							_T("/c"),
							settings.AbsoluteFilePath[i],
							settings.RunOption[i]);
					}

					if(isProcessRunByAdmin)
					{
						resultDw = processHelper.Run(true,fxInstallCmd);
					}
					else
					{
						WCHAR cmdLine_w[MAX_PATH + LENGTH(settings.AbsoluteFilePath[i]) + LENGTH(settings.RunOption[i])+ 2];
						swprintf(cmdLine_w,L"%S",fxInstallCmd);
						resultDw = processHelper.RunAsUser(true,cmdLine_w,domain_w,id_w,password_w);
					}

					if ( ERROR_SUCCESS_REBOOT_REQUIRED == resultDw ||ERROR_SUCCESS == resultDw )
					{
						isFxReboot = true;
					} 
					else 
					{
						switch(resultDw)
						{
						case 8192: // Reboot
							isFxReboot = true;
							break;

						case 4096: 
							{
								CError se(0, ERR_ERROR1, MB_ICONERROR, resultDw );
								throw ( se );
								break ;
							}
						case 4097: 
							{
								CError se( 0, ERR_ERROR2, MB_ICONERROR, resultDw );
								throw ( se );
								break ;
							}
						case 4098: 
							{
								CError se( 0, ERR_ERROR3, MB_ICONERROR, resultDw );
								throw ( se );
								break ;
							}
						case 4099: 
							{
								CError se( 0, ERR_ERROR4, MB_ICONERROR, resultDw );

								throw ( se );
								break ;
							}
						case 4100: 
							{
								CError se( 0, ERR_ERROR5,MB_ICONERROR,resultDw );
								throw ( se );
								break ;
							}
						case 4101: 
							{
								CError se( 0, ERR_ERROR6, MB_ICONERROR, resultDw );
								throw ( se );
								break ;
							}
						case 4104: 
							{
								CError se(0,  ERR_ERROR7,MB_ICONERROR,resultDw );
								throw ( se );
								break ;
							}

						case 4111: 
							{
								CError se( 0, ERR_ERROR8, MB_ICONERROR, resultDw );

								throw ( se );
								break ;
							}
						case 4113: 
							{
								CError se(0,  ERR_ERROR9, MB_ICONERROR, resultDw );
								throw ( se );
								break ;
							}
						case 4114: 
							{
								CError se( 0, ERR_ERROR10, MB_ICONERROR, resultDw );

								throw ( se );
								break ;
							}
						case 8191: 
							{
								CError se( 0, ERR_ERROR5, MB_ICONERROR, resultDw );
								throw ( se );
								break ;
							}
						default :
							{
								break ;
							}
						}
					}
				}
			} //end else if settings.RunType[i]==_T("InstallNetFx")

			// Are we running in quiet mode?
			if(!settings.IsQuietMode)
			{
				TeardownBillboard(threadNoDw, threadHandle);
			}
		}//end for

		//now handle the reboot
		if (isFxReboot || isAppReboot)
		{
			CError se(MSG_REBOOT_QUERY, 0, MB_YESNO);

			resultInt = se.ShowMessage();

			//if (resultInt == IDYES)
			if (resultInt == 1)
			{
				InsertRegistryRun();
				InitiateReboot();
			}
		}

	}
	catch (HRESULT)
	{
		// hresult exception msg display is handled by the originator. the exception is rethrown and caught here in order to exit.
	}
	catch( CError se )
	{
		uRetCode = se.m_nRetCode;
		se.ShowMessage();
	}
	catch( ... )
	{
		CError se( 0, ERR_FILE_HAS_PROBLEM, MB_ICONERROR, COR_EXIT_FAILURE );
		uRetCode = se.m_nRetCode;
		se.ShowMessage();
	}
	return uRetCode;
}

BOOL Reboot(DWORD resultDw)
{
	if ( ERROR_SUCCESS_REBOOT_REQUIRED == resultDw ||
		ERROR_SUCCESS == resultDw )
	{
		return (resultDw == ERROR_SUCCESS_REBOOT_REQUIRED);
	} 
	return false;
}

BOOL HandleResult(DWORD resultDw)
{
	if ( ERROR_SUCCESS_REBOOT_REQUIRED == resultDw ||
		ERROR_SUCCESS == resultDw )
	{
		return true;
	} 
	else 
	{
		// we display the error msg here and do not rethrow
		// this is because we need to continue with a system
		// reboot in the event that fx was installed 
		// successfully before msi-install failure
		CError se( 0, ERR_FILE_HAS_PROBLEM, MB_ICONERROR, resultDw );
		se.ShowMessage();
		return false;
	}
	return true;
}



// ==========================================================================
// CheckNetFxVersion()
// Purpose: Checks whether the provided Microsoft .Net Framework redistributable files should be installed to the local machine
// ==========================================================================
//old time, no use
BOOL CheckNetFxVersion(LPTSTR fxInstallerPath,char* netVersion)
{

	BOOL ifNeedToInstall = TRUE;
	TCHAR fxInstaller[MAX_PATH + 1];
	_sntprintf(fxInstaller, LENGTH(fxInstaller)-1, _T("%s"), fxInstallerPath);

	try 
	{
		HRESULT hr;
		VS_FIXEDFILEINFO vsf;
		//hr = settings.GetFileVersion (fxInstaller, &vsf);
		if (FAILED(hr))
		{
			//throw hr;
			CError se( 0,ERR_VERSION_DETECT_FAILED,MB_ICONERROR,COR_EXIT_FAILURE,fxInstaller);
			throw( se );
		}


		// retrieve dotnetfx.exe build #
		DWORD dwFileVersionLS = vsf.dwFileVersionLS >> 16;
		// we need a text representation 

		TCHAR subVersion[11]; // sufficient for DWORD max + zero term
		_stprintf(subVersion, _T("%u"), dwFileVersionLS);

		// now we'll check the registry for this value
		LONG regQueryResult;
		HKEY hkey = NULL;
		// Append the version to the key
		TCHAR fxRegKey[MAX_PATH+1];
		sprintf(fxRegKey, "%s%s", FX_REG_KEY, netVersion);

		regQueryResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, // handle to open key
			fxRegKey,      // name of subkey to open
			NULL,
			KEY_READ,
			&hkey            // handle to open key
			);

		// we don't proceed unless the call above succeeds
		if (ERROR_FILE_NOT_FOUND == regQueryResult)
		{
			//MessageBox(NULL, "Couldn't find .Net Key", "Error", MB_OK | MB_ICONINFORMATION);
			return ERROR_FILE_NOT_FOUND;
		}

		if (ERROR_SUCCESS == regQueryResult)
		{
			TCHAR policyStr[256];
			DWORD bufLen = LENGTH(policyStr);

			regQueryResult = RegQueryValueEx( hkey,
				subVersion,
				NULL,
				NULL,
				(LPBYTE)policyStr,
				&bufLen);

			if (ERROR_SUCCESS == regQueryResult)
			{
				// key found, now we need to check for the existence of the appropriate language install dir.  
				strncat(windowsDir, 
					_T("\\Microsoft.Net\\Framework\\"), 
					LENGTH(windowsDir));

				strncat(windowsDir, 
					settings.GetNetVersion(), 
					LENGTH(windowsDir));

				strncat(windowsDir, 
					_T("."), 
					LENGTH(windowsDir));

				strncat(windowsDir, 
					subVersion, 
					LENGTH(windowsDir));

				strncat(windowsDir, 
					_T("\\"), 
					LENGTH(windowsDir));

				strncat(windowsDir, 
					"",//settings.GetLanguageDirectory(), 
					LENGTH(windowsDir));

				DWORD resultDw = GetFileAttributes(windowsDir);
				if (resultDw != INVALID_FILE_ATTRIBUTES && (resultDw & FILE_ATTRIBUTE_DIRECTORY))
				{   
					// we found our subdirectory, no need to install
					ifNeedToInstall = FALSE;
				}
			}

			// if we receive an error other than 0x2, throw
			else if (ERROR_FILE_NOT_FOUND != regQueryResult)
			{
				RegCloseKey(hkey);
				throw HRESULT_FROM_WIN32(regQueryResult);
			}
			else
			{
				MessageBox( NULL, "Could not find the .Net Version Number in the registry", "Error", MB_OK | MB_ICONINFORMATION );
			}
			RegCloseKey(hkey);
		}
	}
	catch( HRESULT hr )
	{
		CError se;
		se.ShowHResultMessage(ERR_VERSION_DETECT_FAILED, 
			0, 
			MB_OK, 
			hr, 
			fxInstaller);
		throw hr;
	}

	return ifNeedToInstall;
}

BOOL IfNeedToInstallNetFx(char* fxSetupNpdKey,char* keyValueName,char* keyValue)
{
	BOOL ifNeedToInstall =false;
	LONG regQueryResult;
	HKEY hkey = NULL;
	// Append the version to the key
	TCHAR fullFxSetupNpdKey[MAX_PATH+1];

	sprintf(fullFxSetupNpdKey, "%s%s%s",FX_INSTALL_REG_KEY,"\\",fxSetupNpdKey);
	regQueryResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, // handle to open key
		fullFxSetupNpdKey,      // name of subkey to open
		NULL,
		KEY_READ,
		&hkey            // handle to open key
		);

	if (ERROR_SUCCESS != regQueryResult)
	{
		RegCloseKey(hkey);
		ifNeedToInstall=true;
		return ifNeedToInstall;
	}

	if(_tcsicmp(keyValueName,_T("")) !=0)
	{
		DWORD dwType; 
		//wchar_t data[MAX_PATH]; 
		TCHAR data[MAX_PATH];
		DWORD bufLen = LENGTH(data);
		DWORD dwSize; 
		regQueryResult = RegQueryValueEx( hkey,
			keyValueName,
			NULL,
			&dwType,
			(LPBYTE)data,
			&bufLen);

		if(ERROR_SUCCESS != regQueryResult)
		{
			RegCloseKey(hkey);
			ifNeedToInstall=true;
			return ifNeedToInstall;
		}

		if(_tcsicmp(keyValue,_T("")) !=0)
		{
			TCHAR data1[MAX_PATH];

			if(dwType==REG_EXPAND_SZ||dwType==REG_SZ||dwType==REG_MULTI_SZ)  
			{  
				//printf("%s/n",data);  
				sprintf(data1, "%s",data);
			}  
			else if(dwType==REG_DWORD||dwType==REG_BINARY)  
			{  
				sprintf(data1, "%d",(long)*(short *)data);
			}  
			if(_tcsicmp(data1, keyValue)!=0) 
			{
				ifNeedToInstall=true;
			}
		}
	}

	RegCloseKey(hkey);
	return ifNeedToInstall;
}

// ==========================================================================
// LastError
// ==========================================================================
HRESULT LastError () 
{
	HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
	if (SUCCEEDED(hr))
	{
		hr = E_FAIL;
	}
	return hr;
}

// ==========================================================================
// InitiateReboot(): initiates a system reboot
// ==========================================================================
BOOL InitiateReboot()
{
#if(_WIN32_WINNT >= 0x0500)
	return ExitWindowsEx( EWX_REBOOT, EWX_FORCEIFHUNG);
#else
	return ExitWindowsEx( EWX_REBOOT, 0);
#endif /* _WIN32_WINNT >= 0x0500 */
}

// ==========================================================================
//TerminalServices
// ==========================================================================

// SetTSInInstallMode(): checks if Terminal Services is enabled and if so switches machine to INSTALL mode
void SetTSInInstallMode()
{
	if (IsTerminalServicesEnabled())
	{
		processHelper.Run(false,TS_CHANGE_USER_TO_INSTALL);
	}
}

//Detecting If Terminal Services is Installed, code is taken directly from   http://msdndevstg/library/psdk/termserv/termserv_7mp0.htm
BOOL IsTerminalServicesEnabled() 
{
	BOOL    bResult = FALSE;
	DWORD   dwVersion;
	OSVERSIONINFOEXA osVersion;
	DWORDLONG dwlCondition = 0;
	HMODULE hmodK32 = NULL;
	HMODULE hmodNtDll = NULL;
	typedef ULONGLONG (WINAPI *PFnVerSetCondition) (ULONGLONG, ULONG, UCHAR);
	typedef BOOL (WINAPI *PFnVerifyVersionA) (POSVERSIONINFOEXA, DWORD, DWORDLONG);
	PFnVerSetCondition pfnVerSetCondition;
	PFnVerifyVersionA pfnVerifyVersionA;
	dwVersion = GetVersion();

	// Are we running Windows NT?
	if (!(dwVersion & 0x80000000)) 
	{
		// Is it Windows 2000 or greater?
		if (LOBYTE(LOWORD(dwVersion)) > 4) 
		{
			// In Windows 2000, use the VerifyVersionInfo and  VerSetConditionMask functions. Don't static link because  it won't load on earlier systems.
			hmodNtDll = GetModuleHandleA( "ntdll.dll" );
			if (hmodNtDll) 
			{
				pfnVerSetCondition = (PFnVerSetCondition) GetProcAddress(hmodNtDll, "VerSetConditionMask");
				if (pfnVerSetCondition != NULL) 
				{
					dwlCondition = (*pfnVerSetCondition) (dwlCondition,VER_SUITENAME, VER_AND);

					// Get a VerifyVersionInfo pointer.
					hmodK32 = GetModuleHandleA( "KERNEL32.DLL" );
					if (hmodK32 != NULL) 
					{
						pfnVerifyVersionA = (PFnVerifyVersionA) GetProcAddress(hmodK32, "VerifyVersionInfoA") ;
						if (pfnVerifyVersionA != NULL) 
						{
							ZeroMemory(&osVersion, sizeof(osVersion));
							osVersion.dwOSVersionInfoSize = sizeof(osVersion);
							osVersion.wSuiteMask = VER_SUITE_TERMINAL;
							bResult = (*pfnVerifyVersionA) (&osVersion,
								VER_SUITENAME, dwlCondition);
						}
					}
				}
			}
		}
		else  // This is Windows NT 4.0 or earlier.
			bResult = ValidateProductSuite( "Terminal Server" );
	}

	return bResult;
}

// ValidateProductSuite() : Terminal Services detection code for systems running Windows NT 4.0 and earlier.
BOOL ValidateProductSuite (LPSTR lpszSuiteToValidate) 
{
	BOOL fValidated = FALSE;
	LONG lResult;
	HKEY hKey = NULL;
	DWORD dwType = 0;
	DWORD dwSize = 0;
	LPSTR lpszProductSuites = NULL;
	LPSTR lpszSuite;

	// Open the ProductOptions key.
	lResult = RegOpenKeyA(
		HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Control\\ProductOptions",
		&hKey
		);
	if (lResult != ERROR_SUCCESS)
		goto exit;

	// Determine required size of ProductSuite buffer.
	lResult = RegQueryValueExA( hKey, "ProductSuite", NULL, &dwType,NULL, &dwSize );
	if (lResult != ERROR_SUCCESS || !dwSize)
		goto exit;

	// Allocate buffer.
	lpszProductSuites = (LPSTR) LocalAlloc( LPTR, dwSize );
	if (!lpszProductSuites)
		goto exit;

	// Retrieve array of product suite strings.
	lResult = RegQueryValueExA( hKey, "ProductSuite", NULL, &dwType,(LPBYTE) lpszProductSuites, &dwSize );
	if (lResult != ERROR_SUCCESS || dwType != REG_MULTI_SZ)
		goto exit;

	// Search for suite name in array of strings.
	lpszSuite = lpszProductSuites;
	while (*lpszSuite) 
	{
		if (lstrcmpA( lpszSuite, lpszSuiteToValidate ) == 0) 
		{
			fValidated = TRUE;
			break;
		}
		lpszSuite += (lstrlenA( lpszSuite ) + 1);
	}

exit:
	if (lpszProductSuites)
		LocalFree( lpszProductSuites );

	if (hKey)
		RegCloseKey( hKey );

	return fValidated;
}

// ==========================================================================
// Billboard 
// ==========================================================================
// BillboardProc() Purpose: Callback proc used to set HWND_TOPMOST on billboard
void ShowBillboard(DWORD * pdwThreadId, HANDLE * phThread)
{
	if (windowCount != 0)
		return;

	HANDLE threadEvent = CreateEvent( 
		NULL,     // no security attributes
		FALSE,    // auto-reset event
		FALSE,     // initial state is not signaled
		NULL);    // object not named


	*phThread = CreateThread(NULL, 
		0L, 
		StaticThreadProc, 
		(LPVOID)&threadEvent, 
		0, 
		pdwThreadId );

	// Wait for any message sent or posted to this queue  or for one of the passed handles be set to signaled.
	// This is important as the window does not get created until the StaticThreadProc gets called
	HANDLE handles[1];
	handles[0] = threadEvent;
	DWORD result = WaitForMultipleObjects(1, handles, FALSE, INFINITE); 
}

void TeardownBillboard(DWORD dwThreadId, HANDLE hThread)
{
	if (windowCount != 1 || hThread == NULL)
		return;
	//    Tell the thread to destroy the modeless dialog
	while (!(PostThreadMessage(dwThreadId, PWM_THREADDESTROYWND, 0, 0 )))
	{
		Sleep(5);
	}
	WaitForSingleObject( hThread, INFINITE );
	CloseHandle( hThread );
	hThread = NULL;
	windowCount = 0;
}


BOOL CALLBACK BillboardProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	switch (message) 
	{ 
	case WM_INITDIALOG: 
		SetWindowPos( hwndDlg, HWND_TOPMOST, 0, 0, 0, 0, /*SWP_NOMOVE |*/ SWP_NOSIZE );
		return TRUE;
	} 
	return FALSE; 
} 

// StaticThreadProc()
// Purpose: Thread proc that creates our billboard dialog
DWORD WINAPI StaticThreadProc( LPVOID lpParameter )
{
	MSG msg;
	HANDLE startEvent = *(HANDLE*)lpParameter;  // thread's read event

	// Create the billboard dialog up front so we can change the text
	billBoard = CreateDialog(settings.GetHInstance(), MAKEINTRESOURCE(DIALOG_BILLBOARD), GetDesktopWindow(), BillboardProc);
	ShowWindow(billBoard, SW_SHOW); 
	windowCount = 1;
	SetEvent(startEvent); // Signal event

	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		if (!::IsDialogMessage( billBoard, &msg ))
		{
			if (msg.message == PWM_THREADDESTROYWND)
			{
				//Tell the dialog to destroy itself
				DestroyWindow(billBoard);

				//Tell our thread to break out of message pump
				PostThreadMessage( threadNoDw, WM_QUIT, 0, 0 );
			}
		}
	} 

	return( 0L );
}


void SetBillBoardText(LPTSTR cap, LPTSTR msg) 
{
	SetWindowText(billBoard,cap);
	SetDlgItemText(billBoard, TXT_BillboardMsg, msg);
}

// ==========================================================================
// DlgProc
// ==========================================================================
INT_PTR CALLBACK ProcDlgMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)  
{
	switch(msg)
	{
	case WM_INITDIALOG: 
		{
			HWND desktop = GetDesktopWindow();
			RECT screenRect, dialogRect;
			GetClientRect(desktop, &screenRect);
			GetClientRect(hwnd, &dialogRect);
			SetWindowPos(hwnd, HWND_TOP, (screenRect.right-screenRect.left)/2 - (dialogRect.right - dialogRect.left)/2,
				(screenRect.bottom-screenRect.top)/2 - (dialogRect.bottom - dialogRect.top)/2,
				dialogRect.right - dialogRect.left, dialogRect.bottom - dialogRect.top, SWP_NOSIZE);

			SetWindowText(hwnd,settings.AppTitle);

			HWND control = GetDlgItem(hwnd, CKBOX_SelectAll);
			CheckDlgButton(hwnd, 9999, BST_CHECKED);
			int i=0;
			for(i = 0;i < settings.StepNo;++i)
			{
				control = GetDlgItem(hwnd, 9900+i);
				ShowWindow(control, true);
				if(!settings.IsUnselected[i])
				{
					CheckDlgButton(hwnd, 9900+i, BST_CHECKED);
				}
				if(settings.IsCompulsory[i])
				{
					CheckDlgButton(hwnd, 9900+i, BST_CHECKED);
					EnableWindow(control, false);
				}
				SetWindowText(control,settings.StepDes[i]);
			}
			BringWindowToTop(hwnd);
		}
		break;

	case WM_COMMAND: 
		switch(LOWORD(wParam))
		{
		case CKBOX_SelectAll:   
			{
				int i=0;
				if (IsDlgButtonChecked(hwnd, 9999) == BST_CHECKED)
				{
					for(i = 0;i < settings.StepNo;++i)
					{
						if(!settings.IsCompulsory[i])
						{
							CheckDlgButton(hwnd, 9900+i, BST_CHECKED);
						}
					}
				}
				else
				{
					for(i = 0;i < settings.StepNo;++i)
					{
						if(!settings.IsCompulsory[i])
						{
							CheckDlgButton(hwnd, 9900+i, BST_UNCHECKED);
						}
					}
				}

				return TRUE; 
			}
		case BTN_Next: 
			{
				int i=0;
				for(i = 0;i < settings.StepNo;++i)
				{
					if (IsDlgButtonChecked(hwnd, 9900+i) == BST_CHECKED)
					{
						settings.IsUnselected[i]=false;
					}
					else
					{
						settings.IsUnselected[i]=TRUE;
					}
				}
				EndDialog(hwnd, TRUE);
				return TRUE; 
			}
		case BTN_Cancel:   
			EndDialog(hwnd, FALSE);
			return FALSE; 
		default: 
			return TRUE; 
		}

	case WM_DESTROY: 
		EndDialog(hwnd, FALSE);
		return FALSE; 

	default: 
		return FALSE; 
	}
	return TRUE; 
}

INT_PTR CALLBACK ProcDlgEncrpt(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)  
{
	switch(msg)
	{
	case WM_INITDIALOG: 
		{
			HWND desktop = GetDesktopWindow();
			RECT screenRect, dialogRect;
			GetClientRect(desktop, &screenRect);
			GetClientRect(hwnd, &dialogRect);
			SetWindowPos(hwnd, HWND_TOP, (screenRect.right-screenRect.left)/2 - (dialogRect.right - dialogRect.left)/2,
				(screenRect.bottom-screenRect.top)/2 - (dialogRect.bottom - dialogRect.top)/2,
				dialogRect.right - dialogRect.left, dialogRect.bottom - dialogRect.top, SWP_NOSIZE);
			BringWindowToTop(hwnd); 
		}
		break;

	case WM_COMMAND: 
		switch(LOWORD(wParam))
		{
		case BTN_Caculate: 
			{
				HWND controlClear = GetDlgItem(hwnd, EDIT_ClearCode);
				HWND controlEncrp = GetDlgItem(hwnd, EDIT_EncrptCode);
				char txtClear[300];
				char txtEncrp[300];
				GetWindowText(controlClear,txtClear,300);
				GetWindowText(controlEncrp,txtEncrp,300);
				if(_tcsicmp(txtClear,"") != 0)
				{
					EncryptHelper eh;
					unsigned char* key= (unsigned char *)DES_KEY;
					char *p=eh.DesEncrypt(key,txtClear);
					SetWindowText(controlEncrp,p);
				}
				else{
					SetWindowText(controlEncrp,"");
					
				}
				break; 
			}

		case BTN_Cancel:   
			EndDialog(hwnd, FALSE);
			break;
		}

	case WM_DESTROY: 
		break;

	default: 
		return FALSE; 
	}
	return TRUE; 
}

INT_PTR CALLBACK ProcDlgPasswordVerification(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)  
{
	switch(msg)
	{
	case WM_INITDIALOG: 
		{
			HWND desktop = GetDesktopWindow();
			RECT screenRect, dialogRect;
			GetClientRect(desktop, &screenRect);
			GetClientRect(hwnd, &dialogRect);
			SetWindowPos(hwnd, HWND_TOP, (screenRect.right-screenRect.left)/2 - (dialogRect.right - dialogRect.left)/2,
				(screenRect.bottom-screenRect.top)/2 - (dialogRect.bottom - dialogRect.top)/2,
				dialogRect.right - dialogRect.left, dialogRect.bottom - dialogRect.top, SWP_NOSIZE);
			BringWindowToTop(hwnd); 
		}
		break;

	case WM_COMMAND: 
		switch(LOWORD(wParam))
		{
		case BTN_Ok: 
			{
				HWND control = GetDlgItem(hwnd, EDIT_Password);
				char a[300];
				GetWindowText(control,a,300);
				if(_tcsicmp(a,settings.StartPassword) == 0)
				{
					EndDialog(hwnd, TRUE);
				}
				else{

				}
				break; 
			}

		case BTN_Cancel:   
			EndDialog(hwnd, FALSE);
			break;
		}

	case WM_DESTROY: 
		break;

	default: 
		return FALSE; 
	}
	return TRUE; 
}

INT_PTR CALLBACK ProcDlgMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)  
{
	switch(msg)
	{
	case WM_INITDIALOG: 
		{
			HWND desktop = GetDesktopWindow();
			RECT screenRect, dialogRect;
			GetClientRect(desktop, &screenRect);
			GetClientRect(hwnd, &dialogRect);
			SetWindowPos(hwnd, HWND_TOP, (screenRect.right-screenRect.left)/2 - (dialogRect.right - dialogRect.left)/2,
				(screenRect.bottom-screenRect.top)/2 - (dialogRect.bottom - dialogRect.top)/2,
				dialogRect.right - dialogRect.left, dialogRect.bottom - dialogRect.top, SWP_NOSIZE);
			BringWindowToTop(hwnd); 
			SetWindowText(hwnd,curDlgCaption);
			SetDlgItemText(hwnd, EDIT_Msg, curDlgMsg);
		}
		break;

	case WM_COMMAND: 
		switch(LOWORD(wParam))
		{


		case BTN_Ok:   
			EndDialog(hwnd, FALSE);
			break;
		}

	case WM_DESTROY: 
		break;

	default: 
		return FALSE; 
	}
	return TRUE; 
}

// ==========================================================================
// Registry operation
// ==========================================================================
void InsertRegistryRun()
{
	try
	{
		HKEY l_HKey;
		LONG l_result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			CURVER_REG_KEY,
			0,
			KEY_WRITE,
			&l_HKey);
		if (l_result != ERROR_SUCCESS)
			return;

		TCHAR l_ExeName[2000];

		GetModuleFileName(NULL,(LPTSTR)l_ExeName,LENGTH(l_ExeName));
		RegSetValueEx(l_HKey, C_DOT_NET_INSTALLER,0,REG_SZ,(LPBYTE)(LPCTSTR)l_ExeName, (DWORD)(LENGTH(l_ExeName)+1) );

		RegCloseKey(l_HKey);
	}
	catch(...)
	{
		_ASSERT(false);
	}
};

void RemoveRegistryRun()
{
	try
	{
		HKEY l_HKey;
		LONG l_result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			CURVER_REG_KEY,
			0,
			KEY_WRITE,
			&l_HKey);
		if (l_result != ERROR_SUCCESS)
			return;

		RegDeleteValue(l_HKey, C_DOT_NET_INSTALLER);

		RegCloseKey(l_HKey);
	}
	catch(...)
	{
		_ASSERT(false);
	}
};

// ==========================================================================
// WorkingDir
// ==========================================================================
BOOL SetWorkingDir()
{
	// Desc: Retrieves the directory of the application.
	//  Pre: None.
	// Post: The return value is the directory of the application ending in
	//'\'.

	TCHAR app_path[2000];
	GetModuleFileName(NULL,(LPTSTR)app_path,LENGTH(app_path));
	LONG len = LENGTH(app_path );
	while ((len > 0) && ('\\' != app_path[ len - 1 ]))
		--len;
	app_path[ len ] = '\0';
	return SetCurrentDirectory((LPCTSTR)app_path);
}


// ==========================================================================
// IsProcessRunByAdmin
// ==========================================================================


