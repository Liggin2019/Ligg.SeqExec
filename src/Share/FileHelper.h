#include <windows.h>
#include <string>

class FileHelper
{
public:
	DWORD FileHelper::CheckFile (LPTSTR filename);
	BOOL FileHelper::DirectoryExists(LPCTSTR szPath);

};