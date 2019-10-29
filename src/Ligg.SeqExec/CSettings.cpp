
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>


#include "SetupCodes.h"         // setup-related error codes
#include "CError.h"
#include "resource.h"
#include "..\Share\Encrpt\EncryptHelper.h" 
#include "..\Share\StringHelper.h" 
#include "..\Share\CommonDefine.h"
#include "..\Share\FileHelper.h" 
#include "..\Share\SystemHelper.h" 

#include "CSettings.h"

// ==========================================================================
// CSetting::CSetting() Purpose: parameterless CSetting object construction
// ==========================================================================
//CSettings::CSettings(LPCTSTR settingFileFolder)
CSettings::CSettings()
{
	appInst = NULL;
	IsQuietMode = false;
	IsDefaultMode = false;

	AllRunTypes[0] = "GetDesEncryptCode";
	AllRunTypes[1] = "CheckAdmin";
	AllRunTypes[2] = "ShowNetFxDownloadUrl";
	AllRunTypes[3] = "InstallNetFx";
	AllRunTypes[4] = "InstallMsi";
	AllRunTypes[5] = "Run";
	AllRunTypes[6] = "RunCmd";
	AllRunTypes[7] = "ExecCmd";
	_sntprintf(CurMsgCaptionText, LENGTH(settings.CurMsgCaptionText) - 1, _T("%s"), "Initiation Error");



}

