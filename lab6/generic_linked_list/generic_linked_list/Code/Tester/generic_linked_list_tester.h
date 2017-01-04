/*
author: Acs David
file name: generic_linked_list_tester.h
file creation date: 2017-01-04 22:16:25
*/

#ifndef GENERIC_LINKED_LIST_TESTER_H
#define GENERIC_LINKED_LIST_TESTER_H

#include <string.h>
#include "../Library/generic_linked_list.h"
#include "generic_data.h"

const int BUFFER_SIZE = 1024;

//structs
typedef struct IOFilesTag {
	FILE* input;
	FILE* output;
}IOFilesT;

typedef enum ErrorCodeEnum {
	DATA_STRUCTURE_INEXISTENT, UNRECOGNIZED_COMMAND, TYPE_MISMATCH,
	MISSING_VALUE, INDEX_OUT_OF_BOUNDS, UNRECOGNIZED_HASH_FUNCTION,
	MEMORY_ALLOCATION_FAILED, ITEM_NOT_FOUND, STRUCTURE_IS_EMPTY,
	INSTANCE_UNKNOWN, DATA_STRUCTURE_OVERWRITTEN, UNKNOWN_ERROR_OCCURED
}ErrorCodeE;

typedef enum PossibleCommandsEnum {
	CREATE_LINKED_LIST, PRINT_LINKED_LIST, ADD_LINKED_LIST_ITEM,
	PUT_LINKED_LIST_ITEM, GET_LINKED_LIST_ITEM, DELETE_LINKED_LIST_ITEM,
	SEARCH_LINKED_LIST_ITEM, SORT_LINKED_LIST, MERGE_LINKED_LISTS,
	DELETE_LINKED_LIST, UNKOWN_COMMAND
}PossibleCommandsE;

// define your functions here

/*
	Opens input file(read mode), and results file(write mode).
	If opening any of them fail, the function returns NULL.

	@param file_name: file name without the extensions

	@return: IOFilesT* pointer to files, or NULL if fopen or memory allocation failed(rare).
*/
IOFilesT* create_IOFilesT(const char* file_name);

/*
	Closes files inside IOFilesT* files
*/
void close_IOFiles(IOFilesT* files);

/*
	copies input to output, only used for testing
*/
int copy_files(IOFilesT* files);

/*
	Reades command from files->input and writes corresponding output to files->output

	@param files: IOFilesT* structure, containing files

	@return: void
*/
void test_generic_linked_list(IOFilesT* files);

/*
	Prints an error to the file specified.

	@param error_code: defines which kind of error it is.

	@param message: used in some cases to make the error message more informative
*/
void print_error(ErrorCodeE error_code, FILE* file);

#endif