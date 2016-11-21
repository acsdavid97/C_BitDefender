/*
author: Acs David
file name: longest_increasing_subsequence.h
file creation date: 2016-11-16
*/


#ifndef LONGEST_INCREASING_SUBSEQUENCE_H_GUARD
#define LONGEST_INCREASING_SUBSEQUENCE_H_GUARD

#include <stdio.h>
#include <stdlib.h>

typedef struct SequenceTag
{
	int *sequence;
	int length;
}SequenceT;

/*
@param sequence 
@return (one of) the longest increasing subsequence(s) from the sequence
*/
SequenceT* longest_increasing_subsequence(SequenceT *sequence);
/*
@return the sequence and it's length read from the user
*/
SequenceT* read_sequence();
void print_sequence(SequenceT *sequence);

#endif // !LONGEST_INCREASING_SUBSEQUENCE_H_GUARD
