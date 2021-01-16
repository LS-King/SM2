#include<stdio.h>
#include<stdlib.h>
#include "miracl.h"
#include "Sm2Util.h"
#include "MyString.h"
#include "ConstValue.h"
#include "BigUtil.h"
#include "EcurveUtil.h"
#include "BigUtil.h"

int lengthC1x;						//C1��x����
int lengthC1y;						//C1��y����
big k;								//�����
big PBx;							//��Կx
big PBy;							//��Կy
big dB;								//˽Կ
char* ccode;						//���ܵõ�������
char* inputFileName = "input.txt";	//�����ļ���
MyString fileData;					//�����ļ�����

//���������a<=x<=b
big GetBigRandom(big a, big b) {
	irand((unsigned)time(NULL));
	big result = mirvar(0);
	bigrand(Add2(Sub2(b, a), mirvar(1)), result);		// 0<= result <b-a+1
	return Add2(result, a);							// a<= xxx <=b
}

//���������1<=k<=n-1
void InitRandomK() {
	k = mirvar(0);
	copy(GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(1))), k);	// 1<= k <= n-1
}

//����Ԥ��������û���G
epoint* CalculateG() {
	epoint* G = epoint_init();
	epoint_set(HexCharsToBig(Gx), HexCharsToBig(Gy), 0, G);
	return G;
}

//����Ԥ��������ù�ԿPB
epoint* CalculatePB() {
	epoint* PB = epoint_init();
	epoint_set(PBx, PBy, 0, PB);
	return PB;
}

//���㹫ԿPB��˽ԿdB
void CalculateKeys() {
	big dm = mirvar(0);
	//�����������1<=dm<=n-2��Ϊ˽Կ
	dm = GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(2)));
	epoint* pm = epoint_init();
	//�����P=d*G
	pm = MultiplyEpoint(dm, CalculateG());
	//����洢
	PBx = PointX(pm);
	PBy = PointY(pm);
	dB = dm;
}

//��Բ���߹�Կ��֤
int VerifyKeys() {
	epoint* pm = epoint_init();
	epoint_set(PBx, PBy, 0, pm);
	//��֤PB��������Զ��
	if (point_at_infinity(pm)) {
		printf("��Կ��֤��Ч������������\n");
		system("pause");
		exit(1);
	}
	//��֤PBx��PBy������[0, p-1]��
	if ((mr_compare(PBx, mirvar(0)) == -1) || (mr_compare(PBx, Sub2(HexCharsToBig(p), mirvar(1))) == 1) || (mr_compare(PBy, mirvar(0)) == -1) || (mr_compare(PBy, Sub2(HexCharsToBig(p), mirvar(1))) == 1)) {
		printf("��Կ��֤��Ч������������\n");
		system("pause");
		exit(1);
	}
	//��֤[n]PB������Զ��
	if (!point_at_infinity(MultiplyEpoint(HexCharsToBig(n), pm))) {
		printf("��Կ��֤��Ч������������\n");
		system("pause");
		exit(1);
	}
	printf("��Կ��֤��Ч!\n\n");
	return 1;
}

//�����1��(x1,y1)=k*G
epoint* CalculatePoint1() {
	return MultiplyEpoint(k, CalculateG());
}

//�����2��(x2,y2)=h*PB
epoint* CalculatePoint2() {
	return MultiplyEpoint(k, CalculatePB());
}

