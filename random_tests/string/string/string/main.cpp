#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2016-11-26 23:05:52
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
	char str1[] = "A string";
	char * str2;
	str2 = str1;
	str2[0] = 'D';
	printf("str1: '%s'\nstr2:'%s'\n", str1, str2);	char str22[] = "A string";
	str22[0] = 'H';	printf("%s\n", str22);
	char s[] = "UnSirDeCaractere";
	for (char *p = s; *p++; putchar((*p++)++));
	printf("\n%s", s);
	printf("\n");
	system("pause");
	return 0;
}