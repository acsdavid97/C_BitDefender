#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: matrix_ops.cpp
file creation date: 2016-12-01 13:35:34
*/

#include "matrix_ops.h"

MatrixT* allocate_matrix()
{
	MatrixT* matrix = (MatrixT*)malloc(sizeof(MatrixT));

	printf("enter the number of rows for the matrix");
	int aux = 0;
	scanf("%d", &aux);
	matrix->nr_rows = aux;
	printf("enter the number of columns for the matrix");
	scanf("%d", &aux);
	matrix->nr_cols = aux;

	matrix->elements = (int**)malloc(sizeof(int*) * matrix->nr_rows);
	for (int i = 0; i < matrix->nr_rows; i++)
	{
		*(matrix->elements + i) = (int*)malloc(sizeof(int) * matrix->nr_cols);
	}

	return matrix;
}

//matrix must be allocated, and size set to correct value
void read_matrix(MatrixT* matrix)
{
	for (int i = 0; i < matrix->nr_rows; i++)
	{
		for (int j = 0; j < matrix->nr_cols; j++)
		{
			printf("matrix[%d][%d]=", i, j);
			scanf("%d", *(matrix->elements + i) + j);
		}
	}
}

void print_matrix(MatrixT* matrix)
{
	for (int i = 0; i < matrix->nr_rows; i++)
	{
		for (int j = 0; j < matrix->nr_cols; j++)
		{
			printf("%d ", *(*(matrix->elements + i) + j));
		}
		printf("\n");
	}
}

void add_line(MatrixT* matrix, int line_nr, OrientationE orientation)
{
	if (line_nr > matrix->nr_rows)
	{
		printf("cannot insert line, line number bigger than matrix's number of rows");
		return;
	}

	matrix->elements = (int**)realloc(matrix->elements, sizeof(int*) * (matrix->nr_rows + 1));
	//shift elements to the right
	for (int i = matrix->nr_rows; i >= line_nr; i--)
	{
		*(matrix->elements + i + 1) = *(matrix->elements + i);
	}

	int* to_insert = (int*)malloc(sizeof(int) * matrix->nr_cols);
	if (orientation == FORWARD)
	{
		for (int i = 0; i < matrix->nr_cols; i++)
		{
			printf("matrix[%d][%d]=", line_nr, i);
			scanf("%d", to_insert + i);
		}
	}

	if (orientation == REVERSE)
	{
		for (int i = matrix->nr_cols - 1; i >= 0; i--)
		{
			printf("matrix[%d][%d]=", line_nr, i);
			scanf("%d", to_insert + i);
		}

	}
	*(matrix->elements + line_nr) = to_insert;
	matrix->nr_rows++;

}

void add_column(MatrixT* matrix, int column_nr, OrientationE orientation)
{
	if (column_nr > matrix->nr_cols)
	{
		printf("cannot insert column, column number bigger than matrix's number of cols");
		return;
	}
	if (orientation == FORWARD)
	{
		for (int i = 0; i < matrix->nr_rows; i++)
		{
			*(matrix->elements + i) = (int*)realloc(*(matrix->elements + i), sizeof(int) * (matrix->nr_cols + 1));
			for (int j = matrix->nr_cols; j >= column_nr; j--)
			{
				*(*(matrix->elements + i) + j + 1) = *(*(matrix->elements + i) + j);
			}
			printf("matrix[%d][%d]=", i, column_nr);
			scanf("%d", *(matrix->elements + i) + column_nr);
		}
	}
	if (orientation == REVERSE)
	{
		for (int i = matrix->nr_rows - 1; i >= 0; i--)
		{
			*(matrix->elements + i) = (int*)realloc(*(matrix->elements + i), sizeof(int) * (matrix->nr_cols + 1));
			for (int j = matrix->nr_cols; j >= column_nr; j--)
			{
				*(*(matrix->elements + i) + j + 1) = *(*(matrix->elements + i) + j);
			}
			printf("matrix[%d][%d]=", i, column_nr);
			scanf("%d", *(matrix->elements + i) + column_nr);
		}
	}
	matrix->nr_cols++;
}

void add_ring(MatrixT* matrix)
{
	add_line(matrix, 0, FORWARD);
	add_column(matrix, matrix->nr_cols, FORWARD);
	add_line(matrix, matrix->nr_rows, REVERSE);
	add_column(matrix, 0, REVERSE);
}
