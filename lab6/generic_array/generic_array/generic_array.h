/*
author: Acs David
file name: generic_array.cpp
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct GenericDataTag {
	int data;
}GenericDataT;


typedef struct GenericArrayTag {
	GenericDataT** data;
	int length; // number of elements
	int size; // size currently allocated for the array
}GenericArrayT;

// define your functions here
GenericDataT* create_GenericDataT(int data);
GenericArrayT* create_empty_GenericArrayT();
int add_element_at_index(GenericArrayT* array, GenericDataT* element, int index);
int delete_element_at_index(GenericArrayT* array, int index);
GenericDataT* get_element_at_index(GenericArrayT* array, int index);
int search_element_in_array(GenericArrayT* array, GenericDataT* element, int (*compare)(GenericDataT* a, GenericDataT* b));
void print_elements_array(GenericArrayT* array);

#endif
