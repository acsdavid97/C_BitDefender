#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: longest_increasing_subsequence.cpp
file creation date: 2016-11-16
*/

#include "longest_increasing_subsequence.h"


SequenceT* read_sequence()
{
	int aux;
	SequenceT *sequence = (SequenceT*)malloc(sizeof(SequenceT) );
	sequence->sequence = (int*)malloc(sizeof(int) * 100);
	int nr_elements = 0;
	do 
	{
		scanf("%d", &aux);
		*(sequence->sequence + nr_elements) = aux;
		nr_elements++;
	} while (aux >= 0); //read until a negative value is met
	sequence->sequence = (int*)realloc(sequence->sequence, sizeof(int) * nr_elements);
	sequence->length = nr_elements;

	return sequence;
}

SequenceT* longest_increasing_subsequence(SequenceT* sequence)
{
	SequenceT* sub_sequence = (SequenceT*)malloc(sizeof(SequenceT));
	sub_sequence->length = 0;
	int beginning = 0;
	int current = 0;
	int maxBeginning = 0;
	int maxEnd = 0;
	while (current < sequence->length - 1)
	{
		//end of increasing subsequence
		if (*(sequence->sequence + current) >= *(sequence->sequence + current + 1))
		{
			//longer sequence found
			if (current - beginning > maxEnd - maxBeginning)
			{
				maxEnd = current;
				maxBeginning = beginning;
			}
			//new beginnig of current sub sequence
			beginning = current + 1;
		}
		current++;
	}
	//preparing result
	sub_sequence->length = maxEnd - maxBeginning + 1;
	sub_sequence->sequence = (int*)malloc(sizeof(int)* sub_sequence->length);
	for (int i = 0; i < sub_sequence->length; i++)
	{
		*(sub_sequence->sequence + i) = *(sequence->sequence + i + maxBeginning);
	}
	
	return sub_sequence;
}

void print_sequence(SequenceT *sequence)
{
	for (int i = 0; i < sequence->length; i++)
	{
		printf("%d ", *(sequence->sequence + i));
	}
	printf("\n");
}