#include <stdio.h>
#include <stdlib.h>
#include "miracl.h"

//新建一个椭圆曲线上的点
epoint* NewPoint(big x, big y) {
	epoint* result = epoint_init();
	epoint_set(x, y, 0, result);
	return result;
}

//椭圆曲线上的点相加
epoint* AddEpoint(epoint* a, epoint* b) {
	epoint* result = epoint_init();
	epoint_copy(b, result);
	ecurve_add(a, result);
	return result;
}

//椭圆曲线上的点多倍点运算
epoint* MultiplyEpoint(big a, epoint* b) {
	epoint* result = epoint_init();
	ecurve_mult(a, b, result);
	return result;
}

//获得椭圆曲线上点的横坐标
big PointX(epoint* point) {
	big x = mirvar(0);
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(y);
	return x;
}

//获得椭圆曲线上点的纵坐标
big PointY(epoint* point) {
	big x = mirvar(0);
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(x);
	return y;
}