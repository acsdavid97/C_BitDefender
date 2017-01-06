/*
author: Acs David
file name: generic_array.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

typedef enum ReturnCodeEnum {
	SUCCESS, MEMORY_ALLOCATION_ERROR, INCORRECT_INDEX, COMPARE_FUNCTION_MISMATCH
}ReturnCodeE;

// structs
typedef struct GenericArrayTag {
	void** data;
	int length; // number of elements
	int size;	// size currently allocated for the array
}GenericArrayT;

// define your functions here

/*
	Creates an empty generic array.

	@return:  pointer to new generic array, if memory allocation failed returns NULL.
*/
GenericArrayT* create_empty_GenericArrayT();

/*
	Adds an element at the index specified, resizes array as needed

	@return SUCCESS: operation successful
	@return MEMORY_ALLOCATION_ERROR: resize operation failed, array is still preserved
	@return INCORRECT_INDEX: index supplied is not correct
*/
ReturnCodeE add_element_at_index(GenericArrayT* array, void* element, int index);

/*
	Deletes an element at the index specifed.

	@return SUCCESS: operation successful
	@return INCORRECT_INDEX: index supplied is not correct
*/
ReturnCodeE delete_element_at_index(GenericArrayT* array, int index, void(*free_generic_data)(void* generic_data));

/*
	Returns element at the index specifed.

	@return: pointer to the element, if index is out of bounds, returns NULL.
*/
void* get_element_at_index(GenericArrayT* array, int index);

/*
	Sets element at the index specifed.
*/
ReturnCodeE set_element_at_index(GenericArrayT* array, void* element,  int index);

/*
	Exchanges two elements at the indexes specified.

	@return INDEX_INCORRECT: one of the indexes is out of bounds
	@return SUCCESS: operation successful
*/
ReturnCodeE exchange_elements_at_indexes(GenericArrayT* array, int first_index, int second_index);

/*
	Searches for element in array, with the help of the function compare.

	@param compare: a pointer to a function which can compare two elements.

	@return: index of found element, -1 if item is not found.
*/

int search_element_in_array(GenericArrayT* array, void* element, int (*compare)(const void* a, const void* b));

/*
	Prints all the elements of the array, using the function print_element

	@param file: output to be written to. Should not be NULL, should be opened in write mode.
*/
void print_elements_in_array(GenericArrayT* array, FILE* file, void(*print_element)(const void *a, FILE* file));

/*
	Uses qsort to sort all the elements in the array.

	@param array: the arrray to be sorted

	@param compare: a pointer to a function which can compare two elements.
*/
void qsort_elements_in_array(GenericArrayT* array, int(*compare)(const void* a, const void* b));

/*
	Frees all elements inside the array.
	Sets length and size to zero.
	GenericArrayT* array should be freed afterwards.

	@param array: array from which all elements will be deleted.

	@param free_generic_data: a pointer to a function, which can free an instance of generic_data contained in the array.
*/
void delete_elements_in_array(GenericArrayT* array, void(*free_generic_data)(void* generic_data));

/*
	Merges two arrays. After a successful operation the source array can no longer access it's elements,
	needed to avoid data corruption(E.g.: Merge A and B, Delete B. A loses it's elements from B)

	@param array_destination: array where the merged array will be found after a successful operation.

	@param array_source: array from where data will be appened to array_destination. Should be freed after this operation.

	@return SUCCESS: operation successful.
	@return MEMORY_ALLOCATION_ERROR: array_destination could not be resized, everything remains as it was.
*/
ReturnCodeE merge_arrays(GenericArrayT* array_destination, GenericArrayT* array_source);

#endif
