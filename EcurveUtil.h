#ifndef ECURVEUTIL_H
#define ECURVEUTIL_H

#include "miracl.h"

extern miracl* mip;
extern int maxLen;

epoint* NewPoint(big x, big y);			//�½�һ����Բ�����ϵĵ�
epoint* AddEpoint(epoint*, epoint*);	//��Բ�����ϵĵ����
epoint* MultiplyEpoint(big, epoint*);	//��Բ�����ϵĵ�౶������
big PointX(epoint*);					//�����Բ�����ϵ�ĺ�����
big PointY(epoint*);					//�����Բ�����ϵ��������

#endif