void CSettings::Parse()
{
	FileHelper fileHelper;

	TCHAR * strPt = NULL;
	TCHAR instancefileName[MAX_INI_FILE_NAME_STR_LEN + 1];
	TCHAR inifileName[MAX_INI_FILE_NAME_STR_LEN + 1] = "";
	TCHAR *strPt1 = NULL;
	GetModuleFileName(settings.GetHInstance(), CurrentDir, LENGTH(CurrentDir));
	strPt = _tcsrchr(CurrentDir, BACKSLASH);
	if (strPt)
	{
		strPt++; // preserve trailing '\'
		sprintf(instancefileName, "%s", strPt);
		*strPt = END_OF_STRING;
	}
	_sntprintf(settingFilePath, LENGTH(settingFilePath), _T("%s"), CurrentDir);
	_sntprintf(settingFilePath1, LENGTH(settingFilePath1), _T("%s"), CurrentDir);

	strPt1 = _tcsrchr(instancefileName, DOT);
	*strPt1 = END_OF_STRING;
	// strncat:把src所指字符串的前n个字符添加到dest结尾处（覆盖dest结尾处的'\0'）并添加'\0'。src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。
	strncat(inifileName, instancefileName, LENGTH(inifileName));
	strncat(inifileName, _T(".ini"), LENGTH(inifileName));

	strncat(settingFilePath, inifileName, LENGTH(settingFilePath));

	DWORD checkSettingFileResult = fileHelper.CheckFile(settingFilePath);
	if (checkSettingFileResult != 0)
	{
		strncat(settingFilePath1, _T("ini\\"), LENGTH(settingFilePath1));
		strncat(settingFilePath1, inifileName, LENGTH(settingFilePath1));
		checkSettingFileResult = fileHelper.CheckFile(settingFilePath1);
		if (checkSettingFileResult != 0)
		{
			TCHAR bothSettingFilePath[MAX_PATH + 1];;
			_sntprintf(bothSettingFilePath, LENGTH(bothSettingFilePath), _T("%s"), settingFilePath);
			strncat(bothSettingFilePath, _T(" or "), LENGTH(bothSettingFilePath));
			strncat(bothSettingFilePath, settingFilePath1, LENGTH(bothSettingFilePath));
			//if (checkSettingFileResult == ERROR_FILE_NOT_FOUND)
			//{
			//	CError se(0, ERR_FILE_NOT_EXSIT, MB_ICONERROR, ERROR_FILE_NOT_FOUND, bothSettingFilePath);
			//	throw (se);
			//}
			//else
			//{
			//	CError se(0, ERR_FILE_HAS_PROBLEM, MB_ICONERROR, checkSettingFileResult, bothSettingFilePath);
			//	throw (se);
			//}
			CError se(0, ERR_FILE_NOT_EXSIT, MB_ICONERROR, checkSettingFileResult, bothSettingFilePath);
			throw (se);
		}
		else
		{
			_sntprintf(settingFilePath, LENGTH(settingFilePath1), _T("%s"), settingFilePath1);
		}
	}


	_sntprintf(InstancefileName, LENGTH(InstancefileName) - 1, _T("%s"), instancefileName);

	DWORD dwCount = 0;
	int valStrLen = 0;
	char maxValStrLenStr[5];

	TCHAR appTitle[MAX_INI_VALUE_STR_LEN + 1];
	dwCount = GetPrivateProfileString("Main", "AppTitle", NULL, appTitle, LENGTH(appTitle), settingFilePath);
	if (dwCount == 0)
	{
		ThrowError("Main", "AppTitle", NULL, GET_SETTING_ERR_FLAG_NULL);
	}
	//check str length
	valStrLen = strlen(appTitle);
	if ((valStrLen > 0) && (valStrLen == MAX_INI_APP_TITLE_STR_LEN || valStrLen > MAX_INI_APP_TITLE_STR_LEN))
	{
		sprintf(maxValStrLenStr, "%d", MAX_INI_APP_TITLE_STR_LEN);
		ThrowError("Main", "AppTitle", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
	}
	_sntprintf(AppTitle, LENGTH(AppTitle) - 1, _T("%s"), appTitle);

	TCHAR stepNoStr[5];
	dwCount = GetPrivateProfileString("Main", "StepNo", NULL, stepNoStr, LENGTH(stepNoStr), settingFilePath);
	StepNo = 0;
	if (dwCount == 0)
	{
		ThrowError("Main", "StepNo", NULL, GET_SETTING_ERR_FLAG_NULL);
	}
	else
	{
		StepNo = _ttoi(stepNoStr);
	}

	if (StepNo == 0)
	{
		ThrowError("Main", "StepNo", NULL, GET_SETTING_ERR_FLAG_FORMAT);
	}

	if (StepNo > MAX_STEP_NO)
	{
		char maxStepNoStr[5];
		sprintf(maxStepNoStr, "%d", MAX_STEP_NO);
		CError se(0, ERR_STEP_NO_CANOT_GREATER_THAN, MB_ICONERROR, FALSE, maxStepNoStr);
		throw (se);
	}

	TCHAR isDefaultModeStr[MAX_BOOL_STR_LEN + 1];
	dwCount = GetPrivateProfileString("Main", "IsDefaultMode", NULL, isDefaultModeStr, LENGTH(isDefaultModeStr), settingFilePath);
	if (dwCount == 0)
	{
		ThrowError("Main", "IsDefaultMode", NULL, GET_SETTING_ERR_FLAG_NULL);
	}
	else if (_tcsicmp(isDefaultModeStr, _T("true")) == 0)
	{
		IsDefaultMode = true;
	}
	else if (_tcsicmp(isDefaultModeStr, _T("false")) != 0)//_tcscmp
	{
		ThrowError("Main", "IsDefaultMode", NULL, GET_SETTING_ERR_FLAG_FORMAT);
	}

	TCHAR isQuietModeStr[MAX_BOOL_STR_LEN + 1];
	dwCount = GetPrivateProfileString("Main", "IsQuietMode", NULL, isQuietModeStr, LENGTH(isQuietModeStr), settingFilePath);
	if (dwCount == 0)
	{
		ThrowError("Main", "IsQuietMode", NULL, GET_SETTING_ERR_FLAG_NULL);
	}
	else if (_tcsicmp(isQuietModeStr, _T("true")) == 0)//_tcscmp
	{
		IsQuietMode = true;
	}
	else if (_tcsicmp(isQuietModeStr, _T("false")) != 0)//_tcscmp
	{
		ThrowError("Main", "IsQuietMode", NULL, GET_SETTING_ERR_FLAG_FORMAT);
	}

	TCHAR billboardTitle[MAX_INI_VALUE_STR_LEN + 1];
	_sntprintf(BillboardTitle, LENGTH(BillboardTitle) - 1, _T("%s"), _T("NoNeed"));
	if (!IsQuietMode)
	{
		dwCount = GetPrivateProfileString("Main", "BillboardTitle", NULL, billboardTitle, LENGTH(billboardTitle), settingFilePath);
		if (dwCount == 0)
		{
			_sntprintf(billboardTitle, LENGTH(billboardTitle) - 1, _T("%s"), appTitle);
		}
		//check str length
		valStrLen = strlen(billboardTitle);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_BILLBOARD_TITLE_STR_LEN || valStrLen > MAX_INI_BILLBOARD_TITLE_STR_LEN))
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_BILLBOARD_TITLE_STR_LEN);
			ThrowError("Main", "BillboardTitle", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		_sntprintf(BillboardTitle, LENGTH(BillboardTitle) - 1, _T("%s"), billboardTitle);
	}

	TCHAR startPolicy_encrpted[MAX_INI_ENCRPTED_STR_LEN + 1];
	TCHAR startPassword_encrpted[MAX_INI_ENCRPTED_STR_LEN + 1];
	TCHAR domainName_encrpted[MAX_INI_ENCRPTED_STR_LEN + 1];
	TCHAR id_encrpted[MAX_INI_ENCRPTED_STR_LEN + 1];
	TCHAR password_encrpted[MAX_INI_ENCRPTED_STR_LEN + 1];
	unsigned char* key = (unsigned char *)DES_KEY;
	StringHelper strHelper;
	EncryptHelper encryptHelper;

	StartPolicy = _T("");
	dwCount = GetPrivateProfileString("Main", "StartPolicy", NULL, startPolicy_encrpted, LENGTH(startPolicy_encrpted), settingFilePath);
	if (dwCount != 0)
	{
		//check str length
		valStrLen = strlen(startPolicy_encrpted);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_ENCRPTED_STR_LEN || valStrLen > MAX_INI_ENCRPTED_STR_LEN))
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_ENCRPTED_STR_LEN);
			ThrowError("Main", "StartPolicy", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		char *startPolicyPStr = NULL;
		startPolicyPStr = encryptHelper.DesDecrypt(key, startPolicy_encrpted);
		if (!strHelper.IsAscilString(startPolicyPStr))
		{
			//ThrowError("Main", "StartPolicy", NULL, GET_SETTING_ERR_FLAG_FORMAT);
			ThrowError("Main", "StartPolicy1", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}

		if (strlen(startPolicyPStr) > MAX_INI_DECRPTED_STR_LEN)
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_DECRPTED_STR_LEN);
			ThrowError("Main", "Decrpted StartPolicy", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		StartPolicy = _T(startPolicyPStr);
	}
	else
	{
		ThrowError("Main", "StartPolicy", NULL, GET_SETTING_ERR_FLAG_NULL);
	}
	if (strstr(strHelper.ToLower(StartPolicy), strHelper.ToLower(InstancefileName)) == NULL)
	{
		ThrowError("Main", "StartPolicy", NULL, GET_SETTING_ERR_FLAG_FORMAT);
	}
	if (strstr(StartPolicy, "^") == NULL)
	{
		ThrowError("Main", "StartPolicy", NULL, GET_SETTING_ERR_FLAG_FORMAT);
	}
	char * startPolicyStringArray[16];
	int getCount = 0;
	strHelper.Split(StartPolicy, "^", startPolicyStringArray, &getCount);
	int lastPosIndex = strlen(startPolicyStringArray[1]) - 1;
	if (startPolicyStringArray[1][lastPosIndex] != '0')
	{
		StartPassword = _T("");
		dwCount = GetPrivateProfileString("Main", "StartPassword", NULL, startPassword_encrpted, LENGTH(startPassword_encrpted), settingFilePath);
		if (dwCount != 0)
		{
			//check str length
			valStrLen = strlen(startPassword_encrpted);
			if ((valStrLen > 0) && (valStrLen == MAX_INI_ENCRPTED_STR_LEN || valStrLen > MAX_INI_ENCRPTED_STR_LEN))
			{
				sprintf(maxValStrLenStr, "%d", MAX_INI_ENCRPTED_STR_LEN);
				ThrowError("Main", "StartPassword", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
			}
			char *startPasswordPStr = NULL;
			startPasswordPStr = encryptHelper.DesDecrypt(key, startPassword_encrpted);
			if (!strHelper.IsAscilString(startPasswordPStr))
			{
				ThrowError("Main", "StartPassword", NULL, GET_SETTING_ERR_FLAG_FORMAT);
			}

			if (strlen(startPasswordPStr) > MAX_INI_DECRPTED_STR_LEN)
			{
				sprintf(maxValStrLenStr, "%d", MAX_INI_DECRPTED_STR_LEN);
				ThrowError("Main", "Decrpted StartPassword", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
			}
			StartPassword = _T(startPasswordPStr);
		}
		else
		{
			ThrowError("Main", "StartPassword", NULL, GET_SETTING_ERR_FLAG_NULL);
		}

		sprintf(StartPolicy, "%d", 1);

	}

	Domain = _T("");
	dwCount = GetPrivateProfileString("RunAsAdmin", "Domain", NULL, domainName_encrpted, LENGTH(domainName_encrpted), settingFilePath);
	if (dwCount != 0)
	{
		//check str length
		valStrLen = strlen(domainName_encrpted);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_ENCRPTED_STR_LEN || valStrLen > MAX_INI_ENCRPTED_STR_LEN))
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_ENCRPTED_STR_LEN);
			ThrowError("RunAsAdmin", "Domain", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		char *domainPStr = NULL;
		domainPStr = encryptHelper.DesDecrypt(key, domainName_encrpted);
		if (!strHelper.IsAscilString(domainPStr))
		{
			ThrowError("RunAsAdmin", "Domain", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}

		if (strlen(domainPStr) > MAX_INI_DECRPTED_STR_LEN)
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_DECRPTED_STR_LEN);
			ThrowError("RunAsAdmin", "Decrpted Domain", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		Domain = _T(domainPStr);
	}

	Id = _T("");
	dwCount = GetPrivateProfileString("RunAsAdmin", "Id", NULL, id_encrpted, LENGTH(id_encrpted), settingFilePath);
	if (dwCount != 0)
	{
		//check str length
		valStrLen = strlen(id_encrpted);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_ENCRPTED_STR_LEN || valStrLen > MAX_INI_ENCRPTED_STR_LEN))
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_ENCRPTED_STR_LEN);
			ThrowError("RunAsAdmin", "Id", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		char *idPStr = NULL;
		idPStr = encryptHelper.DesDecrypt(key, id_encrpted);
		if (!strHelper.IsAscilString(idPStr))
		{
			ThrowError("RunAsAdmin", "Id", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}
		if (strlen(idPStr) > MAX_INI_DECRPTED_STR_LEN)
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_DECRPTED_STR_LEN);
			ThrowError("RunAsAdmin", "Decrpted Id", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		Id = _T(idPStr);
	}

	Password = _T("");
	dwCount = GetPrivateProfileString("RunAsAdmin", "Password", NULL, password_encrpted, LENGTH(password_encrpted), settingFilePath);
	if (dwCount != 0)
	{
		//check str length
		valStrLen = strlen(password_encrpted);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_ENCRPTED_STR_LEN || valStrLen > MAX_INI_ENCRPTED_STR_LEN))
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_ENCRPTED_STR_LEN);
			ThrowError("RunAsAdmin", "Password", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}

		char *pwStrPt = NULL;
		pwStrPt = encryptHelper.DesDecrypt(key, password_encrpted);
		if (!strHelper.IsAscilString(pwStrPt))
		{
			ThrowError("RunAsAdmin", "Password", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}

		if (strlen(pwStrPt) > MAX_INI_DECRPTED_STR_LEN)
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_DECRPTED_STR_LEN);
			ThrowError("RunAsAdmin", "Decrpted Password", maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		Password = _T(pwStrPt);
	}

	//int i;
	char step[6] = "Step-";
	char *isCompulsoryStr[MAX_STEP_NO];
	char *isUnselectedStr[MAX_STEP_NO];
	char *ifRunByAdminStr[MAX_STEP_NO];
	//for(int i = 0 ; i <MAX_STEP_NO ; i++ )
	for (int i = 0; i < settings.StepNo; i++)
	{
		StepDes[i] = new TCHAR[MAX_INI_STEP_DES_STR_LEN + 1];
		BillboardMsg[i] = new TCHAR[MAX_INI_BILLBOARD_MSG_STR_LEN + 1];
		FilePath[i] = new TCHAR[MAX_PATH + 1];
		AbsoluteFilePath[i] = new TCHAR[MAX_PATH + 1];
		IsCompulsory[i] = false;
		isCompulsoryStr[i] = new char[MAX_BOOL_STR_LEN + 1];
		IsUnselected[i] = false;
		isUnselectedStr[i] = new char[MAX_BOOL_STR_LEN + 1];
		RunByAdmin[i] = false;
		ifRunByAdminStr[i] = new char[MAX_BOOL_STR_LEN + 1];
		RunType[i] = new char[MAX_INI_STEP_RUN_TYPE_STR_LEN + 1];
		RunOption[i] = new char[MAX_INI_STEP_RUN_OPTION_STR_LEN + 1];
		Args[i] = new TCHAR[MAX_INI_STEP_ARGS_STR_LEN + 1];
	}

	//get all params from ini
	TCHAR* iniVal = NULL;
	bool ifAtLeastOneStepRunByAdmin = false;
	bool ifRunTypeIncludeInstallNetFx = false;
	for (int i = 0;i < settings.StepNo;++i)
	{
		char stepNo[3];
		sprintf_s(stepNo, "%d", i);
		char step[8];
		sprintf_s(step, "%s%s", "Step-", stepNo);

		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("StepDes"), 1); //has problem in vs2017
		//sprintf(StepDes[i], "s", iniVal); //above sentence has problem,cause StepDes[i] get a Garbled character
		//sprintf_s(StepDes[i], MAX_BOOL_STR_LEN + 1, iniVal);//above sentence has problem,cause StepDes[i] get a null
		GetPrivateProfileString(_T(step), _T("StepDes"), NULL, StepDes[i], MAX_INI_VALUE_STR_LEN + 1, settingFilePath);

		//check str length
		valStrLen = strlen(StepDes[i]);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_STEP_DES_STR_LEN || valStrLen > MAX_INI_STEP_DES_STR_LEN))
		{
			sprintf_s(maxValStrLenStr, "%d", MAX_INI_STEP_DES_STR_LEN);
			ThrowError(_T(step), _T("StepDes"), maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}

		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("BillboardMsg"), 0);
		//sprintf(BillboardMsg[i], "s", iniVal);
		GetPrivateProfileString(_T(step), _T("BillboardMsg"), NULL, BillboardMsg[i], MAX_INI_BILLBOARD_MSG_STR_LEN + 1, settingFilePath);


		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("IsCompulsory"), 0);
		//sprintf_s(isCompulsoryStr[i], MAX_BOOL_STR_LEN + 1, iniVal);
		GetPrivateProfileString(_T(step), _T("IsCompulsory"), NULL, isCompulsoryStr[i], MAX_BOOL_STR_LEN + 1, settingFilePath);
		IsCompulsory[i] = false;
		if (_tcsicmp(isCompulsoryStr[i], _T("true")) == 0)
		{
			IsCompulsory[i] = true;
		}
		else if (_tcsicmp(isCompulsoryStr[i], _T("false")) != 0 && _tcsicmp(isCompulsoryStr[i], _T("")) != 0)
		{
			ThrowError(_T(step), "IsCompulsory", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}

		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("IsUnselected"), 0);
		//sprintf_s(isUnselectedStr[i], MAX_BOOL_STR_LEN + 1, iniVal);
		GetPrivateProfileString(_T(step), _T("IsUnselected"), NULL, isUnselectedStr[i], MAX_BOOL_STR_LEN + 1, settingFilePath);
		IsUnselected[i] = false;
		if (_tcsicmp(isUnselectedStr[i], _T("true")) == 0)
		{
			IsUnselected[i] = true;
		}
		else if (_tcsicmp(isUnselectedStr[i], _T("false")) != 0 && _tcsicmp(isUnselectedStr[i], _T("")) != 0)
		{
			ThrowError(_T(step), "IsUnselected", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}

		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("RunByAdmin"), 0);
		//sprintf_s(ifRunByAdminStr[i], MAX_BOOL_STR_LEN + 1, iniVal);
		GetPrivateProfileString(_T(step), _T("RunByAdmin"), NULL, ifRunByAdminStr[i], MAX_BOOL_STR_LEN + 1, settingFilePath);
		RunByAdmin[i] = false;
		if (_tcsicmp(ifRunByAdminStr[i], _T("true")) == 0)
		{
			RunByAdmin[i] = true;
			ifAtLeastOneStepRunByAdmin = true;
		}
		else if (_tcsicmp(ifRunByAdminStr[i], _T("false")) != 0 && _tcsicmp(ifRunByAdminStr[i], _T("")) != 0)
		{
			ThrowError(_T(step), "RunByAdmin", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}

		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("RunType"), 1);
		//sprintf_s(RunType[i], MAX_INI_STEP_RUN_TYPE_STR_LEN + 1, iniVal);
		GetPrivateProfileString(_T(step), _T("RunType"), NULL, RunType[i], MAX_INI_STEP_RUN_TYPE_STR_LEN + 1, settingFilePath);
		if (_tcsicmp(RunType[i], _T("InstallNetFx")) == 0)
		{
			ifRunTypeIncludeInstallNetFx = true;
		}

		bool isRunTypeSpellingOk = false;
		for (int i1 = 0; i1 < LENGTH(AllRunTypes); i1++)
		{
			if (_tcsicmp(RunType[i], AllRunTypes[i1]) == 0)
			{
				isRunTypeSpellingOk = true;
			}
		}
		if (!isRunTypeSpellingOk)
		{
			ThrowError(_T(step), "RunType", NULL, GET_SETTING_ERR_FLAG_FORMAT);
		}

		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("FilePath"), 0);
		//sprintf_s(FilePath[i], MAX_PATH + 1, iniVal);
		GetPrivateProfileString(_T(step), _T("FilePath"), NULL, FilePath[i], MAX_PATH + 1, settingFilePath);
		char runTypeToLower[MAX_INI_VALUE_STR_LEN];
		strcpy_s(runTypeToLower, RunType[i]);
		for (int i2 = 0; i2 <= strlen(runTypeToLower); i2++)
		{
			runTypeToLower[i2] = tolower(runTypeToLower[i2]);
		}
		if (strstr(runTypeToLower, "run") != NULL || strstr(runTypeToLower, "install") != NULL)
		{
			if (_tcsicmp(FilePath[i], _T("")) == 0) ThrowError(_T(step), "FilePath", NULL, GET_SETTING_ERR_FLAG_DEFAUT);
			TCHAR absolutePath[MAX_PATH + 1] = "";
			if (strstr(FilePath[i], ":") == NULL && strstr(FilePath[i], "\\\\") == NULL)
			{
				sprintf_s(absolutePath, "%s%s", CurrentDir, FilePath[i]);
			}
			else
			{
				sprintf_s(absolutePath, "%s", FilePath[i]);
			}
			sprintf_s(AbsoluteFilePath[i], MAX_PATH + 1, absolutePath);
			DWORD checkFilePathResult = fileHelper.CheckFile(absolutePath);
			if (checkFilePathResult != 0)
			{
				if (checkFilePathResult == ERROR_FILE_NOT_FOUND)
				{
					CError se(0, ERR_FILE_NOT_EXSIT, MB_ICONERROR, ERROR_FILE_NOT_FOUND, FilePath[i]);
					throw (se);
				}
				else
				{
					CError se(0, ERR_FILE_HAS_PROBLEM, MB_ICONERROR, checkFilePathResult, FilePath[i]);
					throw (se);
				}
			}
		}
		//check str length
		valStrLen = strlen(FilePath[i]);
		if ((valStrLen > 0) && (valStrLen == MAX_PATH || valStrLen > MAX_PATH))
		{
			sprintf_s(maxValStrLenStr, "%d", MAX_PATH);
			ThrowError(_T(step), _T("FilePath"), maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}

		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("RunOption"), 0);
		//sprintf_s(RunOption[i], MAX_INI_STEP_RUN_OPTION_STR_LEN + 1, iniVal);
		GetPrivateProfileString(_T(step), _T("RunOption"), NULL, RunOption[i], MAX_INI_STEP_RUN_OPTION_STR_LEN + 1, settingFilePath);
		//check str length
		valStrLen = strlen(RunOption[i]);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_STEP_RUN_OPTION_STR_LEN || valStrLen > MAX_INI_STEP_RUN_OPTION_STR_LEN))
		{
			sprintf_s(maxValStrLenStr, "%d", MAX_INI_STEP_RUN_OPTION_STR_LEN);
			ThrowError(_T(step), _T("RunOption"), maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		if (strstr(runTypeToLower, "shownetfxdownloadurl") != NULL)
		{
			if (_tcsicmp(RunOption[i], _T("")) == 0) ThrowError(_T(step), "RunOption", NULL, GET_SETTING_ERR_FLAG_DEFAUT);
		}

		//get Args
		//iniVal = settings.GetPrivateProfileValue(_T(step), _T("Args"), 0);
		//sprintf_s(Args[i], MAX_INI_STEP_ARGS_STR_LEN + 1, iniVal);
		GetPrivateProfileString(_T(step), _T("Args"), NULL, Args[i], MAX_INI_STEP_ARGS_STR_LEN + 1, settingFilePath);
		//check str length
		valStrLen = strlen(Args[i]);
		if ((valStrLen > 0) && (valStrLen == MAX_INI_STEP_ARGS_STR_LEN || valStrLen > MAX_INI_STEP_ARGS_STR_LEN))
		{
			sprintf(maxValStrLenStr, "%d", MAX_INI_STEP_ARGS_STR_LEN);
			ThrowError(_T(step), _T("Args"), maxValStrLenStr, GET_SETTING_ERR_FLAG_LENGTH);
		}
		if (strstr(runTypeToLower, "installnetfx") != NULL)
		{
			if (_tcsicmp(Args[i], _T("")) == 0) ThrowError(_T(step), "Args", NULL, GET_SETTING_ERR_FLAG_DEFAUT);
		}

		//get dotNetVersion and check if file NetFxInstaller exists
		if (_tcsicmp(RunType[i], _T("InstallNetFx")) == 0)
		{
			//for old time 'CheckNetFxVersion'
			//sprintf(dotNetVersion,"%s",Args[i]);

			//HRESULT hr;
			//VS_FIXEDFILEINFO vsf;

			//hr = GetFileVersion(FilePath[i], &vsf);
			//if (FAILED(hr))
			//{
			//	//throw hr;
			//	CError se( 0,ERR_VERSION_DETECT_FAILED,MB_ICONERROR,COR_EXIT_FAILURE,FilePath[i]);
			//	throw( se );
			//}
		}

	} //for end

	//check RunAsAdmin
	IsProcessRunByAdmin = false;
	SystemHelper sysHelper;
	try
	{
		//if (sysHelper.IsProcessRunByAdmin())//invalid in Vs2017
		if (sysHelper.IsCurrentUserAdmin())//invalid in Vs2017
		{
			IsProcessRunByAdmin = true;
		}
	}
	catch (...)
	{
		IsProcessRunByAdmin = false;
	}

	if (ifAtLeastOneStepRunByAdmin || ifRunTypeIncludeInstallNetFx)
	{
		if (!IsProcessRunByAdmin)
		{
			if (_tcsicmp(Id, _T("")) != 0)
			{
				HANDLE hUser;
				bool b = LogonUser(Id, Domain, Password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hUser);
				if (!b)
				{
					CError se(0, ERR_INI_FILE_VALUE_INCORRECT, MB_ICONERROR, FALSE, "RunAsAdmin/Domain or Id or Password is not correct! ");
					throw (se);
				}
			}
			else
			{
				CError se(0, ERR_INSUFFICIENT_PRIVILEGES, MB_ICONERROR, COR_SINGLE_INSTANCE_FAIL);
				throw(se);
			}
		}
		else
		{
		}
	}
}

