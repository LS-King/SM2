#ifndef MYSTRING_H
#define MYSTRING_H

typedef struct MyString
{
	unsigned char* data;
	int size;
}MyString;

char* ConvertStringAsHex(MyString*);

#endif