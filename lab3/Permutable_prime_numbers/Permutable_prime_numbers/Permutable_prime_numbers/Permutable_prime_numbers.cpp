/*
author: Acs David
file name: Permutable_prime_numbers.cpp
file creation date: 2016-11-20 23:26:11
*/

#include "Permutable_prime_numbers.h"

//returns the number of digits in a number
int get_nb_of_digits(int number)
{
	int nb_of_digits = 0;
	while (number != 0)
	{
		number /= 10;
		nb_of_digits++;
	}
	return nb_of_digits;
}

//shifts the digits of a number to the left circularly once
int permute_number_circular(int number)
{
	int nb_of_digits = get_nb_of_digits(number);
	int power = (int)pow(10, nb_of_digits - 1);
	int remainder = number % 10;
	number /= 10;
	number += remainder * power;

	return number;
}

//generates the seive of eratoshenes, 0 means it is prime, 1 means it is not prime
int* seive_of_eratosthenes(int max)
{
	//by default all elements are intialized to 0(PRIME)
	int *seive = (int*)calloc(max, sizeof(int));
	for (int i = 2; i <= sqrt(max); i++)
	{
		if (*(seive + i) == PRIME)
		{
			for (int j = 2*i; j < max; j += i)
			{
				*(seive + j) = NOT_PRIME;
			}
		}
	}

	return seive;
}


ListT* permutable_primes(int lower, int upper)
{
	
	//we have to check primes up until the next digit number, because 199 is prime and we have to check 911 too
	int power = pow(10, get_nb_of_digits(upper));
	int* seive = seive_of_eratosthenes(power);

	//most of the time 50 is enough
	int resultsMax = 50;
	ListT* results = (ListT*)malloc(sizeof(ListT));
	results->length = 0;
	int resultCounter = 0;
	results->elements = (int*)malloc(sizeof(int) * resultsMax);

	//check for all numbers if they are prime
	for (int i = lower; i < upper; i++)
	{
		int prime = PRIME;
		int number = i;
		for (int j = 0; j < get_nb_of_digits(i) && prime == PRIME; j++)
		{
			prime = *(seive + number);
			number = permute_number_circular(number);
		}
		if (prime == PRIME)
		{
			if (resultCounter >= resultsMax - 1)
			{
				results->elements = (int*)realloc(results->elements, sizeof(int) * resultsMax * 2);
			}
				 
			*(results->elements + resultCounter) = i;
			resultCounter++;
		}
	}

	//preparing result 
	results->elements = (int*) realloc(results->elements, sizeof(int) * resultCounter);
	results->length = resultCounter;

	return results;
}

//prints the results
void print_results(ListT* results)
{
	for (int i = 0; i < results->length; i++)
	{
		printf("%d, ", *(results->elements + i));
	}
}