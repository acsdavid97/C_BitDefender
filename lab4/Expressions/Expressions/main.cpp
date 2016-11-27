#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2016-11-26 23:32:28
*/

#include "expressions.h"

int main()
{
	FILE* f = fopen("expresii.txt", "r");
	if (f == NULL)
	{
		printf("file could not be opened, exiting");
		exit(1);

	}
	printf("reading from file...\n");
	LinesT* lines = read_file(f);
	fclose(f);

	printf("genarating expression list...\n");
	ExpressionListT* expressionlist = generate_expression_list(lines);

	//sort
	printf("sorting list...\n");
	qsort(expressionlist->list, expressionlist->length, sizeof(ExpressionT), compare);

	char choice = '1';
	int aux;

	while ( choice != '0')
	{
		print_menu();
		printf("choose: ");
		choice = getchar();
		switch (choice)
		{
		case '1':
			printf("enter a line number:");
			scanf("%d", &aux);
			print_line(lines, aux);
			break;

		case '2':
			print_expression_list(expressionlist);
			break;
		case '3':
			print_to_binary_file(expressionlist);
			break;
		case '0':
			printf("exiting...\n");
			break;
		default:
			printf("invalid choice...\n");
			break;
		}
		getchar();
	}

	system("pause");
	return 0;
}