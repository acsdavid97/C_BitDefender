#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: hipermarket.cpp
file creation date: 2017-01-10 09:38:04
*/

#include "hipermarket.h"

ProductT* create_ProductT(char* name, double price)
{
	ProductT* new_product = (ProductT*)malloc(sizeof(ProductT));

	new_product->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(new_product->name, name);
	new_product->price = price;

	return new_product;
}
int compare_ProductT(const void* a, const void* b)
{
	ProductT* aa = (ProductT*)a;
	ProductT* bb = (ProductT*)b;

	return strcmp(aa->name, bb->name);
}
void print_ProductT(const void* a, FILE* file)
{
	ProductT* p = (ProductT*)a;
	fprintf(file, "%s %lf ", p->name, p->price);
}

HipermarketT* create_HipermarketT(char* name)
{
	HipermarketT* empty_market = (HipermarketT*)malloc(sizeof(HipermarketT));
	empty_market->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(empty_market->name, name);
	empty_market->price_of_items = NULL;

	return empty_market;
}

void free_HipermarketT(void* hipermarket)
{
	//TODO: free
	return;
}

double get_hipermarket_price(HipermarketT* hipermarket, GenericLinkedListT* items_to_buy)
{
	GenericHashTableT* hipermarket_prices = hipermarket->price_of_items;
	double sum = 0;
	NodeT* pWalker = items_to_buy->head;
	for (; pWalker != NULL; pWalker = pWalker->next)
	{
		char* item_to_buy = (char*)pWalker->data;

		// price does not matter
		ProductT* product_to_find = create_ProductT(item_to_buy, 0);
		ProductT* product_in_hipermarket = (ProductT*)search_element_in_hash_table(hipermarket_prices, product_to_find, compare_ProductT);
		if (product_in_hipermarket == NULL)
		{
			printf("item not found: %s in %s, skipping hipermarket\n", item_to_buy, hipermarket->name);
			//returning more than initial min, so this hipermarket will not be chosen as min
			return 1E301;
		}

		double product_price = product_in_hipermarket->price;
		sum += product_price;
	}

	return sum;
}

unsigned int hash_ProductT(const void* a)
{
	const int MULT = 37;
	unsigned int sum = 0;
	char* name = ((ProductT*)a)->name;
	while (*name != '\0')
	{
		sum = sum * MULT + *name;
		name++;
	}

	return sum;
}
