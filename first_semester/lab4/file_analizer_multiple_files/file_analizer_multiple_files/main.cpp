#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-12-06 21:03:16
*/

#include "file_analizer.h"

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("not enough arguments...\n exiting\n");
		exit(1);
	}
	WordListT* merged = (WordListT*)malloc(sizeof(WordListT));
	merged->nr_of_elements = 0;
	for (int i = 1; i < argc; i++)
	{
		WordListT* wordlist = analize_file(argv[i]);
		merged = concat_wordlists(merged, wordlist);
		print_wordlist(merged);
		free(wordlist);

	}
	//qsort(merged->wordlist, merged->nr_of_elements, sizeof(WordT), cmp_word);
	print_wordlist(merged);
	int n = 0;
	printf("file analized, words sorted, enter n: ");
	scanf("%d", &n);
	print_last_n(merged, n);
	system("pause");
	return 0;
}