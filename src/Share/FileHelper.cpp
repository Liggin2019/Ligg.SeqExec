#include <windows.h>
#include <tchar.h>
#include <stdio.h>


#include "FileHelper.h"

DWORD FileHelper::CheckFile (LPTSTR filename) 
{
	HANDLE handle = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (handle==INVALID_HANDLE_VALUE)
	{
		DWORD dwResult = GetLastError();
		return dwResult;
	}
	else
	{
		CloseHandle(handle);
		return 0;
	}
}


BOOL FileHelper::DirectoryExists(LPCTSTR szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}









