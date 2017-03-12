/*
author: Acs David
file name: generic_linked_list
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_LINKED_LIST_H
#define GENERIC_LINKED_LIST_H

#include "generic_data_type.h"

// structs
typedef struct NodeTag {
	GenericDataT* data;
	struct NodeTag* next;
}NodeT;

typedef struct GenericLinkedListTag {
	NodeT* head;
	NodeT* tail;
	int length;
}GenericLinkedListT;

// define your functions here
NodeT* create_NodeT(GenericDataT* data);
GenericLinkedListT* create_empty_GenericLinkedListT();
int add_node_at_beginning(GenericLinkedListT* list, NodeT* node_to_insert);
int add_node_at_end(GenericLinkedListT* list, NodeT* node_to_insert);
int add_node_at_index(GenericLinkedListT* list, NodeT* node_to_insert, int index);
void print_linked_list(GenericLinkedListT* list, void(*print_data)(GenericDataT* data));
int remove_node_from_beginning(GenericLinkedListT* list);
int remove_node_from_end(GenericLinkedListT* list);
int remove_node_at_index(GenericLinkedListT* list, int index);
NodeT* get_node_at_index(GenericLinkedListT* list, int index);
int search_element_in_list(GenericLinkedListT* list, GenericDataT* element, int (*compare)(GenericDataT* a, GenericDataT* b));

#endif
