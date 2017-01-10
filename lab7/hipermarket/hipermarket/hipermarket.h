/*
author: Acs David
file name: hipermarket.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef HIPERMARKET_H
#define HIPERMARKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../lab6/generic_hash_table/generic_hash_table/Code/Library/generic_hash_table.h"

// structs
typedef struct ProductTag {
	char* name;
	double price;
}ProductT;

typedef struct HipermarketTag {
	char* name;
	GenericHashTableT* price_of_items; // char to double 
}HipermarketT;

// define your functions here
double get_hipermarket_price(HipermarketT* hipermarket, GenericLinkedListT* items_to_buy);
ProductT* create_ProductT(char* name, double price);
int compare_ProductT(const void* a, const void* b);
unsigned int hash_ProductT(const void* a);
void print_ProductT(const void* a, FILE* file);
HipermarketT* create_HipermarketT(char* name);
void free_HipermarketT(void* hipermarket);

#endif
