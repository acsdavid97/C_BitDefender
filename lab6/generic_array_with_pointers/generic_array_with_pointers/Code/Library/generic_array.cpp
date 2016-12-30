#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: generic_array.cpp
file creation date: 2016-12-18 19:48:52
*/

#include "generic_array.h"

const int STARTING_ARRAY_SIZE = 16;

GenericArrayT* create_empty_GenericArrayT()
{
	GenericArrayT* empty_array = (GenericArrayT*)malloc(sizeof(GenericArrayT));
	if (empty_array == NULL)
	{
		return NULL;
	}

	empty_array->data = (void**)malloc(sizeof(void*) * STARTING_ARRAY_SIZE);
	if (empty_array->data == NULL)
	{
		free(empty_array);
		return NULL;
	}

	empty_array->length = 0;
	empty_array->size = STARTING_ARRAY_SIZE;

	return empty_array;
}

ReturnCodeE add_element_at_index(GenericArrayT* array, void* element, int index)
{
	//out of bounds
	if (index < 0 || index > array->length)
	{
		return INCORRECT_INDEX;
	}

	//resize array if needed(double the array size)
	if (array->length + 1 >= array->size - 1)
	{
		void** tmp = (void**)realloc(array->data, sizeof(void*) * array->size * 2);
		if (tmp == NULL)
		{
			return MEMORY_ALLOCATION_ERROR;
		}
		
		array->size *= 2;
		array->data = tmp;
	}

	//shift elements to the right
	for (int i = array->length; i > index; i--)
	{
		*(array->data + i) = *(array->data + i - 1);
	}

	//insert element
	*(array->data + index) = element;
	array->length++;

	return SUCCESS;
}

ReturnCodeE delete_element_at_index(GenericArrayT* array, int index, void(*free_generic_data)(void* generic_data))
{
	//out of bounds
	if (index < 0 || index >= array->length)
	{
		return INCORRECT_INDEX;
	}
	//free element
	free_generic_data(*(array->data + index));
	free(*(array->data + index));

	//shift elements to the left, overwriting the element
	for (int i = index; i < array->length; i++)
	{
		*(array->data + i) = *(array->data + i + 1);
	}
	array->length--;
	return SUCCESS;
}

void* get_element_at_index(GenericArrayT* array, int index)
{
	//out of bounds
	if (index < 0 || index >= array->length)
	{
		return NULL;
	}

	return *(array->data + index);
}

int search_element_in_array(GenericArrayT* array, void* element, int (*compare)(const void* a, const void* b))
{
	//iterate over array
	for(int i = 0; i < array->length; i++)
	{
		//compare element with ith element in array
		if (compare(*(array->data + i), element) == 0)
			return i;
	}
	return -1;
}

void print_elements_in_array(GenericArrayT* array, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	for (int i = 0; i < array->length; i++)
	{
		print_element(*(array->data + i), file);
	}
}

void qsort_elements_in_array(GenericArrayT* array, int(*compare)(const void* a, const void* b))
{
	qsort(array->data, array->length, sizeof(void*), compare);
}

void delete_elements_in_array(GenericArrayT* array, void(*free_generic_data)(void* generic_data))
{
	for (int i = 0; i < array->length; i++)
	{
		free_generic_data(*(array->data + i));
		free(*(array->data + i));
	}

	free(array->data);
	array->length = 0;
	array->size = 0;
}