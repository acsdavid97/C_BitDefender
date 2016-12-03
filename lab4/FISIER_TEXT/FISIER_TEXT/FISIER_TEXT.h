/*
author: Acs David
file name: FISIER_TEXT.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef FISIER_TEXT_H
#define FISIER_TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
typedef struct FileLineTag {
	char *line;
	int word_count;
	int char_count;
}FileLineT;

// define your functions here
void reverse_character(FILE* input, FILE* output);
void reverse_lines(FILE* input, FILE* output);
int count_words(char *line);
FileLineT* read_lines(FILE* input, int *length);
int compare_wrd(const void *a, const void *b);
int compare_chr(const void *a, const void *b);
int compare_str(const void *a, const void *b);
void print_lines(FileLineT* lines, int length);


#endif
