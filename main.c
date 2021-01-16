#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "miracl.h"
#include "ConstValue.h"
#include "Sm2Util.h"

miracl* mip;
int maxLen = 8192;			//全局字符串最大长度
int v = 64;					//哈希的16进制串长，即256位

int main() {
	mip = mirsys(500, 0);
	//初始化椭圆曲线
	ecurve_init(HexCharsToBig(a), HexCharsToBig(b), HexCharsToBig(p), MR_PROJECTIVE);
	//计算公钥和私钥
	CalculateKeys();
	//加密
	Encryption();
	//解密
	Decryption();
	system("pause");
	mirkill(mip);
	return 0;
}