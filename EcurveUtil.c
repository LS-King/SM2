#include <stdio.h>
#include <stdlib.h>
#include "miracl.h"

//�½�һ����Բ�����ϵĵ�
epoint* NewPoint(big x, big y) {
	epoint* result = epoint_init();
	epoint_set(x, y, 0, result);
	return result;
}

//��Բ�����ϵĵ����
epoint* AddEpoint(epoint* a, epoint* b) {
	epoint* result = epoint_init();
	epoint_copy(b, result);
	ecurve_add(a, result);
	return result;
}

//��Բ�����ϵĵ�౶������
epoint* MultiplyEpoint(big a, epoint* b) {
	epoint* result = epoint_init();
	ecurve_mult(a, b, result);
	return result;
}

//�����Բ�����ϵ�ĺ�����
big PointX(epoint* point) {
	big x = mirvar(0);
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(y);
	return x;
}

//�����Բ�����ϵ��������
big PointY(epoint* point) {
	big x = mirvar(0);
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(x);
	return y;
}