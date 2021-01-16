#ifndef SM2UTIL_H
#define SM2UTIL_H

#include "miracl.h"
#include "MyString.h"

//�ⲿ��������
extern miracl* mip;
extern int maxLen;
extern int v;

//���ܺ�������
big GetBigRandom(big, big);				//��������� a<= result <=b
void InitRandomK();						//�����������K
void CalculateKeys();					//������Կ��˽Կ
int VerifyKeys();						//��֤��Կ��˽Կ
epoint* CalculateG();					//�Զ������G
epoint* CalculatePB();					//��ԿPB(PBx,PBy)
MyString* EpointToBytes(epoint*);		////����Բ�����ϵ�ת��Ϊ���ش���δѹ����ʽ����ͷ��PC=04
void ReadInputFile();					//��ȡ�ļ�����
void Encryption();						//����
epoint* CalculatePoint1();				//����(x1,y1)
epoint* CalculatePoint2();				//����(x2,y2)
MyString* CalculateC1();				//����C1
MyString* CalculateC2();				//����C2
char* CalculateC3();					//����C3
big KDF(epoint*, int);					//����t
void Decryption();						//����
char* GetPartString(char*, int, int);	//��ȡ�ַ���

#endif