/*
author: Acs David
file name: generic_data_type
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_DATA_TYPE_H
#define GENERIC_DATA_TYPE_H

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct GenericDataTag {
	int data;
	double daat2;
}GenericDataT;

// define your functions here
GenericDataT* create_GenericDataT(int data);
void print_GenericDataT(GenericDataT* element);

#endif
