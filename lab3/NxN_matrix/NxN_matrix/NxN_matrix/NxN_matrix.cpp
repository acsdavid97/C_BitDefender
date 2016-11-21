#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: NxN_matrix.cpp
file creation date: 2016-11-21
*/

#include "NxN_matrix.h"

NmatrixT* createMatrix(int n, int **matrix)
{
	NmatrixT* nmatrix = (NmatrixT*)malloc(sizeof(NmatrixT));
	nmatrix->matrix = matrix;
	nmatrix->n = n;
	return nmatrix;
}

NmatrixT* read_matrix()
{
	int n = 0;
	printf("n=");
	scanf("%d", &n);
	int **matrix = (int**)malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++)
	{
		*(matrix + i) = (int*)malloc(sizeof(int) * n);
		for (int j = 0; j < n; j++)
		{
			printf("Matrix[%d][%d]=", i, j);
			scanf("%d", *(matrix + i) + j);
		}
	}
	return createMatrix(n, matrix);
}

void print_matrix(NmatrixT* nmatrix)
{
	printf("size of the matrix is: %d\n", nmatrix->n);
	printf("contents: \n");
	for (int i = 0; i < nmatrix->n; i++)
	{
		for (int j = 0; j < nmatrix->n; j++)
		{
			printf("%d ", *(*(nmatrix->matrix + i) + j));
		}
		printf("\n");
	}
}

int sum_of_elements_row(int row, NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + row) + i);
	}

	return sum;
}

int sum_of_elements_column(int column, NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + i) + column);
	}

	return sum;
}


int sum_of_elements_first_diagonal(NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + i) + i);
	}

	return sum;
}

int sum_of_elements_second_diagonal(NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + i) + nmatrix->n - i - 1);
	}

	return sum;
}
