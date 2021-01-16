#ifndef SM3UTIL_H
#define SM3UTIL_H

#include "miracl.h"

extern miracl* mip;
extern int maxLen;

big SM3(big);
char* SM3ByHexString(char*);
void bTd(int* a, int length);
void CF(unsigned int* V, big B);//ѹ������
unsigned int P1(unsigned m);//��Ϣ��չ�е��û�����
unsigned int TT(int j);
unsigned int FF(int i, unsigned int a, unsigned int b, unsigned int c);//������������j�ı仯ȡ��ͬ�ı��ʽ
unsigned int GG(int i, unsigned int a, unsigned int b, unsigned int c);//������������j�ı仯ȡ��ͬ�ı��ʽ
unsigned int P0(unsigned int TT2);//ѹ�������е��û�����
unsigned int Rol(unsigned int n, int m);

#endif