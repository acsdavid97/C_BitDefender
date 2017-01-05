/*
author: Acs David
file name: generic_hash_table.h
file creation date: 2017-01-04 23:26:24
*/

#ifndef GENERIC_HASH_TABLE_H
#define GENERIC_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>

#include "generic_linked_list.h"
#include "../Tester/generic_data.h"
#include <math.h>

const double MAX_LOAD_FACTOR = 0.75;
const int STARTING_TABLE_SIZE = 17;

// structs
typedef struct GenericHashTableTag {
	GenericLinkedListT** array_of_lists;
	int nr_of_elements; // number of elements
	int size;	// size currently allocated for the hash table
	unsigned int(*hash_function)(const void*);
}GenericHashTableT;

// define your functions here

/*
	Creates an empty generic hash table.

	@return:  pointer to new generic hash table, if memory allocation failed returns NULL.
*/
GenericHashTableT* create_empty_GenericHashTableT();

/*
	Returns the hash of element, using the hash function contained in table.
	If no hash function is found in table, uses the pointer values of element.

	@param table: hash table
	@param element: element to be hashed

	@return: hash of element
*/
unsigned int get_hash(GenericHashTableT* table, void* element);

/*
	Adds an element, resizes hash table as needed

	@return SUCCESS: operation successful
	@return MEMORY_ALLOCATION_ERROR: resize operation failed, hash table is still preserved
*/
ReturnCodeE add_element_in_hash_table(GenericHashTableT* hash_table, void* element);

/*
	Deletes an element from the hash table.

	@return pointer to deleted element, otherwise NULL.
*/
void* delete_element_from_hash_table(GenericHashTableT* hash_table, void* element, int(*compare)(const void* a, const void* b));

/*
	Searches for element in hash table, with the help of the function compare.

	@param compare: a pointer to a function which can compare two elements.

	@return: pointer to element found, otherwise NULL.
*/
void* search_element_in_hash_table(GenericHashTableT* hash_table, void* element, int (*compare)(const void* a, const void* b));

/*
	Prints all the elements of hash_table, using the function print_element

	@param file: output to be written to. Should not be NULL, should be opened in write mode.
*/
void print_elements_in_hash_table(GenericHashTableT* hash_table, FILE* file, void(*print_element)(const void *a, FILE* file));

/*
	Frees all elements inside the hash table.
	Sets nr_of_elements and size to zero.
	GenericHashTableT* hash_table should be freed afterwards.

	@param hash_table: hash table from which all elements will be deleted.

	@param free_generic_data: a pointer to a function, which can free an instance of generic_data contained in the hash table.
*/
void delete_elements_in_hash_table(GenericHashTableT* hash_table, void(*free_generic_data)(void* generic_data));

/*
	Rehashes the table with the specified function. If the operation is successful,
	the rehashed table will be in place of the original one.

	@param hash_table: the table to be rehashed
	@param hash_function: pointer to a function, which can hash an element

	@return TODO: too lazy now.
*/
ReturnCodeE rehash_table(GenericHashTableT* hash_table,unsigned int new_size, unsigned int(*hash_function)(const void* element));
#endif
