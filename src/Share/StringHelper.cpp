
#include "StringHelper.h"
#include <stdio.h>  

bool  StringHelper::IsAscilString(char *a) 
{
	int i;
	for(i=0;a[i]!='\0';i++)
	{
		if (a[i]<32||a[i]>126) return false;
	}
	return true;
}

void StringHelper::Split(char *src, const char *separator, char **dest, int *getCount)
{
    char *pNext;
    int count = 0;
    
    if (src == NULL || strlen(src) == 0) return;
    if (separator == NULL || strlen(separator) == 0) return; 

    pNext = strtok(src,separator);
    
    while(pNext != NULL)
    {
        *dest++ = pNext;
        ++count;
        pNext = strtok(NULL,separator);
    }
    *getCount = count;
}



char*  StringHelper::ToLower(char* str) 
{
	int i;
	char * strTolower=new char[strlen(str)+1];
	sprintf(strTolower,"%s",str);
	for (int i=0; i <=strlen(str); i++) 
	{
		strTolower[i] = tolower(str[i]);
	}
	return strTolower;
}

char*  StringHelper::Xor(char a[] ,char b[]) 
{
	int i;
	for(i=0;a[i]!='\0';i++)
		a[i]=a[i]^b[i];
	return a;
}

wchar_t * StringHelper::AnsiToUnicode( const char* str )
{
	int textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 ); 
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
	memset(result,0,(textlen+1)*sizeof(wchar_t)); 
	MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen ); 
	return result; 
}

size_t getStringIndex(char *searchStr, char *searchFor)
{
	size_t searchLen = strlen(searchStr);
	size_t forLen = strlen(searchFor);
	for (size_t i=0; i < searchLen; i++)
	{
		if (searchStr[i] != searchFor[0])
			continue;
		bool found = false;
		for (size_t j=0; j < forLen; j++)
		{
			found = false;
			if (i+j > searchLen)
				break;
			if (searchStr[i+j] != searchFor[j])
				break;
			found = true;
		}
		if (found)
			return i;
	}
	return -1;
}


