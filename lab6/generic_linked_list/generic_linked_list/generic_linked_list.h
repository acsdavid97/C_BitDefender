/*
author: Acs David
file name: generic_linked_list
file creation date: 2016-11-20 23:26:24
*/

#ifndef generic_linked_list_H
#define generic_linked_list_H

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct GenericDataTag {
	char* surname;
	char* name;
	char* address;
	char* city;
	char* telephone;
	char* email;
}GenericDataT;

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
GenericDataT* create_GenericDataT(char* surname, char* name, char* address, char* city, char* telephone, char* email);
void print_GenericDataT(GenericDataT* data);
NodeT* create_NodeT(GenericDataT* data);
GenericLinkedListT* create_empty_GenericLinkedListT();
int add_node_at_end(GenericLinkedListT* list, NodeT* node_to_insert);
void print_linked_list(GenericLinkedListT* list, void(*print_data)(GenericDataT* data));
int remove_node_from_end(GenericLinkedListT* list);
NodeT* get_node_at_index(GenericLinkedListT* list, int index);
int remove_node_at_index(GenericLinkedListT* list, int index);

#endif
