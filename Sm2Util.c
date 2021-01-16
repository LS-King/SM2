#include<stdio.h>
#include<stdlib.h>
#include "miracl.h"
#include "Sm2Util.h"
#include "MyString.h"
#include "ConstValue.h"
#include "BigUtil.h"
#include "EcurveUtil.h"
#include "BigUtil.h"

int lengthC1x;						//C1的x长度
int lengthC1y;						//C1的y长度
big k;								//随机数
big PBx;							//公钥x
big PBy;							//公钥y
big dB;								//私钥
char* ccode;						//加密得到的密文
char* inputFileName = "input.txt";	//输入文件名
MyString fileData;					//输入文件数据

//生成随机数a<=x<=b
big GetBigRandom(big a, big b) {
	irand((unsigned)time(NULL));
	big result = mirvar(0);
	bigrand(Add2(Sub2(b, a), mirvar(1)), result);		// 0<= result <b-a+1
	return Add2(result, a);							// a<= xxx <=b
}

//生成随机数1<=k<=n-1
void InitRandomK() {
	k = mirvar(0);
	copy(GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(1))), k);	// 1<= k <= n-1
}

//根据预设参数设置基点G
epoint* CalculateG() {
	epoint* G = epoint_init();
	epoint_set(HexCharsToBig(Gx), HexCharsToBig(Gy), 0, G);
	return G;
}

//根据预设参数设置公钥PB
epoint* CalculatePB() {
	epoint* PB = epoint_init();
	epoint_set(PBx, PBy, 0, PB);
	return PB;
}

//计算公钥PB和私钥dB
void CalculateKeys() {
	big dm = mirvar(0);
	//随机生成整数1<=dm<=n-2作为私钥
	dm = GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(2)));
	epoint* pm = epoint_init();
	//计算点P=d*G
	pm = MultiplyEpoint(dm, CalculateG());
	//结果存储
	PBx = PointX(pm);
	PBy = PointY(pm);
	dB = dm;
}

//椭圆曲线公钥验证
int VerifyKeys() {
	epoint* pm = epoint_init();
	epoint_set(PBx, PBy, 0, pm);
	//验证PB不是无穷远点
	if (point_at_infinity(pm)) {
		printf("公钥验证无效，请重启程序\n");
		system("pause");
		exit(1);
	}
	//验证PBx和PBy在区间[0, p-1]上
	if ((mr_compare(PBx, mirvar(0)) == -1) || (mr_compare(PBx, Sub2(HexCharsToBig(p), mirvar(1))) == 1) || (mr_compare(PBy, mirvar(0)) == -1) || (mr_compare(PBy, Sub2(HexCharsToBig(p), mirvar(1))) == 1)) {
		printf("公钥验证无效，请重启程序\n");
		system("pause");
		exit(1);
	}
	//验证[n]PB是无穷远点
	if (!point_at_infinity(MultiplyEpoint(HexCharsToBig(n), pm))) {
		printf("公钥验证无效，请重启程序\n");
		system("pause");
		exit(1);
	}
	printf("公钥验证有效!\n\n");
	return 1;
}

//计算点1，(x1,y1)=k*G
epoint* CalculatePoint1() {
	return MultiplyEpoint(k, CalculateG());
}

//计算点2，(x2,y2)=h*PB
epoint* CalculatePoint2() {
	return MultiplyEpoint(k, CalculatePB());
}

//将椭圆曲线上点转换为比特串
MyString* EpointToBytes(epoint* point) {
	unsigned char* x = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	unsigned char* y = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	int lengthX = big_to_bytes(0, PointX(point), x, FALSE);
	int lengthY = big_to_bytes(0, PointY(point), y, FALSE);
	//初始化结构体
	MyString* result = (MyString*)malloc(sizeof(MyString));
	result->size = lengthX + lengthY + 1;
	result->data = (unsigned char*)malloc(sizeof(unsigned char) * (result->size));
	//非压缩比特串"04"打头，后逐位赋值
	int i = 0;
	result->data[i++] = 4;
	for (int j = 0; j < lengthX; j++) {
		result->data[i++] = x[j];
	}
	for (int j = 0; j < lengthY; j++) {
		result->data[i++] = y[j];
	}

	return result;		// 04 || point->X || point->Y
}

