#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: Main.cpp
file creation date: 2016-11-21
*/

#include "NxN_matrix.h"

int main()
{
	NmatrixT* myNmatrix = read_matrix();
	print_matrix(myNmatrix);

	printf("sum of elements per rows: \n");

	for (int i = 0; i < myNmatrix->n; i++)
	{
		printf("row%d: sum of elemtents: %d\n", i, sum_of_elements_row(i, myNmatrix));
	}

	printf("sum of elements per column: \n");
	for (int i = 0; i < myNmatrix->n; i++)
	{
		printf("column%d: sum of elemtents: %d\n", i, sum_of_elements_column(i, myNmatrix));
	}

	printf("sum of elements of the first diagonal: %d\n", sum_of_elements_first_diagonal(myNmatrix));
	printf("sum of elements of the second diagonal: %d\n", sum_of_elements_second_diagonal(myNmatrix));
	
	system("pause");
	return 0;
}