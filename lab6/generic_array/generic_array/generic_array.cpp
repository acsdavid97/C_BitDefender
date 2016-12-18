#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: generic_array.cpp
file creation date: 2016-12-18 19:48:52
*/

#include "generic_array.h"

const int STARTING_ARRAY_SIZE = 16;

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

GenericArrayT* create_empty_GenericArrayT()
{
	GenericArrayT* empty_array = (GenericArrayT*)malloc(sizeof(GenericArrayT));
	empty_array->data = (GenericDataT**)malloc(sizeof(GenericDataT*) * STARTING_ARRAY_SIZE);
	empty_array->length = 0;
	empty_array->size = STARTING_ARRAY_SIZE;

	return empty_array;
}

int add_element_at_index(GenericArrayT* array, GenericDataT* element, int index)
{
	//out of bounds
	if (index < 0 || index > array->length)
	{
		return 1;
	}

	//resize array if needed
	if (array->length + 1 >= array->size - 1)
	{
		GenericDataT** tmp = (GenericDataT**)realloc(array->data, sizeof(GenericDataT*) * array->size * 2);
		if (tmp != NULL)
		{
			array->size *= 2;
			array->data = tmp;
		}
		else
		{
			return 0;
		}
	}

	//shift elements to the right
	for (int i = array->length; i > index; i--)
	{
		*(array->data + i) = *(array->data + i - 1);
	}

	*(array->data + index) = element;
	array->length++;

	return 1;
}

int delete_element_at_index(GenericArrayT* array, int index)
{
	//shift elements to the left, overwriting the element
	for (int i = index; i < array->length; i++)
	{
		*(array->data + i) = *(array->data + i + 1);
	}
	array->length--;
	return 1;
}

GenericDataT* get_element_at_index(GenericArrayT* array, int index)
{
	return *(array->data + index);
}

int search_element_in_array(GenericArrayT* array, GenericDataT* element, int (*compare)(GenericDataT* a, GenericDataT* b))
{
	for(int i = 0; i < array->length; i++)
	{
		if (compare(*(array->data + i), element) == 0)
			return i;
	}
	return -1;
}

void print_elements_array(GenericArrayT* array)
{
	for (int i = 0; i < array->length; i++)
	{
		print_GenericDataT(*(array->data + i));
	}
}