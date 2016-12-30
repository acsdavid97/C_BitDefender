#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: generic_data
file creation date: 2016-12-28 18:39:33
*/

#include "generic_data.h"

GenericDataT* create_GenericDataT(int number)
{
	GenericDataT* data = (GenericDataT*)malloc(sizeof(GenericDataT));
	if (data == NULL)
	{
		return NULL;
	}
	
	data->number = number;
	return data;
}

void print_GenericDataT(const void* generic_data, FILE* file) 
{
	fprintf(file, "%d ", ((GenericDataT*)generic_data)->number);
}

int compare_GenericDataT(const void* a, const void* b)
{
	return ((GenericDataT*)a)->number - ((GenericDataT*)b)->number;
}

void* read_and_create_GenericDataT(FILE* file)
{
	int aux = 0;
	fscanf(file, "%d", &aux);
	return create_GenericDataT(aux);
}

void free_GenericDataT(void* generic_data_to_delete)
{
	return;//no need to free anything, since generic_data_to_delete contains
	//		 onlty primitive types.
}