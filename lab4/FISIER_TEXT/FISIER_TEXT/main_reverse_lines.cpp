#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-11-29 08:29:19
*/

#include "FISIER_TEXT.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("incorrect usage, program inputfile outputfile");
		exit(1);
	}

	FILE* input = fopen(argv[1], "r");
	if (input == NULL)
	{
		printf("input file could not be open");
		exit(1);
	}
	FILE* output = fopen(argv[2], "w");
	if (input == NULL)
	{
		printf("output file could not be open");
		exit(1);
	}

	reverse_lines(input, output);

	system("pause");
	return 0;
}