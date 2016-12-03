/*
author: Acs David
file name: matrix_ops.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include <stdio.h>
#include <stdlib.h>

typedef enum OrientationETag {
	FORWARD, REVERSE
}OrientationE;

// structs
typedef struct MatrixTag {
	int nr_rows;
	int nr_cols;
	int **elements;
}MatrixT;

// define your functions here
MatrixT* allocate_matrix();
void print_matrix(MatrixT* matrix);
void read_matrix(MatrixT* matrix);
void add_line(MatrixT* matrix, int line_nr, OrientationE orientation);
void add_column(MatrixT* matrix, int column_nr, OrientationE orientation);
void add_ring(MatrixT* matrix);

#endif
