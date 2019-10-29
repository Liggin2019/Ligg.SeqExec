#include <stdio.h>
#include<memory.h>
#include<string.h>
#include <time.h>
#define Decode 0
#define Encode 1
bool Sub_Key[16][48];
//明文初始置换
unsigned char Initial [] =
{
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

//最终置换
unsigned char Final[] =
{
 40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

//S-盒置换
unsigned char S_Box[8][64] =
{
    /* S1 */
{ 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7, 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0, 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},

    /* S2 */
{ 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5, 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},

    /* S3 */
{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},

    /* S4 */
{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9, 10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},

    /* S5 */
{2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},

    /* S6 */
{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11, 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,  9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},

    /* S7 */
{4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1, 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,  6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},

    /* S8 */
{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2, 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
};

unsigned char Key_Exchange[56] =
{
 57, 49, 41, 33, 25, 17,  9,
  1, 58, 50, 42, 34, 26, 18,
 10,  2, 59, 51, 43, 35, 27,
 19, 11,  3, 60, 52, 44, 36,
 63, 55, 47, 39, 31, 23, 15,
  7, 62, 54, 46, 38, 30, 22,
 14,  6, 61, 53, 45, 37, 29,
 21, 13,  5, 28, 20, 12,  4
};
//压缩变换
unsigned char Compression[] =
{
    14, 17, 11, 24,  1,  5, 3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8, 16, 7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

//扩展变换
unsigned char Expansion[] =
{
32,  1,  2,  3,  4,  5,
  4,  5,  6,  7,  8,  9,
  8,  9, 10, 11, 12, 13,
 12, 13, 14, 15, 16, 17,
 16, 17, 18, 19, 20, 21,
 20, 21, 22, 23, 24, 25,
 24, 25, 26, 27, 28, 29,
 28, 29, 30, 31, 32,  1
};

//P_盒置换
unsigned char P_Box[]=
{
    16, 7, 20, 21, 29, 12, 28, 17,
 1, 15, 23, 26, 5, 18, 31, 10,
 2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

//密钥置换
unsigned char Key[]=
{
    57, 49, 41, 33, 25, 17,  9, 1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};

//
unsigned char Key_Move[]=
{
 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1
};
void Key_Process(char *);
void DES(char*,char *,int);
void Generate_SubKey(bool*,bool*);
void Rotate_Key(bool *,int);
void Initial_Exchange(char *, bool*,bool* );
void Exchange_Key(char *, bool*);
void Expand_Right(bool*, bool*);
void S_function(bool* ,bool* );
void P_function(bool * );
void Final_Exchange(bool* , bool*,char*);
void BitToByte(char *Out, const bool *In, int bits);


void Key_Process(char *S_Key)
{
    int round ;
    bool Bit_Key[56];//56bits
 Exchange_Key(S_Key,Bit_Key);

 for(round = 0 ;round<16;round++)
 {
  //KL->消息左28位，KR->消息右28位
     bool*KL = &Bit_Key[0], *KR = &Bit_Key[28];
  //循环左移
     Rotate_Key(KL,Key_Move[round]);
     Rotate_Key(KR,Key_Move[round]);
  //产生子密钥
     Generate_SubKey(Bit_Key,Sub_Key[round]);
    }
}
void Exchange_Key(char *Key , bool* New_Key)
{
    int i ,j;
 bool Bit_Key[64];

 //Transform the initial key to bool type
 for(i = 0 ;i<8;i++)
  for(j=0;j<8;j++)
  {
   Bit_Key[i*8+j] = (Key[i]>>j)&0x01;
  }

    for(i=0;i<56;i++)
        New_Key[i]=Bit_Key[Key_Exchange[i]-1];
}

void Rotate_Key(bool*Key ,int round)
{
    bool temp[64];   
 //void *memcpy( void *to, const void *from, size_t count );
    //函数从from中复制count 个字符到to中，并返回to指针
 memcpy(temp, Key, round);  
    memcpy(Key, Key+round, 28-round); 
    memcpy(Key+28-round, temp, round); 
}
void Generate_SubKey(bool*Input, bool* Output)
{
 for(int i=0;i<48;i++ )
 {
     Output[i]=Input[Compression[i]-1];
    }
}

//DES函数
void DES( char*Message,char *Final_Result,int type)
{
    bool Right[32];//32bits
 bool Left[32];//32bits
    bool Expanded_Right[48];//扩展后的右消息
    bool flag[32];//用于左右消息交换
 bool Result1[48];//48bits
 bool Result2[32];//32bits
 int round ,i;

 Initial_Exchange(Message,Left,Right);
 
 //加密算法
    if(type == Encode)
 {
  for(  round = 0 ; round < 16 ; round++ )
  {
   memcpy(flag,Right,32);

   Expand_Right(Right,Expanded_Right);

   for( i = 0 ; i<48 ; i++ )
    Result1[i]=Expanded_Right[i]^Sub_Key[round][i];

   S_function(Result1,Result2);

   P_function(Result2);

   for( i = 0 ; i<32 ; i++)
    Right[i] = Result2[i]^Left[i];
            memcpy(Left,flag,32);
  }
 }
 //解密算法
 else if(type == Decode)
 {

  for(  round = 15 ; round >=0 ; round-- )
  {
   memcpy(flag,Left,32);

   Expand_Right(Left,Expanded_Right);

   for( i = 0 ; i<48 ; i++ )
    Result1[i]=Expanded_Right[i]^Sub_Key[round][i];

   S_function(Result1,Result2);

   P_function(Result2);

   for( i = 0 ; i<32 ; i++)
    Left[i] = Result2[i]^Right[i];

    memcpy(Right,flag,32);
  }
 }
    Final_Exchange( Left , Right , Final_Result) ;

}

void Initial_Exchange(char *Message , bool*Left,bool*Right)
{
    bool temp[64];
    int i,j;
 //把 Message 转化成 bit形式
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            temp[i*8+j]= (Message[i]>>j)&0x01;
    for(i=0;i<32;i++)
        Left[i]=temp[Initial[i]-1];
    for(;i<64;i++)
        Right[i-32]=temp[Initial[i]-1];
}

void Expand_Right(bool*Input , bool*Output)
{
    for(int i =0;i<48;i++)
    Output[i]=Input[Expansion[i]-1];
}
void S_function(bool* Input,bool*Output)
{
    unsigned int x,y;
    char z;
    bool* in=Input;
    bool* out=Output;

    for(int j=0; j<8; j++,in+=6,out+=4)
    {
  //第1位和第6位
        x = (in[0]<<1) + in[5];
  //2-4位
        y = (in[1]<<3) + (in[2]<<2) + (in[3]<<1) + in[4];
  //由x y 确定的数z
        z = S_Box[j][x*16+y];
  //z用4个bit表示
        for(int i=0; i<4; i++)
        {
            out[i] = (z>>i) & 1;
        }
    }
}

void P_function(bool* a)
{
    bool temp[32];
    memcpy(temp,a,32);
    for(int i = 0 ;i<32;i++)
        a[i]=temp[P_Box[i]-1];
}

void Final_Exchange(bool*left , bool*right ,char*result )
{
    memset(result,0,8);
    bool temp1[64];
 //left 和 right 组合成 temp1
    memcpy(temp1,left,32);
    memcpy(temp1+32,right,32);
    bool temp2[64];
    for(int i=0;i<64;i++)
    {
  //由末置换得到temp2
        temp2[i]=temp1[Final[i]-1];
  //将temp2转化成char型
        result[i/8] |= temp2[i]<<(i%8);
    }
}