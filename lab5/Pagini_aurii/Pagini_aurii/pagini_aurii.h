/*
author: Acs David
file name: pagini_aurii.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef PAGINI_AURII_H
#define PAGINI_AURII_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
typedef struct EntryTag {
	char *name;
	char *surname;
	char *telephone; //not a numerical value, since can contain special characters, e.g.: -
	char *address;
}EntryT;

typedef struct NodeTag {
	EntryT* entry;
	struct NodeTag *left;
	struct NodeTag *right;
}NodeT;

// define your functions here
EntryT* create_EntryT(char *name, char *surname, char *telephone, char *address);
void print_EntryT(EntryT* node);
NodeT* insert(NodeT* toinsert, NodeT* node);
NodeT* create_NodeT(EntryT* entry);
void printtree(NodeT* node);
NodeT* delete_node_wraper(NodeT* root, char *name);
NodeT* find(char *name, NodeT* root);
NodeT* search_by_telephone_number(NodeT* root, char *telephone);
void print_menu();
NodeT* read_entry();
char* print_read(char *to_print);
void print_all_with_same_surname(NodeT* node, char *surname);

#endif
