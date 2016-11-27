/*
author: Acs David
file name: Permutable_prime_numbers.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef PERMUTABLE_PRIME_NUMBERS_H
#define PERMUTABLE_PRIME_NUMBERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// structs or enums

typedef struct ListTag
{
	int length;
	int *elements;
}ListT;

enum ENUM_PRIME { PRIME, NOT_PRIME };

// define your functions here
int permute_number_circular(int number);
int* seive_of_eratosthenes(int max);
int get_nb_of_digits(int number);
ListT* permutable_primes(int lower, int upper);
void print_results(ListT* results);

#endif
