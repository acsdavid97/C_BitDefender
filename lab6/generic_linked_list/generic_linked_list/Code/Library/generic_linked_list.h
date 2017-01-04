/*
author: Acs David
file name: generic_linked_list
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_LINKED_LIST_H
#define GENERIC_LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef enum ReturnCodeEnum {
	SUCCESS, MEMORY_ALLOCATION_ERROR, INCORRECT_INDEX
}ReturnCodeE;

// structs
typedef struct NodeTag {
	void* data;
	struct NodeTag* next;
}NodeT;

typedef struct GenericLinkedListTag {
	NodeT* head;
	NodeT* tail;
	int length;
}GenericLinkedListT;

// define your functions here
NodeT* create_NodeT(void* data);

/*
	Creates an empty generic linked list.

	@return:  pointer to new generic linked list, if memory allocation failed returns NULL.
*/
GenericLinkedListT* create_empty_GenericLinkedListT();

/*
	Adds an element at the beginning of the linked list.

	@return SUCCESS: operation successful
*/
ReturnCodeE add_node_at_beginning(GenericLinkedListT* list, NodeT* node_to_insert);

/*
	Adds an element at the end of the linked list.

	@return SUCCESS: operation successful
*/
ReturnCodeE add_node_at_end(GenericLinkedListT* list, NodeT* node_to_insert);

/*
	Adds an element at the index specified.
	It is optimized for adding at the begginning and at the end.

	@return SUCCESS: operation successful
	@return INCORRECT_INDEX: index supplied is not correct
*/
ReturnCodeE add_node_at_index(GenericLinkedListT* list, NodeT* node_to_insert, int index);

/*
	Prints all the elements of the linked list, using the function print_element

	@param file: output to be written to. Should not be NULL, should be opened in write mode.
*/
void print_linked_list(GenericLinkedListT* list, FILE* file, void(*print_data)(const void* data, FILE* file));

/*
	Returns element at the index specifed.

	@return: pointer to the element, if index is out of bounds, returns NULL.
*/
NodeT* get_node_at_index(GenericLinkedListT* list, int index);

/*
	Removes an element from the beginning of the linked list.

	@return SUCCESS: operation successful
	@return INCORRECT_INDEX: list is empty
*/
ReturnCodeE remove_node_from_beginning(GenericLinkedListT* list);

/*
	Removes an element from the end of the linked list.

	@return SUCCESS: operation successful
	@return INCORRECT_INDEX: list is empty
*/
ReturnCodeE remove_node_from_end(GenericLinkedListT* list);

/*
	Deletes an element at the index specifed.
	It is optimized for deleting at the begginning and at the end.

	@return SUCCESS: operation successful
	@return INCORRECT_INDEX: index supplied is not correct
*/
ReturnCodeE remove_node_at_index(GenericLinkedListT* list, int index);

/*
	Searches for element in list, with the help of the function compare.

	@param compare: a pointer to a function which can compare two elements.

	@return: element found, otherwise NULL.
*/
void* search_element_in_list(GenericLinkedListT* list, void* element, int(*compare)(const void* a, const void *b));

/*
	Sorts elements in the linked list, with the help of the function compare

	@param compare: a pointer to a function which can compare two elements.
*/
void sort_elements_in_list(GenericLinkedListT* list, int(*compare)(const void* a, const void *b));

/*
	Merges two linked lists. After a successful operation the source list can no longer access it's elements,
	needed to avoid data corruption(E.g.: Merge A and B, Delete B. A loses it's elements from B)

	@param list_destination: list where the merged list will be found after a successful operation.

	@param list_source: list from where data will be appened to list_destination. Should be freed after this operation.

	@return SUCCESS: operation successful.
*/
void merge_linked_lists(GenericLinkedListT* list_destination, GenericLinkedListT* list_source);

/*
	Frees all elements inside the list.	Sets length to zero.
	GenericLinkedList* list should be freed afterwards.

	@param list: list from which all elements will be deleted.

	@param free_generic_data: a pointer to a function, which can free an instance of generic_data contained in the list.
*/
void delete_linked_list(GenericLinkedListT* list, void(*free_generic_data)(void* generic_data));

#endif
