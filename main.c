#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "miracl.h"
#include "ConstValue.h"
#include "Sm2Util.h"

miracl* mip;
int maxLen = 8192;			//ȫ���ַ�����󳤶�
int v = 64;					//��ϣ��16���ƴ�������256λ

int main() {
	mip = mirsys(500, 0);
	//��ʼ����Բ����
	ecurve_init(HexCharsToBig(a), HexCharsToBig(b), HexCharsToBig(p), MR_PROJECTIVE);
	//���㹫Կ��˽Կ
	CalculateKeys();
	//����
	Encryption();
	//����
	Decryption();
	system("pause");
	mirkill(mip);
	return 0;
}