//从文件中读取信息
void ReadInputFile() {
	FILE* fp = fopen(inputFileName, "r");
	//打开输入文件
	if (fp == NULL) {
		printf("%s不存在\n", inputFileName);
		system("pause");
		exit(2);
	}
	//读取数据，可能包含回车、空格等，故使用fgetc
	char* data = (char*)malloc(sizeof(char) * maxLen);
	int dataSize = 0;
	char ch;
	while ((ch = fgetc(fp)) != EOF) {
		data[dataSize] = ch;
		dataSize++;
	}
	fclose(fp);
	data[dataSize] = '\0';

	fileData.data = data;
	fileData.size = dataSize;
	printf("文件读取成功！\n\n");
}

//加密
void Encryption() {
	VerifyKeys();			//验证公钥
	ReadInputFile();		//读取消息

//重新开始生成参数
Restart:				
	InitRandomK();			//初始化随机数

	//计算C1并将其十六进制存入字符串
	MyString* C1 = CalculateC1();
	char* c1String = ConvertStringAsHex(C1);
	free(C1->data);
	free(C1);
	//计算C2并将其十六进制存入字符串
	MyString* C2 = CalculateC2();
	if (C2->size <= 0 && C2->data == NULL){//t全零
		goto Restart;
	}
	char* c2String = ConvertStringAsHex(C2);
	free(C2->data);
	free(C2);

	char* c3String = CalculateC3();

	//打印加密重要参数
	printf("参数如下(以下数据均为16进制)\n");
	printf("椭圆曲线方程为：y^2=x^3+a*x+b\n");
	printf("p = %s\n", p);
	printf("a = %s\n", a);
	printf("b = %s\n", b);
	printf("n = %s\n", n);
	printf("Gx = %s\n", Gx);
	printf("Gy = %s\n", Gy);
	printf("k = %s\n\n", BigToHexChars2(k));
	printf("秘钥参数如下:\n");
	printf("私钥d = %s\n", BigToHexChars2(dB));
	printf("公钥x = %s\n", BigToHexChars2(PBx));
	printf("公钥y = %s\n", BigToHexChars2(PBy));
	printf("C1 = %s\n", c1String);
	printf("C2 = %s\n", c2String);
	printf("C3 = %s\n\n", c3String);

	char* c = (char*)calloc(strlen(c1String) + strlen(c2String) + strlen(c3String) + 1, sizeof(char));
	strcat(c, c1String);
	strcat(c, c2String);
	strcat(c, c3String);
	free(c1String);
	free(c2String);
	free(c3String);

	ccode = c;
	printf("明文为:\n%s\n\n", fileData.data);
	printf("密文为:\n%s\n\n", ccode);
}

//计算C1 = k*G
MyString* CalculateC1() {
	epoint* point1 = CalculatePoint1();
	unsigned char* x = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	unsigned char* y = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	//储存C1点x1和y1的十六进制（4位）串长度，由字符串（8位）长度乘2获得
	lengthC1x = big_to_bytes(0, PointX(point1), x, FALSE) * 2;
	lengthC1y = big_to_bytes(0, PointY(point1), y, FALSE) * 2;

	MyString* result = EpointToBytes(point1);
	epoint_free(point1);
	return result;
}

