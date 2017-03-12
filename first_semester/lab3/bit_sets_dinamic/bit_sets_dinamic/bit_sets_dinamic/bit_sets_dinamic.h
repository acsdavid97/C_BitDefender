/*
author: Acs David
file name: bit_sets_dinamic.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef BIT_SETS_DINAMIC_H
#define BIT_SETS_DINAMIC_H

#include <stdio.h>
#include <stdlib.h>

// structs
typedef  struct SetsTag {
	int *a;
	int *b;
	int *c;
	int INT_SIZE = 32;
	int ARRAY_SIZE = 32;
}SetsT;

// define your functions here

void print_menu();
void get_input(SetsT *sets);
void check_number_in_set(int *bit_set, int number, SetsT* sets);
void difference_sets(int *first, int *second, int *result, SetsT* sets);
void intersection_sets(int *first, int *second, int *result, SetsT* sets);
void reunion_sets(int *first, int *second, int *result, SetsT* sets);
void insert_number_in_set(int *bit_set, int number, SetsT* sets);
void print_cardinality(int *bit_set, SetsT* sets);
void insert_set(int *bit_set, SetsT* sets);
void print_elements(int *bit_set, SetsT* sets);
int* get_set(SetsT *sets);

#endif
