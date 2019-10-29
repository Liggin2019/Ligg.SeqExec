#include "des.h" 
#include "Base64.h" 
class EncryptHelper{ 
public :  
	char* DesEncrypt( unsigned char key[8], char* data);  
	char* DesDecrypt ( unsigned char key[8], char* data); CBase64 base; 
};