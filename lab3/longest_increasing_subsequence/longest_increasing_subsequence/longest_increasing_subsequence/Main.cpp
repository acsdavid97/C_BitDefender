/*
author: Acs David
file name: Main.cpp
file creation date: 2016-11-16
*/

#include "longest_increasing_subsequence.h"
int main()
{
	printf("This program will determine the longest increasing subsequnce from a user defined sequence\n");
	printf("Introduceti elementele vectorului, un element negativ semnifica finalul vectorului: ");
	SequenceT *sequence = read_sequence();
	SequenceT *subsequence = longest_increasing_subsequence(sequence);
	printf("Secventa cea mai lunga ordonata crescator este: ");
	print_sequence(subsequence);
	
	system("pause");
	return 0;
}