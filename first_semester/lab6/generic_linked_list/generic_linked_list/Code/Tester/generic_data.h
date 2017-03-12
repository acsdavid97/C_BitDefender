/*
author: Acs David
file name: generic_data.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_DATA_H
#define GENERIC_DATA_H

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct GenericDataTag {
	int number;
}GenericDataT;


// define your functions here
GenericDataT* create_GenericDataT(int number);
void print_GenericDataT(const void* generic_data, FILE* file);
int compare_GenericDataT(const void* a, const void* b);
void* read_and_create_GenericDataT(FILE* file);
void free_GenericDataT(void* generic_data_to_delete);

#endif
