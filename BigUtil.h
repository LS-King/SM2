#ifndef BIGUTIL_H
#define BIGUTIL_H

#include "miracl.h"

extern miracl* mip;
extern int maxLen;

//big类型基本运算封装
big Xor2(big x, big y);
big Add2(big x, big y);
big Sub2(big x, big y);
big Multiply2(big x, big y);
big Divide2(big x, big y);
big Mod2(big x, big y);
big Pow2(big x, int y);					// x^y
char* BigToHexChars2(big x);		//大数转十六进制串
big HexCharsToBig(char* str);		//十六进制串转大数

#endif