//计算C2 = Xor(M, t)
MyString* CalculateC2() {
	MyString* result = (MyString*)malloc(sizeof(MyString));
	epoint* point2 = CalculatePoint2();
	big t = KDF(point2, fileData.size);
	//判断t是否为全0
	if (mr_compare(t, mirvar(0)) == 0) {
		result->data = NULL;
		result->size = -1;
		return result;
	}
	epoint_free(point2);
	//判断M与t的长短，使其等长
	unsigned char* tString = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	int lengthT = big_to_bytes(0, t, tString, FALSE);		//求出byte数,将t转换成字符串
	if (lengthT != fileData.size) {
		for (int i = lengthT - 1; i >= 0; i--) {
			tString[fileData.size - lengthT + i] = tString[i];
		}
		for (int i = 0; i < fileData.size - lengthT; i++) {
			tString[i] = 0;
		}
		lengthT = fileData.size;
	}
	//逐位异或
	result->data = (unsigned char*)malloc(sizeof(unsigned char) * lengthT);
	for (int i = 0; i < lengthT; i++) {
		result->data[i] = tString[i] ^ fileData.data[i];
	}
	result->size = lengthT;

	free(tString);
	mirkill(t);
	return result;
}

//密钥导出函数
big KDF(epoint* point2, int klen) {
	unsigned char* xStr = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	unsigned char* yStr = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	int lengthX = big_to_bytes(0, PointX (point2), xStr, FALSE);
	int lengthY = big_to_bytes(0, PointY(point2), yStr, FALSE);
	free(xStr);
	free(yStr);

	big x = PointX(point2);
	big y = PointY(point2);
	big t = mirvar(0);
	big ct = mirvar(1);						//计数器至少32位，初值为1
	big V0 = mirvar(0);
	big Ha = mirvar(0);
	big Z = mirvar(0);
	int v = 32;								//sm3的hash值长度为32字，即256位
	
	//求解Z = x2||y2
	copy(x, Z);
	sftbit(Z, lengthY * 8, Z);				//左移y位数
	add(Z, y, Z);							//Z比特拼接y 最终Z = x2||y2

	if (klen / v >= 1) {
		for (int i = 1; i <= klen / v; i++) {
			sftbit(Z, 32, t);					//Z左移ct位数
			add(t, ct, t);						//t比特拼接ct

			big sm3Value = SM3(t);
			copy(sm3Value, V0);					//求t的hash值 t= Z||ct
			mirkill(sm3Value);
			add(Ha, V0, Ha);					//Ha比特拼接V0

			sftbit(Ha, 256, Ha);				//Ha左移V0位数，为下一次拼接做准备 Ha= Ha||x||y||ct
			add(ct, mirvar(1), ct);				//计数器加一
		}
		sftbit(Ha, -256, Ha);					//去掉多出的一次左移
	}

	if (klen % v == 0) {						//klen/v为整数
		//同上算法
		sftbit(Z, 32, t);						//左移32位
		add(t, ct, t);							//结合后的数

		big sm3Value = SM3(t);
		copy(sm3Value, V0);						//求t的hash值 t= Z||ct
		mirkill(sm3Value);

		sftbit(Ha, 256, Ha);
		add(Ha, V0, Ha);						//哈希后的数和之前的数相加
	}
	else {
		sftbit(Z, 32, t);						//左移32位
		add(t, ct, t);							//结合后的数

		copy(SM3(t), V0);						//哈希后的数，保存到V0中,256位
		sftbit(V0, -(256 - (klen * 8 - (klen / v) * v * 8)), V0);

		sftbit(Ha, klen * 8 - (klen / v) * v * 8, Ha);
		add(Ha, V0, Ha);						//哈希后的数和之前的数相加
	}
	mirkill(V0);
	mirkill(x);
	mirkill(y);
	mirkill(t);
	mirkill(ct);
	mirkill(Z);
	return Ha;
}

