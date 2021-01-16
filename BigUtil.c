#include <stdio.h>
#include <stdlib.h>
#include "miracl.h"
#include "BigUtil.h"

//大数异或
big Xor2(big x, big y) {
	big result = mirvar(0);
	unsigned char xString[1000];
	unsigned char yString[1000];
	//大数转换为二进制比特串
	int lengthX = big_to_bytes(0, x, xString, FALSE);
	int lengthY = big_to_bytes(0, y, yString, FALSE);

	if (lengthX < lengthY) {
		for (int i = 0; i < lengthX; i++) {
			yString[lengthY - 1 - i] ^= xString[lengthX - 1 - i];
		}
		//二进制比特串转换为大数
		bytes_to_big(lengthY, yString, result);
	}
	else {
		for (int i = 0; i < lengthY; i++) {
			xString[lengthX - 1 - i] ^= yString[lengthY - 1 - i];
		}
		//二进制比特串转换为大数
		bytes_to_big(lengthX, xString, result);
	}
	return result;
}

//大数加法
big Add2(big x, big y) {
	big result = mirvar(0);
	add(x, y, result);
	return result;
}

//大数减法
big Sub2(big x, big y) {
	big result = mirvar(0);
	subtract(x, y, result);
	return result;
}

//大数乘法
big Multiply2(big x, big y) {
	big result = mirvar(0);
	multiply(x, y, result);
	return result;
}

//大数除法
big Divide2(big x, big y) {
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	big z1 = mirvar(0);
	copy(x, x1);
	copy(y, y1);
	divide(x1, y1, z1);
	mirkill(x1);
	mirkill(y1);
	return z1;
}

//大数取余
big Mod2(big x, big y) {
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	big z1 = mirvar(0);
	copy(x, x1);
	copy(y, y1);
	powmod(x1, mirvar(1), y1, z1);
	mirkill(x1);
	mirkill(y1);
	return z1;
}

//大数乘方
big Pow2(big x, int y) {
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	copy(x, x1);
	for (int i = 0; i < y; i++)
	{
		y1 = Multiply2(x1, x1);
	}
	mirkill(x1);
	return y1;
}

//大数转换为十六进制串
char* BigToHexChars2(big x) {
	mip->IOBASE = 16;
	char* str = (char*)malloc(sizeof(char) * maxLen);
	cotstr(x, str);
	return str;
}

//十六进制数串转换为大数
big HexCharsToBig(char* str) {
	mip->IOBASE = 16;
	big result = mirvar(0);
	cinstr(result, str);
	return result;
}