TCHAR * CSettings::GetPrivateProfileValue(LPCSTR section, LPCSTR key, bool ifPopupError)
{
	DWORD dwCount = 0;
	TCHAR iniVal[MAX_INI_VALUE_STR_LEN + 1];
	dwCount = GetPrivateProfileString(section,
		key,
		NULL,
		iniVal,
		MAX_INI_VALUE_STR_LEN + 1,
		settingFilePath);
	if (dwCount == 0)
	{
		if (ifPopupError)
		{
			ThrowError(section, key, NULL, GET_SETTING_ERR_FLAG_NULL);
		}
		else
		{
			return "";
		}
	}
	else
	{
		return iniVal;
	}
}

void * CSettings::ThrowError(LPCSTR section, LPCSTR key, LPCSTR pram1, WORD flag)
{
	TCHAR secAndKey[2 * MAX_INI_KEY_STR_LEN + 1 + 1 + 1];
	_sntprintf(secAndKey, LENGTH(secAndKey) - 1, _T("%s/%s"), section, key);
	TCHAR errDetail[MAX_MSG_STR_LEN + 1];
	if (flag == GET_SETTING_ERR_FLAG_NULL)
	{
		_sntprintf(errDetail, LENGTH(errDetail) - 1, _T("%s does not exsit"), secAndKey);
		CError se(0, ERR_INI_FILE_VALUE_INCORRECT, MB_ICONERROR, FALSE, errDetail);
		throw (se);
	}
	else  if (flag == GET_SETTING_ERR_FLAG_FORMAT)
	{
		if (_tcsicmp(key, _T("StepNo")) == 0)
		{
			_sntprintf(errDetail, LENGTH(errDetail) - 1, _T("%s %s"), secAndKey, _T("must be a number"));
		}
		else if (_tcsicmp(key, _T("IsDefaultMode")) == 0 || _tcsicmp(key, _T("IsQuietMode")) == 0 || _tcsicmp(key, _T("IsCompulsory")) == 0 || _tcsicmp(key, _T("RunByAdmin")) == 0)
		{
			_sntprintf(errDetail, LENGTH(errDetail) - 1, _T("%s %s"), secAndKey, _T("must be only 'true' or 'false'"));
		}
		else if (_tcsicmp(key, _T("RunType")) == 0)
		{
			_sntprintf(errDetail, LENGTH(errDetail) - 1, _T("%s %s"), secAndKey, _T("must be a string same as defined in advance"));
		}
		else
		{
			_sntprintf(errDetail, LENGTH(errDetail) - 1, _T("%s %s"), secAndKey, _T("format is incorrect"));
		}

		CError se(0, ERR_INI_FILE_VALUE_INCORRECT, MB_ICONERROR, FALSE, errDetail);
		throw (se);
	}
	else if (flag == GET_SETTING_ERR_FLAG_LENGTH)
	{
		_sntprintf(errDetail, LENGTH(errDetail) - 1, _T("%s string length can't be greater than %s"), secAndKey, pram1);
		CError se(0, ERR_INI_FILE_VALUE_INCORRECT, MB_ICONERROR, FALSE, errDetail);
		throw (se);
	}

	else
	{
		CError se(0, ERR_READ_INI_FILE_ERROR, MB_ICONERROR, FALSE, secAndKey);
		throw (se);
	}
}

