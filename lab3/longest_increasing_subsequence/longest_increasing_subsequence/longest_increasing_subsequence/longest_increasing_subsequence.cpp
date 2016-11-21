/*
author: Acs David
file name: longest_increasing_subsequence.cpp
file creation date: 2016-11-16
*/

#include "longest_increasing_subsequence.h"

SequenceT* longest_increasing_subsequence(SequenceT *sequence)
{
	
}

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
	} while (aux >= 0);
	sequence->sequence = (int*)realloc(sequence->sequence, sizeof(int) * nr_elements);
}

void print_sequence(SequenceT *sequence)
{
	for (int i = 0; i < sequence->length; i++)
	{
		printf("%d ", *(sequence->sequence + i));
	}
	printf("\n");
}