#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: fisier_text.cpp
file creation date: 2016-11-29 08:30:00
*/

#include "FISIER_TEXT.h"

//input file opened in read mode, output file open in write mode, none of them can be null
void reverse_character(FILE* input, FILE* output)
{
	char aux = ' ';
	char *pAux = &aux;
	fseek(input, 0, SEEK_END);
	long currentPos = ftell(input);
	while (currentPos > 0)
	{
		//prepare to read char
		fseek(input, -1, SEEK_CUR);
		fread(pAux, 1, sizeof(char), input);
		fwrite(pAux, 1, sizeof(char), output);

		//migitating the fact that line endings are two character long in windows
		if (*pAux == '\n')
		{
			fseek(input, -1, SEEK_CUR);
		}

		//jump one char forward
		fseek(input, -1, SEEK_CUR);
		currentPos = ftell(input);
	}
	
}

void reverse_lines(FILE* input, FILE* output)
{
	char aux = ' ';
	char *pAux = &aux;
	char buffer[1024];
	fseek(input, -1, SEEK_END);
	long currentPos = ftell(input);
	while (currentPos > 0)
	{
		//prepare to read char
		fseek(input, -1, SEEK_CUR);
		fread(pAux, 1, sizeof(char), input);

		//found a newline
		if (*pAux == '\n')
		{
			long lastpos = ftell(input);
			if(fgets(buffer, 1024, input))
				fputs(buffer, output);
			
			fseek(input, lastpos - 1, SEEK_SET);
		}

		//jump one char forward
		fseek(input, -1, SEEK_CUR);
		currentPos = ftell(input);
	}
	rewind(input);
	if (fgets(buffer, 1024, input))
	{
		fputs(buffer, output);
	}
}

int count_words(char *line)
{
	int nr_of_words = 0;
	char *aux = (char*)malloc(sizeof(char) * strlen(line));
	strcpy(aux, line);

	// space delimits words
	char *token = strtok(aux, " \n");
	while (token != NULL)
	{
		token = strtok(NULL, " \n");
		nr_of_words++;
	}

	return nr_of_words;
}

int count_chars(char *line)
{
	return strlen(line) - 1;
}

FileLineT* read_lines(FILE* input, int *length)
{
	char buffer[1024];

	//suppose file is not larger than 1024 lines
	FileLineT* lines = (FileLineT*)malloc(sizeof(FileLineT) * 1024);
	int count = 0;
	while (fgets(buffer, 1024, input))
	{
		(*(lines + count)).line = (char*)malloc(sizeof(char) * strlen(buffer));
		strcpy((*(lines + count)).line, buffer);
		(*(lines + count)).word_count = count_words(buffer);
		(*(lines + count)).char_count = count_chars(buffer);
		count++;
	}
	*length = count;

	// realloc to save space
	lines = (FileLineT*)realloc(lines, sizeof(FileLineT) * count);
	return lines;
}

int compare_str(const void *a, const void *b)
{
	return _stricmp(((FileLineT*)a)->line, ((FileLineT*)b)->line);
}

int compare_wrd(const void *a, const void *b)
{
	return ((FileLineT*)a)->word_count - ((FileLineT*)b)->word_count;
}

int compare_chr(const void *a, const void *b)
{
	return ((FileLineT*)a)->char_count - ((FileLineT*)b)->char_count;
}

void print_lines(FileLineT* lines, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf("%s word count: %d char count: %d \n", *(lines + i));
	}
}