//GetFileVersion()retrieves a file version info structure for the specified file
//HRESULT CSettings::GetFileVersion (LPTSTR filename, VS_FIXEDFILEINFO *pvsf) 
//{
//	DWORD dwHandle;
//	HRESULT hrReturn = S_OK;
//	char* pver = NULL;
//	try 
//	{
//		DWORD cchver = GetFileVersionInfoSize(filename,&dwHandle);
//		if (cchver == 0)
//		{
//			throw HRESULT_FROM_WIN32(GetLastError());
//		}
//		pver = new char[cchver];
//
//		if (!pver)
//		{
//			throw E_OUTOFMEMORY;
//		}
//
//		BOOL bret = GetFileVersionInfo(filename,dwHandle,cchver,pver);
//		if (!bret) 
//		{
//			throw HRESULT_FROM_WIN32(GetLastError());
//		}
//		UINT uLen;
//		void *pbuf;
//		bret = VerQueryValue(pver,_T("\\"),&pbuf,&uLen);
//		if (!bret) 
//		{
//			throw HRESULT_FROM_WIN32(GetLastError());
//		}
//		memcpy(pvsf,pbuf,sizeof(VS_FIXEDFILEINFO));
//	}
//	catch (HRESULT hr)
//	{
//		hrReturn = hr;
//	}
//
//	delete[] pver;
//	return hrReturn;
//}