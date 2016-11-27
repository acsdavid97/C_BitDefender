/*
author: Acs David
file name: main.cpp
file creation date: 2016-11-20 23:27:51
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Permutable_prime_numbers.h"

int main()
{

	int lower = 0, upper = 0;
	printf("enter lower bound of interval(must be bigger than 1): ");
	scanf("%d", &lower);
	printf("enter upper bound of interval(must be bigger than lower): ");
	scanf("%d", &upper);
	//TODO check user input

	ListT* results = permutable_primes(lower, upper);
	print_results(results);

	system("pause");
	return 0;
}