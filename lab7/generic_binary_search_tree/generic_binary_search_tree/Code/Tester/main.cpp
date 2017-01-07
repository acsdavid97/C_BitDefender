#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: main.cpp
file creation date: 2017-01-04 22:16:25
*/

#include "generic_binary_search_tree_tester.h"
#include "dirent.h"
/*
NOTE: I did not found a cross-platform solution to traverse directories.
On Windows you should copy dirent.h to the include folders.
Follow this link for more info: https://github.com/tronkko/dirent
*/

void runall(const char* dirname)
{
	// Open directory stream.
	DIR* dir = opendir(dirname);
	if (dir != NULL) {

		struct dirent *ent;
		// Iterate through all files and directories within the directory.
		while ((ent = readdir(dir)) != NULL)
		{
			// If it's a file
			if (ent->d_type == DT_REG)
			{
				// Suppose only one dot in filename, right before the extension.
				// Suppose only files with .in extention are in the folder.
				char* file_name = strtok(ent->d_name, ".");
				IOFilesT* files = create_IOFilesT(file_name);

				// Skip if one of the files could not be opened.
				if (files == NULL)
				{
					printf("skipping %s !\n", file_name);
					continue;
				}

				test_generic_binary_search_tree(files);
				close_IOFiles(files);
				free(files);
			}
		}

		closedir(dir);
	}
	else {
		// Could not open directory.
		printf("Cannot open directory %s\n", dirname);
	}
}

int main(int argc, char* args[])
{
	if (argc < 2)
	{
		printf("not enough arguments.\n");
		printf("Usage:\n");
		printf("tester.exe <test_file_number> -- runs test file test_file_number.in, outputs to test_file_number.out\n");
		printf("tester.exe <test_file_number1> <test_file_number2> -- runs test file test_file_number1.in and test_file_number2.in, outputs to test_file_number1.out and test_file_number2.out\n");
		printf("tester.exe runall -- runs all the test files in the input folder and creates for each of them a corresponding output file\n");
		printf("exiting...\n");
		system("pause");
		exit(1);
	}

	//NOTE: Discards all other command line arguments.
	if (strcmp(args[1], "runall") == 0)
	{
		printf("running all input files!\n");
		runall("Input/");
		system("pause");
		exit(0);
	}

	//for each argument, append extensions, open files, run test.
	for (int i = 1; i < argc; i++)
	{
		IOFilesT* files = create_IOFilesT(args[i]);

		// Skip if one of the files could not be opened.
		if (files == NULL)
		{
			printf("skipping %s !\n", args[i]);
			continue;
		}

		test_generic_binary_search_tree(files);
		close_IOFiles(files);
		free(files);
	}

	system("pause");
	return 0;

}