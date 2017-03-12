/*
author: Acs David
file name: file_analizer
file creation date: 2016-11-20 23:26:24
*/

#ifndef FILE_ANALIZER_H
#define FILE_ANALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
typedef struct WordTag {
	char *word;
	int nr_of_apparitions;
}WordT;

typedef struct WordListTag {
	WordT* wordlist;
	int nr_of_elements;
}WordListT;


// define your functions here
WordListT* analize_file(char* file_name);
void add_word(WordListT* wordlist, char* word);
void print_wordlist(WordListT* wordlist);
void print_word(WordT word);
int get_index(WordListT* wordlist, char* word);
int cmp_word(const void *a, const void *b);
void print_last_n(WordListT* wordlist, int n);
WordListT* concat_wordlists(WordListT *a, WordListT *b);

#endif
