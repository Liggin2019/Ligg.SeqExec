
#include <windows.h>
#include <string>

class StringHelper
{
public:
	bool  StringHelper::IsAscilString(char *a); 
	void StringHelper::Split(char *src, const char *separator, char **dest, int *num);
	char*  StringHelper::ToLower(char str[]);
	char*  StringHelper::Xor(char a[] ,char b[]); 
	wchar_t * StringHelper::AnsiToUnicode( const char* str );
	//size_t GetStringIndex(char *searchStr, char *searchFor);

};