//计算C3 = SM3(x2 || M || y2)
char* CalculateC3() {
	epoint* point2 = CalculatePoint2();
	char* x = (char*)malloc(sizeof(char) * maxLen);
	char* y = (char*)malloc(sizeof(char) * maxLen);
	int lengthX = big_to_bytes(0, PointX(point2), x, FALSE);
	int lengthY = big_to_bytes(0, PointY(point2), y, FALSE);
	epoint_free(point2);
	//十六进制串拼接
	char* xmy = (char*)malloc(sizeof(char) * (lengthX * 2 + lengthY * 2 + fileData.size * 2 + 1));
	int i = 0;
	for (int j = 0; j < lengthX; j++) {
		sprintf(&xmy[i], "%02X", (unsigned char)(x[j]));
		i += 2;
	}
	for (int j = 0; j < fileData.size; j++) {
		sprintf(&xmy[i], "%02X", (unsigned char)(fileData.data[j]));
		i += 2;
	}
	for (int j = 0; j < lengthY; j++) {
		sprintf(&xmy[i], "%02X", (unsigned char)(y[j]));
		i += 2;
	}
	xmy[i] = '\0';
	free(x);
	free(y);

	xmy = SM3ByHexString(xmy);											//hash（x2||data||y2)
	return xmy;
}

//解密
void Decryption() {
	//从密文中得到C1、C2、C3
	char* x1String = GetPartString(ccode, 2, lengthC1x);					//2代表去除头部的04压缩种类标识
	char* y1String = GetPartString(ccode, 2 + lengthC1x, lengthC1y);
	char* C2String = GetPartString(ccode, 2 + lengthC1x + lengthC1y, strlen(ccode) - (2 + lengthC1x + lengthC1y + v));
	char* C3String = GetPartString(ccode, strlen(ccode) - v, v);
	free(ccode);
	//求解明文长度
	int klen = strlen(C2String) / 2;									//获取明文长度
	//求解C1
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	cinstr(x1, x1String);
	cinstr(y1, y1String);
	epoint* C1 = NewPoint(x1, y1);			//还原C1
	//验证C1为椭圆曲线上点
	if (point_at_infinity(C1)) {
		printf("C1验证失败，解密终止\n");
		system("pause");
		exit(1);
	}
	//求解t
	epoint* C2 = MultiplyEpoint(dB, C1);	//求解 [DB]C1 = (x2, y2)
	epoint_free(C1);
	big t = KDF(C2, klen);					//求解 t = KDF(x2∥y2,klen)
	//判断t是否为全0
	if (mr_compare(t, mirvar(0)) == 0) {
		printf("解密时t全0，错误\n");
		system("pause");
		exit(3);
	}
	//求M' = Xor(C2,t)，结果为十六进制串
	big C2Number = mirvar(0);
	cinstr(C2Number, C2String);
	char* mcode = BigToHexChars2(Xor2(C2Number, t));
	mirkill(C2Number);
	//求C3 = SM3(x2||M'||y2)
	char* xmy = (char*)calloc(strlen(BigToHexChars2(PointX(C2))) + strlen(BigToHexChars2(PointY(C2))) + strlen(mcode) + 1, sizeof(char));
	strcat(xmy, BigToHexChars2(PointX(C2)));
	strcat(xmy, mcode);
	strcat(xmy, BigToHexChars2(PointY(C2)));
	xmy[strlen(BigToHexChars2(PointX(C2))) + strlen(BigToHexChars2(PointY(C2))) + strlen(mcode)] = '\0';
	if (strcmp(SM3ByHexString(xmy), C3String) != 0) {
		printf("破译失败\n");
		return;
	}
	free(xmy);
	printf("经验证，解密成功，明文十六进制为:\n%s\n", mcode);
	//输出明文字符串
	big mNumber = mirvar(0);
	cinstr(mNumber, mcode);
	char mString[1024];
	int mStringLength = big_to_bytes(0, mNumber, mString, FALSE);
	mString[mStringLength] = '\0';
	printf("\n翻译成明文为:\n%s\n\n", mString);
}

//截取字符串
char* GetPartString(char* string, int startIndex, int length) {
	char* str = (char*)malloc(sizeof(char) * (length + 1));//位数加1个字符，尾部填"\0"构成字符串
	int i = 0;
	for (int j = 0; j < length; j++) {
		str[i++] = string[startIndex + j];
	}
	str[i] = '\0';
	return str;
}