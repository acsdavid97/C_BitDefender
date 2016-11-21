/*
author: Acs David
file name: Main.cpp
file creation date: 2016-11-16
*/

#include "longest_increasing_subsequence.h"
//TODO scanf not safe
int main()
{
	printf("This program will determine the longest increasing subsequnce from a user defined sequence\n");
	printf("Introduceti elementele vectorului: ");
	SequenceT *sequence = read_sequence();
	print_sequence(sequence);
	SequenceT *subsequence = longest_increasing_subsequence(sequence);
	

}