#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-11-22 10:41:58
*/

#include "bit_sets_dinamic.h"


int main()
{
	SetsT* sets = (SetsT*)malloc(sizeof(SetsT));
	sets->ARRAY_SIZE = 32;
	sets->INT_SIZE = 32;
	sets->a = (int*)calloc(sets->ARRAY_SIZE, sizeof(int));
	sets->b = (int*)calloc(sets->ARRAY_SIZE, sizeof(int));
	sets->c = (int*)calloc(sets->ARRAY_SIZE, sizeof(int));

	print_menu();

	while (1)
	{
		get_input(sets);
	}

	system("pause");
	return 0;
}

