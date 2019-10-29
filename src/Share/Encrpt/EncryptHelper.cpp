#include "EncryptHelper.h"  
#include <stdio.h>  
char* EncryptHelper::DesEncrypt( unsigned char key[8], char* data) 
{ 
//将字符指针转化为字符数组 
char* datavv = data; 
data =(char *) calloc(100,1); 
memcpy(data,datavv,strlen(datavv)); 
data[strlen(data)] = '\0'; 
//DES加密  
DES des;  
des.encrypt(key,(unsigned char *)data,1);  //base64编码 
base.Encode(data); 
char* res =(char *) base.EncodedMessage(); 

return res; 
}; 


char* EncryptHelper::DesDecrypt ( unsigned char key[8], char* data) { 
//base64解码 
base.Decode(data); 
char* datares=(char *) base.DecodedMessage(); 
//将字符指针转化为字符数组 
char* datarea = datares; 
datares = (char *)malloc(256); 
memcpy(datares,datarea,strlen(datarea)); datares[strlen(datarea)] = '\0'; 


//DES解密 
DES des;  
des.decrypt(key,(unsigned char*) datares,1); 

return datares; 
};