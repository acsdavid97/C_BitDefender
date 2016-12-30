#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: generic_data_type
file creation date: 2016-12-20 10:40:00
*/

#include "generic_data_type.h"

GenericDataT* create_GenericDataT(int data)
{
	GenericDataT* new_GenericDataT = (GenericDataT*)malloc(sizeof(GenericDataT));
	new_GenericDataT->data = data;

	return new_GenericDataT;
}

void print_GenericDataT(GenericDataT* element)
{
	printf("%d\n", element->data);
}
