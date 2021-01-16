#ifndef ECURVEUTIL_H
#define ECURVEUTIL_H

#include "miracl.h"

extern miracl* mip;
extern int maxLen;

epoint* NewPoint(big x, big y);			//新建一个椭圆曲线上的点
epoint* AddEpoint(epoint*, epoint*);	//椭圆曲线上的点相加
epoint* MultiplyEpoint(big, epoint*);	//椭圆曲线上的点多倍点运算
big PointX(epoint*);					//获得椭圆曲线上点的横坐标
big PointY(epoint*);					//获得椭圆曲线上点的纵坐标

#endif