#include<stdio.h>
#include<stdlib.h>
#include "MyString.h"

//���ַ���ת��Ϊ16��������
char* ConvertStringAsHex(MyString* s) {
	char* result = (char*)malloc(sizeof(char) * (s->size * 2 + 1));
	for (int i = 0; i < s->size; i++) {
		sprintf(&result[i * 2], "%02X", s->data[i]);
	}
	result[s->size * 2] = '\0';
	return result;
}