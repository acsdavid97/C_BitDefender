#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_hash_table.cpp
file creation date: 2016-12-18 19:48:52
*/

#include "generic_heap.h"

GenericHeapT* create_empty_GenericHeapT()
{
	GenericHeapT* empty_heap = (GenericHeapT*)malloc(sizeof(GenericHeapT));
	if (empty_heap == NULL)
	{
		return NULL;
	}

	empty_heap->array = create_empty_GenericArrayT();
	if (empty_heap->array == NULL)
	{
		free(empty_heap);
		return NULL;
	}

	return empty_heap;
}


ReturnCodeE add_element_in_heap(GenericHeapT* heap, void* element, int(*compare)(const void* a, const void* b))
{
	//TODO: implement
}

void* get_heap_top(GenericHeapT* heap)
{
	//TODO: implement
}

void* delete_heap_top(GenericHeapT* heap)
{
	//TODO: implement
}

void* delete_element_from_heap(GenericHeapT* heap, void* element, int(*compare)(const void* a, const void* b))
{
	void* deleted_element = NULL;
	//TODO: implement
	return deleted_element;
}

void print_elements_in_heap(GenericHeapT* heap, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	//TODO: implement
}

ReturnCodeE merge_heaps(GenericHeapT* heap_destination, GenericHeapT* heap_source)
{
	//TODO: implement
}

void delete_elements_in_heap(GenericHeapT* heap, void(*free_generic_data)(void* generic_data))
{
	//TODO: implement
}

