#include <stdio.h>
#include <stdlib.h>
#include "Sm3Util.h"

void bTd(int* a, int length) {
	int j = 0;
	for (int i = 0; i < 64; i++)
		a[i] = 0;
	while (length > 0) {
		a[j] = length % 2;
		length = length / 2;
		j++;
	}
}

void CF(unsigned int* V, big B) {
	int i;
	unsigned int turn;
	unsigned int W[68];
	unsigned int w[64];
	unsigned int a[8];
	unsigned int SS1, SS2, TT1, TT2;
	unsigned int T, G;
	big one;
	big x;
	big input;
	big m;
	big A[8];							//��A,B,C,D,E,F,G,HΪ�ּĴ���,
	input = mirvar(1);
	one = mirvar(1);
	x = mirvar(1);
	m = mirvar(1);
	for (i = 0; i < 8; i++) {
		a[i] = 0;
	}
	for (i = 0; i < 68; i++) {
		W[i] = 0;
	}
	for (i = 0; i < 64; i++) {
		w[i] = 0;
	}
	for (i = 0; i < 8; i++) {
		A[i] = mirvar(1);
	}
	expb2(32, x);						//2��32�η� 
	copy(B, input);
	//����Ϣ����B(i)����Ϊ16����W0, W1, �� �� �� , W15
	for (i = 0; i < 16; i++) {

		powmod(input, one, x, m);		//��ģȡ��ŵ�m��
		W[15 - i] = *m->w;
		sftbit(input, -32, input);		//����32λ 	
	}
	//b,16-67
	for (i = 16; i < 68; i++) {
		turn = P1(W[i - 16] ^ W[i - 9] ^ (Rol(W[i - 3], 15)));
		W[i] = turn ^ (Rol(W[i - 13], 7)) ^ W[i - 6];
	}
	//c,0-63
	for (i = 0; i < 64; i++) {
		w[i] = W[i] ^ W[i + 4];
	}
	//ABCDEFGH
	for (i = 0; i < 8; i++) {
		a[i] = V[i];
	}
	for (i = 0; i < 64; i++) {
		turn = Rol(TT(i), i);
		SS1 = Rol(((Rol(a[0], 12)) + a[4] + turn), 7);
		SS2 = SS1 ^ (Rol(a[0], 12));
		T = FF(i, a[0], a[1], a[2]);
		TT1 = T + a[3] + SS2 + w[i];
		G = GG(i, a[4], a[5], a[6]);
		TT2 = G + a[7] + SS1 + W[i];
		a[3] = a[2];
		a[2] = Rol(a[1], 9);
		a[1] = a[0];
		a[0] = TT1;
		a[7] = a[6];
		a[6] = Rol(a[5], 19);
		a[5] = a[4];
		a[4] = P0(TT2);
	}
	for (i = 0; i < 8; i++) {
		V[i] = V[i] ^ a[i];
	}
}

unsigned int P1(unsigned m) {
	m = m ^ (Rol(m, 15)) ^ (Rol(m, 23));
	return m;
}

unsigned int TT(int j) {
	unsigned int a = 0X79cc4519;
	unsigned int b = 0X7a879d8a;
	if (j >= 0 && j <= 15) {
		return a;
	}
	else {
		return b;
	}
}

unsigned int FF(int i, unsigned int a, unsigned int b, unsigned int c) {
	if (i >= 0 && i <= 15) {
		return a ^ b ^ c;
	}
	else {
		return (a & b) | (a & c) | (b & c);
	}
}

unsigned int GG(int i, unsigned int a, unsigned int b, unsigned int c) {
	if (i >= 0 && i <= 15) {
		return a ^ b ^ c;
	}
	else {
		return (a & b) | (~a & c);
	}
}

unsigned int P0(unsigned int TT2) {
	return TT2 ^ (Rol(TT2, 9)) ^ (Rol(TT2, 17));
}

unsigned int Rol(unsigned int n, int m) {
	n = ((n) << m) | (n >> (32 - m));
	return n;

}

big SM3(big input) {
	big z = mirvar(1);
	big x = mirvar(1);
	big V = mirvar(0);
	big one = mirvar(1);
	unsigned int V0[8]; 						//V(0)Ϊ256���س�ʼֵIV
	big B[5];
	int l;										//��Ϣ�ĳ���
	int ll = 63;
	int length[64];								//��Ϣ�Ķ����Ʊ�ʾ 
	int n; 										//�������� 

	for (int i = 0; i < 5; i++) {
		B[i] = mirvar(0);
	}

	V0[0] = 0X7380166f;
	V0[1] = 0X4914b2b9;
	V0[2] = 0X172442d7;
	V0[3] = 0Xda8a0600;
	V0[4] = 0Xa96f30bc;
	V0[5] = 0X163138aa;
	V0[6] = 0Xe38dee4d;
	V0[7] = 0Xb0fb0e4e;

	l = 4 * numdig(input);							//���ش��������ֵĸ��� 
	bTd(length, l);									//���ȵĶ����Ʊ�ʾ���ŵ�length������ 

	//���
	sftbit(input, 1, z);							//��һ����������1λ
	incr(z, 1, z);									//��1
	copy(z, input);
	l++;
	//���0 
	while (l % 512 != 448) {
		sftbit(input, 1, z);						//:��һ����������nλ
		copy(z, input);
		l++;
	}
	// 
	while (ll >= 0) {
		sftbit(input, 1, z);						//:��һ����������1λ

		if (length[ll] == 1)
		{
			incr(z, 1, z);							//+1
		}
		copy(z, input);
		ll--;
	}

	//����
	expb2(512, x);									//2��512�η� 	
	n = (l + 64) / 512;								//512������ 

	//�����밴��512bit���� 
	for (int i = 0; i < n; i++) {
		powmod(input, one, x, B[n - 1 - i]);		//��ģȡ��ŵ�B[i]�� 
		sftbit(input, -512, input);					//����512λ 
	}
	for (int i = 0; i < n; i++) {
		CF(V0, B[i]);								//����Vi�����Vi+1  
	}
	for (int i = 0; i < 8; i++) {
		*V->w = V0[i];
		V->len = (i + 1) * 8;
		sftbit(V, 32, V);
	}
	sftbit(V, -32, V);
	return V;
}

//����SM3���ʮ�����ƴ�
char* SM3ByHexString(char* input) {
	mip->IOBASE = 16;
	big bigCodeNumber = mirvar(0);
	cinstr(bigCodeNumber, input);
	big resultNUmber = mirvar(0);
	copy(SM3(bigCodeNumber), resultNUmber);

	char* result = (char*)malloc(sizeof(char) * maxLen);
	cotstr(resultNUmber, result);
	return result;
}
