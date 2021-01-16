#ifndef SM3UTIL_H
#define SM3UTIL_H

#include "miracl.h"

extern miracl* mip;
extern int maxLen;

big SM3(big);
char* SM3ByHexString(char*);
void bTd(int* a, int length);
void CF(unsigned int* V, big B);//压缩函数
unsigned int P1(unsigned m);//消息扩展中的置换函数
unsigned int TT(int j);
unsigned int FF(int i, unsigned int a, unsigned int b, unsigned int c);//布尔函数，随j的变化取不同的表达式
unsigned int GG(int i, unsigned int a, unsigned int b, unsigned int c);//布尔函数，随j的变化取不同的表达式
unsigned int P0(unsigned int TT2);//压缩函数中的置换函数
unsigned int Rol(unsigned int n, int m);

#endif