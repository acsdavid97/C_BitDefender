#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: expressions
file creation date: 2016-11-26 23:33:35
*/

#include "expressions.h"

int const BUFFER_SIZE = 256;


void print_menu()
{
	printf("\n------------------------------------------------\n");
	printf("1. get a line\n");
	printf("2. print sorted results\n");
	printf("3. write results to rezultate.dat binary file\n");
	printf("0. exit\n");
	printf("\n------------------------------------------------\n");
}

LinesT* read_file(FILE* input_file)
{
	char buffer[BUFFER_SIZE];
	int nr_of_lines = 0;
	//get number of lines
	while (fgets(buffer, BUFFER_SIZE, input_file))
	{
		nr_of_lines++;
	}

	//rewind and read the lines
	rewind(input_file);
	char **lines = (char**)malloc(sizeof(char*) * nr_of_lines);
	for (int i = 0; i < nr_of_lines; i++)
	{
		fgets(buffer, BUFFER_SIZE, input_file);
		*(lines + i) = (char*)malloc(sizeof(char) * strlen(buffer));
		strcpy(*(lines + i), buffer);
	}

	//return result
	LinesT* result = (LinesT*)malloc(sizeof(LinesT));
	result->lines = lines;
	result->nr_of_lines = nr_of_lines;
	return result;
}

void print_line(LinesT* lines, int nr_of_line)
{
	if (nr_of_line < lines->nr_of_lines && nr_of_line >= 0)
	{
		printf("%s\n", *(lines->lines + nr_of_line));
	}
	else
	{
		printf("line %d does not exist\n", nr_of_line);
	}
}

int get_nr_of_digits(int number)
{
	if (number == 0)
		return 0;

	int nr_of_digits = 0;
	while (number != 0)
	{
		nr_of_digits++;
		number /= 10;
	}
	return nr_of_digits;
}

int calculate_result(char *expresssion)
{
	int result = 0;
	int aux;
	char operation, test_end;
	int offset = 0;
	sscanf(expresssion + offset, "%d ", &result);
	
	//number offset
	offset += get_nr_of_digits(result);
	

	//scanf returns 1 if not at the end of list
	while (sscanf(expresssion + offset, "%c", &test_end) == 1)
	{
		//test_end offset
		offset += 1;
		
		sscanf(expresssion + offset, "%c", &operation);
		//char offset
		offset += 2;

		sscanf(expresssion + offset, "%d ", &aux);
		//number offset
		offset += get_nr_of_digits(aux);

		//decide which operation to perform
		switch (operation)
		{
			case '+':
				result += aux;
				break;
			case '-':
				result -= aux;
				break;
			default:
				//failsafe, if we cannot regonize the operation ->exit
				printf("what?");
				exit(2);
				break;
		}
	}
	return result;
}

ExpressionT* create_expression(char* expression)
{
	ExpressionT* created = (ExpressionT*)malloc(sizeof(ExpressionT));
	strcpy(created->expression, expression);
	created->result = calculate_result(expression);

	return created;
}


int compare(void const *a, void const *b)
{
	return ((ExpressionT*)a)->result - ((ExpressionT*)b)->result;
}

ExpressionListT* generate_expression_list(LinesT* lines)
{
	ExpressionListT* expressionlist = (ExpressionListT*)malloc(sizeof(ExpressionListT));
	expressionlist->length = lines->nr_of_lines * 2;
	// on average each line contains minimun 2 expressions
	expressionlist->list = (ExpressionT*)malloc(sizeof(ExpressionT) * (lines->nr_of_lines * 2));

	int nr_of_expressions = 0;

	for (int i = 0; i < lines->nr_of_lines; i++)
	{
		//seperate expressions by ';'
		char *tokenized = strtok(*(lines->lines + i), ";\n");
		while (tokenized != NULL)
		{
			//if we have a space at the beginning, skip it
			if (tokenized[0] == ' ') { tokenized++; }

			ExpressionT* to_add = create_expression(tokenized);
			
			if (nr_of_expressions >= expressionlist->length - 1)
			{
				//realloc with double size
				expressionlist->list = (ExpressionT*)realloc(expressionlist->list, sizeof(ExpressionT) * (expressionlist->length * 2));
			}

			//add to list
			*(expressionlist->list + nr_of_expressions) = *to_add;
			nr_of_expressions++;

			tokenized = strtok(NULL, ";\n");
		}
	}
	expressionlist->length = nr_of_expressions;
	expressionlist->list = (ExpressionT*)realloc(expressionlist->list, sizeof(ExpressionT) * nr_of_expressions);

	return expressionlist;
}

void print_expression_list(ExpressionListT* expressionlist)
{
	for (int i = 0; i < expressionlist->length; i++)
	{
		ExpressionT expression = *(expressionlist->list + i);
		printf("expression: %s result: %d\n", expression.expression, expression.result);
	}
}

void print_to_binary_file(ExpressionListT* expressionlist)
{
	FILE* f = fopen("rezultate.dat", "wb");
	if (f == NULL)
	{
		printf("could not open file, exiting");
		exit(1);
	}
	fwrite(expressionlist->list, sizeof(ExpressionT), expressionlist->length, f);
}