/*
author: Acs David
file name: trie_module.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef TRIE_MODULE_H
#define TRIE_MODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum ReturnCodeEnum {
	SUCCESS, NOT_SUCCESS
}ReturnCodeE;

// structs
typedef struct TrieNodeTag {
	struct TrieNodeTag* child;
	struct TrieNodeTag* brother;
	char element;
	bool word_end;
}TrieNodeT;

// define your functions here
TrieNodeT* create_TrieNodeT(char element);
void add_word_in_trie(TrieNodeT* subtrie_root, char* word_to_add);
int search_word_in_trie(TrieNodeT* subtrie_root, char* word_to_search);
int get_nr_of_words(TrieNodeT* subtrie_root);

#endif
