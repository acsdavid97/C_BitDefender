#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-12-06 21:03:16
*/

#include "file_analizer.h"

int main()
{
	WordListT* wordlist = analize_file("test.txt");
	qsort(wordlist->wordlist, wordlist->nr_of_elements, sizeof(WordT), cmp_word);
	int n = 0;
	printf("file analized, words sorted, enter n: ");
	scanf("%d", &n);
	print_last_n(wordlist, n);
	system("pause");
	return 0;
}