//����Բ�����ϵ�ת��Ϊ���ش�
MyString* EpointToBytes(epoint* point) {
	unsigned char* x = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	unsigned char* y = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	int lengthX = big_to_bytes(0, PointX(point), x, FALSE);
	int lengthY = big_to_bytes(0, PointY(point), y, FALSE);
	//��ʼ���ṹ��
	MyString* result = (MyString*)malloc(sizeof(MyString));
	result->size = lengthX + lengthY + 1;
	result->data = (unsigned char*)malloc(sizeof(unsigned char) * (result->size));
	//��ѹ�����ش�"04"��ͷ������λ��ֵ
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

//���ļ��ж�ȡ��Ϣ
void ReadInputFile() {
	FILE* fp = fopen(inputFileName, "r");
	//�������ļ�
	if (fp == NULL) {
		printf("%s������\n", inputFileName);
		system("pause");
		exit(2);
	}
	//��ȡ���ݣ����ܰ����س����ո�ȣ���ʹ��fgetc
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
	printf("�ļ���ȡ�ɹ���\n\n");
}

//����
void Encryption() {
	VerifyKeys();			//��֤��Կ
	ReadInputFile();		//��ȡ��Ϣ

//���¿�ʼ���ɲ���
Restart:				
	InitRandomK();			//��ʼ�������

	//����C1������ʮ�����ƴ����ַ���
	MyString* C1 = CalculateC1();
	char* c1String = ConvertStringAsHex(C1);
	free(C1->data);
	free(C1);
	//����C2������ʮ�����ƴ����ַ���
	MyString* C2 = CalculateC2();
	if (C2->size <= 0 && C2->data == NULL){//tȫ��
		goto Restart;
	}
	char* c2String = ConvertStringAsHex(C2);
	free(C2->data);
	free(C2);

	char* c3String = CalculateC3();

	//��ӡ������Ҫ����
	printf("��������(�������ݾ�Ϊ16����)\n");
	printf("��Բ���߷���Ϊ��y^2=x^3+a*x+b\n");
	printf("p = %s\n", p);
	printf("a = %s\n", a);
	printf("b = %s\n", b);
	printf("n = %s\n", n);
	printf("Gx = %s\n", Gx);
	printf("Gy = %s\n", Gy);
	printf("k = %s\n\n", BigToHexChars2(k));
	printf("��Կ��������:\n");
	printf("˽Կd = %s\n", BigToHexChars2(dB));
	printf("��Կx = %s\n", BigToHexChars2(PBx));
	printf("��Կy = %s\n", BigToHexChars2(PBy));
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
	printf("����Ϊ:\n%s\n\n", fileData.data);
	printf("����Ϊ:\n%s\n\n", ccode);
}

//����C1 = k*G
MyString* CalculateC1() {
	epoint* point1 = CalculatePoint1();
	unsigned char* x = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	unsigned char* y = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	//����C1��x1��y1��ʮ�����ƣ�4λ�������ȣ����ַ�����8λ�����ȳ�2���
	lengthC1x = big_to_bytes(0, PointX(point1), x, FALSE) * 2;
	lengthC1y = big_to_bytes(0, PointY(point1), y, FALSE) * 2;

	MyString* result = EpointToBytes(point1);
	epoint_free(point1);
	return result;
}

//����C2 = Xor(M, t)
MyString* CalculateC2() {
	MyString* result = (MyString*)malloc(sizeof(MyString));
	epoint* point2 = CalculatePoint2();
	big t = KDF(point2, fileData.size);
	//�ж�t�Ƿ�Ϊȫ0
	if (mr_compare(t, mirvar(0)) == 0) {
		result->data = NULL;
		result->size = -1;
		return result;
	}
	epoint_free(point2);
	//�ж�M��t�ĳ��̣�ʹ��ȳ�
	unsigned char* tString = (unsigned char*)malloc(sizeof(unsigned char) * maxLen);
	int lengthT = big_to_bytes(0, t, tString, FALSE);		//���byte��,��tת�����ַ���
	if (lengthT != fileData.size) {
		for (int i = lengthT - 1; i >= 0; i--) {
			tString[fileData.size - lengthT + i] = tString[i];
		}
		for (int i = 0; i < fileData.size - lengthT; i++) {
			tString[i] = 0;
		}
		lengthT = fileData.size;
	}
	//��λ���
	result->data = (unsigned char*)malloc(sizeof(unsigned char) * lengthT);
	for (int i = 0; i < lengthT; i++) {
		result->data[i] = tString[i] ^ fileData.data[i];
	}
	result->size = lengthT;

	free(tString);
	mirkill(t);
	return result;
}

//��Կ��������
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
	big ct = mirvar(1);						//����������32λ����ֵΪ1
	big V0 = mirvar(0);
	big Ha = mirvar(0);
	big Z = mirvar(0);
	int v = 32;								//sm3��hashֵ����Ϊ32�֣���256λ
	
	//���Z = x2||y2
	copy(x, Z);
	sftbit(Z, lengthY * 8, Z);				//����yλ��
	add(Z, y, Z);							//Z����ƴ��y ����Z = x2||y2

	if (klen / v >= 1) {
		for (int i = 1; i <= klen / v; i++) {
			sftbit(Z, 32, t);					//Z����ctλ��
			add(t, ct, t);						//t����ƴ��ct

			big sm3Value = SM3(t);
			copy(sm3Value, V0);					//��t��hashֵ t= Z||ct
			mirkill(sm3Value);
			add(Ha, V0, Ha);					//Ha����ƴ��V0

			sftbit(Ha, 256, Ha);				//Ha����V0λ����Ϊ��һ��ƴ����׼�� Ha= Ha||x||y||ct
			add(ct, mirvar(1), ct);				//��������һ
		}
		sftbit(Ha, -256, Ha);					//ȥ�������һ������
	}

	if (klen % v == 0) {						//klen/vΪ����
		//ͬ���㷨
		sftbit(Z, 32, t);						//����32λ
		add(t, ct, t);							//��Ϻ����

		big sm3Value = SM3(t);
		copy(sm3Value, V0);						//��t��hashֵ t= Z||ct
		mirkill(sm3Value);

		sftbit(Ha, 256, Ha);
		add(Ha, V0, Ha);						//��ϣ�������֮ǰ�������
	}
	else {
		sftbit(Z, 32, t);						//����32λ
		add(t, ct, t);							//��Ϻ����

		copy(SM3(t), V0);						//��ϣ����������浽V0��,256λ
		sftbit(V0, -(256 - (klen * 8 - (klen / v) * v * 8)), V0);

		sftbit(Ha, klen * 8 - (klen / v) * v * 8, Ha);
		add(Ha, V0, Ha);						//��ϣ�������֮ǰ�������
	}
	mirkill(V0);
	mirkill(x);
	mirkill(y);
	mirkill(t);
	mirkill(ct);
	mirkill(Z);
	return Ha;
}

