#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-11-29 08:29:19
*/

#include "FISIER_TEXT.h"

int main()
{
	FILE* input = fopen("input.txt", "r");
	if (input == NULL)
	{
		printf("input file could not be open");
		exit(1);
	}
	FILE* output = fopen("output.txt", "w");
	if (input == NULL)
	{
		printf("output file could not be open");
		exit(1);
	}

//	reverse_character(input, output);
//	reverse_lines(input, output);
	int length = 0;
	FileLineT* lines = read_lines(input, &length);
	printf("unsorted \n\n-------------");
	print_lines(lines, length);

	qsort(lines, length, sizeof(FileLineT), compare_str);
	printf("sorted by alphabetically \n\n-------------");
	print_lines(lines, length);

	qsort(lines, length, sizeof(FileLineT), compare_wrd);
	printf("sorted by word count \n\n-------------");
	print_lines(lines, length);

	qsort(lines, length, sizeof(FileLineT), compare_chr);
	printf("sorted by char count \n\n-------------");
	print_lines(lines, length);
	system("pause");
	return 0;
}