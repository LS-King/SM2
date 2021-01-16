#ifndef BIGUTIL_H
#define BIGUTIL_H

#include "miracl.h"

extern miracl* mip;
extern int maxLen;

//big���ͻ��������װ
big Xor2(big x, big y);
big Add2(big x, big y);
big Sub2(big x, big y);
big Multiply2(big x, big y);
big Divide2(big x, big y);
big Mod2(big x, big y);
big Pow2(big x, int y);					// x^y
char* BigToHexChars2(big x);		//����תʮ�����ƴ�
big HexCharsToBig(char* str);		//ʮ�����ƴ�ת����

#endif