/*
author: Acs David
file name: generic_heap.h
file creation date: 2017-01-04 23:26:24
*/

#ifndef GENERIC_HEAP_H
#define GENERIC_HEAP_H

#include <stdio.h>
#include <stdlib.h>

#include "../Tester/generic_data.h"
#include "generic_array.h"

// structs
typedef struct GenericHeapTag {
	GenericArrayT* array; //array, where the elements are stored.
}GenericHeapT;

// define your functions here

/*
	Creates an empty generic heap.

	@return:  pointer to new generic heap, if memory allocation failed returns NULL.
*/
GenericHeapT* create_empty_GenericHeapT();

/*
	Adds an element, resizes heap as needed

	@return SUCCESS: operation successful
	@return MEMORY_ALLOCATION_ERROR: resize operation failed, heap is still preserved
*/
ReturnCodeE add_element_in_heap(GenericHeapT* heap, void* element, int(*compare)(const void* a, const void* b));

/*
	Returns the top element in the heap (usually min or max).

	@return: pointer to top element, if heap is empty returns NULL.
*/
void* get_heap_top(GenericHeapT* heap);

/*
	Deletes and returns the top element in the heap (usually min or max).

	@return: pointer to top element, if heap is empty returns NULL.
*/
void* delete_heap_top(GenericHeapT* heap);

/*
	Deletes an element from the heap.

	@return pointer to deleted element, otherwise NULL.
*/
void* delete_element_from_heap(GenericHeapT* heap, void* element, int(*compare)(const void* a, const void* b));

/*
	Prints all the elements of heap in tree format, using the function print_element

	@param file: output to be written to. Should not be NULL, should be opened in write mode.
*/
void print_elements_in_heap(GenericHeapT* heap, FILE* file, void(*print_element)(const void *a, FILE* file));


/*
	Merges two heaps. 

	@param heap_destination: heap where the merged heap will be found after a successful operation.

	@param heap_source: heap from where data will be added to heap_destination. Should be freed after this operation.

	@return SUCCESS: operation successful.
	@return MEMORY_ALLOCATION_ERROR: could not resize heap to hold all elements.
	Everything remains as it was before attempting the operation
*/
ReturnCodeE merge_heaps(GenericHeapT* heap_destination, GenericHeapT* heap_source);

/*
	Frees all elements inside the heap.
	Sets nr_of_elements and size to zero.
	GenericHeapT* heap should be freed afterwards.

	@param heap: heap from which all elements will be deleted.

	@param free_generic_data: a pointer to a function, which can free an instance of generic_data contained in the heap.
*/
void delete_elements_in_heap(GenericHeapT* heap, void(*free_generic_data)(void* generic_data));

#endif
