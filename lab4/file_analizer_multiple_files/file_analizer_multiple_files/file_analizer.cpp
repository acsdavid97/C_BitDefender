#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: file_analizer.cpp
file creation date: 2016-12-06 21:03:29
*/

#include "file_analizer.h"

const int BUFFER_SIZE = 1024;

WordListT* analize_file(char* file_name)
{
	FILE* finput = fopen(file_name, "r");
	if (finput == NULL)
	{
		perror("no such file");
		exit(1);
	}
	WordListT* wordlist = (WordListT*)malloc(sizeof(WordListT));
	wordlist->nr_of_elements = 0;
	wordlist->wordlist = (WordT*)malloc(sizeof(WordT));

	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, finput))
	{
		char *token = strtok(buffer, " \n");
		while (token != NULL)
		{
			int index = get_index(wordlist, token);
			if (index == -1)
			{			
				char *token_copy = (char*)malloc(sizeof(char) * strlen(token));
				strcpy(token_copy, token);
				add_word(wordlist, token_copy);
			}
			else
			{
				(*(wordlist->wordlist + index)).nr_of_apparitions++;
			}

			token = strtok(NULL, " \n");
		}
	}
	return wordlist;
}

int get_index(WordListT* wordlist, char* word)
{
	for (int i = 0; i < wordlist->nr_of_elements; i++)
	{
		char *current_word = (*(wordlist->wordlist + i)).word;
		if (strcmp(current_word, word) == 0)
			return i;
	}
	return -1;

}

void add_word(WordListT* wordlist, char* word)
{
	wordlist->nr_of_elements++;
	wordlist->wordlist = (WordT*)realloc(wordlist->wordlist, sizeof(WordT) * (wordlist->nr_of_elements + 1));
	(*(wordlist->wordlist + wordlist->nr_of_elements - 1)).word = word;
	(*(wordlist->wordlist + wordlist->nr_of_elements - 1)).nr_of_apparitions = 1;
}

void print_wordlist(WordListT* wordlist)
{
	for (int i = 0; i < wordlist->nr_of_elements; i++)
	{
		print_word(*(wordlist->wordlist + i));
	}
}

void print_word(WordT word)
{
	printf("%s - %d\n", word.word, word.nr_of_apparitions);
}

int cmp_word(const void *a, const void *b)
{
	return ((WordT*)a)->nr_of_apparitions - ((WordT*)b)->nr_of_apparitions;
}

void print_last_n(WordListT* wordlist, int n)
{
	for (int i = wordlist->nr_of_elements - 1; i > wordlist->nr_of_elements - n - 1 && i >= 0; i--)
	{
		print_word(*(wordlist->wordlist + i));
	}
}

WordListT* concat_wordlists(WordListT *a, WordListT *b)
{
	WordListT* result = (WordListT*)malloc(sizeof(WordListT));
	result->nr_of_elements = a->nr_of_elements + b->nr_of_elements;
	result->wordlist = (WordT*)malloc(sizeof(WordT) * result->nr_of_elements);
	for (int i = 0; i < a->nr_of_elements; i++)
	{
		*(result->wordlist + i) = *(a->wordlist + i);
	}
	for (int i = a->nr_of_elements; i < result->nr_of_elements; i++)
	{
		*(result->wordlist + i) = *(b->wordlist + i);
	}

	return result;
}