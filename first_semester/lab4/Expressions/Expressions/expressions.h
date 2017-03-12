/*
author: Acs David
file name: expressions
file creation date: 2016-11-20 23:26:24
*/

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
typedef struct LinesTag {
	char **lines;
	int nr_of_lines;
}LinesT;

typedef struct ExpressionTag {
	//suppose every expression is maximum 32 characters
	char expression[32];
	int result;

}ExpressionT;

typedef struct ExpressionListTag {
	ExpressionT *list;
	int length;
}ExpressionListT;

// define your functions here

LinesT* read_file(FILE* input_file);
void print_line(LinesT* lines, int nr_of_line);
ExpressionListT* generate_expression_list(LinesT* lines);
void print_expression_list(ExpressionListT* expressionlist);
int compare(void const *a, void const *b);
void print_menu();
void print_to_binary_file(ExpressionListT* expressionlist);

#endif
