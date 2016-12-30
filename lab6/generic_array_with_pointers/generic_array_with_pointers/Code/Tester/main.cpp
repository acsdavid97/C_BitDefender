#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-12-18 19:48:35
*/

#include "generic_array_tester.h"

int main(int argc, char* args[])
{
	if (argc < 2)
	{
		printf("not enough arguments.\n");
		printf("Usage:\n");
		printf("tester.exe <test_file_number> -- runs test file test_file_number.in, outputs to test_file_number.out\n");
		printf("tester.exe <test_file_number1> <test_file_number2> -- runs test file test_file_number1.in and test_file_number2.in, outputs to test_file_number1.out and test_file_number2.out\n");
		printf("tester.exe runall -- runs all the test files in the input folder and creates for each of them a corresponding output file\n");
		printf("exiting...");
		exit(1);
	}

	//NOTE: discards all other command line arguments
	if (strcmp(args[1], "runall") == 0)
	{
		//TODO: impement runall
		printf("run all input files\n");
		exit(0);
	}
	for (int i = 1; i < argc; i++)
	{
		IOFilesT* files = create_IOFilesT(args[i]);

		//skip if one files could not be opened
		if (files == NULL)
		{
			printf("skipping %s !\n", args[i]);
			continue;
		}

		test_generic_array(files);
		close_IOFiles(files);
		free(files);
	}

	system("pause");
	return 0;

}