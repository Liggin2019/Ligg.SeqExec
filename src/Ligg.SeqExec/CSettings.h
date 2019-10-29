
#ifndef SETTINGS_H
#define SETTINGS_H

#include <tchar.h>


//defines
#define EMPTY_BUFFER	{ _T( '\0' ) }
#define END_OF_STRING	_T( '\0' )
#define BACKSLASH       _T( '\\' )
#define DOT				_T( '.' )

#define MAX_MSG_STR_LEN 4096
#define MAX_BOOL_STR_LEN 5

#define MAX_STEP_NO 30
#define RUN_TYPE_NO 8

#define MAX_INI_VALUE_STR_LEN 512

#define MAX_INI_APP_TITLE_STR_LEN 128
#define MAX_INI_BILLBOARD_TITLE_STR_LEN 128
#define MAX_INI_BILLBOARD_MSG_STR_LEN 512
#define MAX_INI_KEY_STR_LEN 32
#define MAX_INI_FILE_NAME_STR_LEN 32

#define MAX_INI_ERROR_CAPTION_STR_LEN 128
#define MAX_INI_ENCRPTED_STR_LEN 128
#define MAX_INI_DECRPTED_STR_LEN 128

#define MAX_INI_STEP_MSG_STR_LEN 512
#define MAX_INI_STEP_DES_STR_LEN 128
#define MAX_INI_STEP_RUN_TYPE_STR_LEN 32
#define MAX_INI_STEP_RUN_OPTION_STR_LEN 260
#define MAX_INI_STEP_ARGS_STR_LEN 512


const	unsigned char DES_KEY[]="q*%#GFW&"; 

class CSettings
{
public:
	// Constructors
	CSettings();
	void	Parse();
	inline	HINSTANCE GetHInstance()           { return appInst;  }
	inline void	SetHInstance(HINSTANCE hInst) { appInst = hInst; }

	bool IsProcessRunByAdmin;
	TCHAR	CurrentDir[MAX_PATH+1];
	char	*AllRunTypes[RUN_TYPE_NO];
	
	TCHAR	AppTitle[MAX_INI_APP_TITLE_STR_LEN+1];
	TCHAR	BillboardTitle[MAX_INI_BILLBOARD_TITLE_STR_LEN+1];
	int		StepNo;
	BOOL	IsDefaultMode;
	BOOL	IsQuietMode;
	char *	StartPolicy;
	char *	StartPassword;
	TCHAR InstancefileName[MAX_INI_FILE_NAME_STR_LEN+1];

	LPCTSTR	Domain;
	LPCTSTR Id;
	LPCTSTR Password;
	
	bool	IsCompulsory[MAX_STEP_NO];
	bool	IsUnselected[MAX_STEP_NO];
	bool	RunByAdmin[MAX_STEP_NO];


	TCHAR	*StepDes[MAX_INI_STEP_DES_STR_LEN + 1];
	TCHAR	*BillboardMsg[MAX_STEP_NO];
	TCHAR	*FilePath[MAX_STEP_NO];

	TCHAR	*AbsoluteFilePath[MAX_STEP_NO];
	TCHAR	*RunType[MAX_STEP_NO];
	TCHAR	*RunOption[MAX_STEP_NO];
	TCHAR	*Args[MAX_STEP_NO];


	TCHAR	CurStepDes[MAX_INI_VALUE_STR_LEN+1];
	TCHAR	CurMsgCaptionText[MAX_INI_VALUE_STR_LEN+1];

	TCHAR	*GetPrivateProfileValue(LPCSTR Section,LPCSTR key ,bool ifPopupError) ; 
	//DWORD CheckFile (LPTSTR filename);
	void * ThrowError(LPCSTR section,LPCSTR key,LPCSTR pram1,WORD flag) ; 

	inline	LPCTSTR GetNetVersion()		{ return dotNetVersion; }

	//HRESULT GetFileVersion (LPTSTR filename, VS_FIXEDFILEINFO *pvsf) ;

private:
	HINSTANCE	appInst;
	TCHAR	settingFilePath[MAX_PATH+1];
	TCHAR	settingFilePath1[MAX_PATH + 1];
	TCHAR	dotNetVersion[15];
};

extern CSettings settings;

#endif