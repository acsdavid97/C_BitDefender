#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: NxN_matrix.cpp
file creation date: 2016-11-21
*/

#include "NxN_matrix.h"

// creates a matrix from int** and n
NmatrixT* createMatrix(int n, int **matrix)
{
	NmatrixT* nmatrix = (NmatrixT*)malloc(sizeof(NmatrixT));
	nmatrix->matrix = matrix;
	nmatrix->n = n;
	return nmatrix;
}

//reads a matrix from the keyboard and returns a pointer to Nmatrix
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

//prints a matrix to stdout
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

//calulates the sum of elements on a row
int sum_of_elements_row(int row, NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + row) + i);
	}

	return sum;
}

//calulates the sum of elements on a column
int sum_of_elements_column(int column, NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + i) + column);
	}

	return sum;
}


//calulates the sum of elements on the first diagonal
int sum_of_elements_first_diagonal(NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + i) + i);
	}

	return sum;
}

//calulates the sum of elements on the second diagonal
int sum_of_elements_second_diagonal(NmatrixT *nmatrix)
{
	int sum = 0;

	for (int i = 0; i < nmatrix->n; i++)
	{
		sum += *(*(nmatrix->matrix + i) + nmatrix->n - i - 1);
	}

	return sum;
}

//calculates the sum of elements on a ring
int sum_of_ring(int ring, NmatrixT *nmatrix)
{
	int sum = 0;
	for (int i = ring; i < nmatrix->n - ring; i++)
	{
		sum += *(*(nmatrix->matrix + ring) + i);
		sum += *(*(nmatrix->matrix + nmatrix->n - ring -1) + i);
		sum += *(*(nmatrix->matrix + i) + ring);
		sum += *(*(nmatrix->matrix + i) + nmatrix->n - ring - 1);
	}
	sum -= *(*(nmatrix->matrix + ring) + ring);
	sum -= *(*(nmatrix->matrix + ring) + nmatrix->n - ring - 1);
	sum -= *(*(nmatrix->matrix + nmatrix->n - ring - 1) + ring);
	sum -= *(*(nmatrix->matrix + nmatrix->n - ring - 1) + nmatrix->n - ring -1 );

	return sum;
}
