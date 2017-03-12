/*
author: Acs David
file name: NxN_MATRIX.h
file creation date: 2016-21-11
*/

#ifndef NxN_MATRIX_H
#define NxN_MATRIX_H

#include <stdio.h>
#include <stdlib.h>

// structs

typedef struct NMatrixTag {
	int n;
	int **matrix;
}NmatrixT;

// define your functions here

NmatrixT* read_matrix();
void print_matrix(NmatrixT* nmatrix);
int sum_of_elements_row(int row, NmatrixT* nmatrix);
int sum_of_elements_column(int column, NmatrixT *nmatrix);
int sum_of_elements_first_diagonal(NmatrixT *nmatrix);
int sum_of_elements_second_diagonal(NmatrixT *nmatrix);
int sum_of_ring(int ring, NmatrixT *nmatrix);

#endif