//����C3 = SM3(x2 || M || y2)
char* CalculateC3() {
	epoint* point2 = CalculatePoint2();
	char* x = (char*)malloc(sizeof(char) * maxLen);
	char* y = (char*)malloc(sizeof(char) * maxLen);
	int lengthX = big_to_bytes(0, PointX(point2), x, FALSE);
	int lengthY = big_to_bytes(0, PointY(point2), y, FALSE);
	epoint_free(point2);
	//ʮ�����ƴ�ƴ��
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

	xmy = SM3ByHexString(xmy);											//hash��x2||data||y2)
	return xmy;
}

//����
void Decryption() {
	//�������еõ�C1��C2��C3
	char* x1String = GetPartString(ccode, 2, lengthC1x);					//2����ȥ��ͷ����04ѹ�������ʶ
	char* y1String = GetPartString(ccode, 2 + lengthC1x, lengthC1y);
	char* C2String = GetPartString(ccode, 2 + lengthC1x + lengthC1y, strlen(ccode) - (2 + lengthC1x + lengthC1y + v));
	char* C3String = GetPartString(ccode, strlen(ccode) - v, v);
	free(ccode);
	//������ĳ���
	int klen = strlen(C2String) / 2;									//��ȡ���ĳ���
	//���C1
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	cinstr(x1, x1String);
	cinstr(y1, y1String);
	epoint* C1 = NewPoint(x1, y1);			//��ԭC1
	//��֤C1Ϊ��Բ�����ϵ�
	if (point_at_infinity(C1)) {
		printf("C1��֤ʧ�ܣ�������ֹ\n");
		system("pause");
		exit(1);
	}
	//���t
	epoint* C2 = MultiplyEpoint(dB, C1);	//��� [DB]C1 = (x2, y2)
	epoint_free(C1);
	big t = KDF(C2, klen);					//��� t = KDF(x2��y2,klen)
	//�ж�t�Ƿ�Ϊȫ0
	if (mr_compare(t, mirvar(0)) == 0) {
		printf("����ʱtȫ0������\n");
		system("pause");
		exit(3);
	}
	//��M' = Xor(C2,t)�����Ϊʮ�����ƴ�
	big C2Number = mirvar(0);
	cinstr(C2Number, C2String);
	char* mcode = BigToHexChars2(Xor2(C2Number, t));
	mirkill(C2Number);
	//��C3 = SM3(x2||M'||y2)
	char* xmy = (char*)calloc(strlen(BigToHexChars2(PointX(C2))) + strlen(BigToHexChars2(PointY(C2))) + strlen(mcode) + 1, sizeof(char));
	strcat(xmy, BigToHexChars2(PointX(C2)));
	strcat(xmy, mcode);
	strcat(xmy, BigToHexChars2(PointY(C2)));
	xmy[strlen(BigToHexChars2(PointX(C2))) + strlen(BigToHexChars2(PointY(C2))) + strlen(mcode)] = '\0';
	if (strcmp(SM3ByHexString(xmy), C3String) != 0) {
		printf("����ʧ��\n");
		return;
	}
	free(xmy);
	printf("����֤�����ܳɹ�������ʮ������Ϊ:\n%s\n", mcode);
	//��������ַ���
	big mNumber = mirvar(0);
	cinstr(mNumber, mcode);
	char mString[1024];
	int mStringLength = big_to_bytes(0, mNumber, mString, FALSE);
	mString[mStringLength] = '\0';
	printf("\n���������Ϊ:\n%s\n\n", mString);
}

//��ȡ�ַ���
char* GetPartString(char* string, int startIndex, int length) {
	char* str = (char*)malloc(sizeof(char) * (length + 1));//λ����1���ַ���β����"\0"�����ַ���
	int i = 0;
	for (int j = 0; j < length; j++) {
		str[i++] = string[startIndex + j];
	}
	str[i] = '\0';
	return str;
}