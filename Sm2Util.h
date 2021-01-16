#ifndef SM2UTIL_H
#define SM2UTIL_H

#include "miracl.h"
#include "MyString.h"

//外部变量导入
extern miracl* mip;
extern int maxLen;
extern int v;

//功能函数声明
big GetBigRandom(big, big);				//产生随机数 a<= result <=b
void InitRandomK();						//产生随机参数K
void CalculateKeys();					//产生公钥和私钥
int VerifyKeys();						//验证公钥和私钥
epoint* CalculateG();					//自定义参数G
epoint* CalculatePB();					//公钥PB(PBx,PBy)
MyString* EpointToBytes(epoint*);		////将椭圆曲线上点转换为比特串，未压缩形式，故头部PC=04
void ReadInputFile();					//读取文件输入
void Encryption();						//加密
epoint* CalculatePoint1();				//计算(x1,y1)
epoint* CalculatePoint2();				//计算(x2,y2)
MyString* CalculateC1();				//计算C1
MyString* CalculateC2();				//计算C2
char* CalculateC3();					//计算C3
big KDF(epoint*, int);					//计算t
void Decryption();						//解密
char* GetPartString(char*, int, int);	//截取字符串

#endif