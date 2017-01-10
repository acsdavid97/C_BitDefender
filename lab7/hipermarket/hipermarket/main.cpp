#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2017-01-10 09:37:32
*/

#include "hipermarket.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("too many, or too few arguments!\n");
		printf("usage: hipermarket.exe hipermarket_prices.txt list_of_products.txt\n");
		exit(1);
	}
	FILE* hipermarkets_file = fopen(argv[1], "r");
	if (hipermarkets_file == NULL)
	{
		printf("could not open %s", argv[1]);
		exit(1);
	}
	FILE* list_of_products_file = fopen(argv[2], "r");
	if (list_of_products_file == NULL)
	{
		fclose(hipermarkets_file);
		printf("could not open %s", argv[2]);
		exit(1);
	}

	GenericLinkedListT* hipermarket_list = create_empty_GenericLinkedListT();
	char buffer[1024];
	while (fgets(buffer, 1024, hipermarkets_file))
	{
		char* trimmed_buffer = strtok(buffer, "\n");
		HipermarketT* hipermarket = create_HipermarketT(trimmed_buffer);
		hipermarket->price_of_items = create_empty_GenericHashTableT();
		rehash_table(hipermarket->price_of_items, hipermarket->price_of_items->size, hash_ProductT);

		//for each supermarket read products
		while (fgets(buffer, 1024, hipermarkets_file))
		{
			char* product_name = strtok(buffer, " \n");
			if (product_name == NULL)
			{
				// we reached end of a supermarket, since this line consists only of whitespace
				break;
			}
			char* price_string = strtok(NULL, " \n");
			double price;
			sscanf(price_string, "%lf", &price);
			ProductT* product = create_ProductT(product_name, price);

			add_element_in_hash_table(hipermarket->price_of_items, product);
		}

		NodeT* node = create_NodeT(hipermarket);
		add_node_at_end(hipermarket_list, node);
	}

	GenericLinkedListT* things_to_buy = create_empty_GenericLinkedListT();
	while (fgets(buffer, 1024, list_of_products_file))
	{
		char* trimmed_buffer = strtok(buffer, "\n");
		char* name_of_product = (char*)malloc(sizeof(char) * (strlen(trimmed_buffer) + 1));
		strcpy(name_of_product, trimmed_buffer);
		NodeT* node = create_NodeT(name_of_product);
		add_node_at_end(things_to_buy, node);
	}

	double min = 1E300;
	HipermarketT* min_hipermarket = NULL;
	NodeT* pWalker = hipermarket_list->head;
	for (; pWalker != NULL; pWalker = pWalker->next)
	{
		HipermarketT* current_hipermarket = (HipermarketT*)pWalker->data;
		double hipermarket_price = get_hipermarket_price(current_hipermarket, things_to_buy);

		if (hipermarket_price < min)
		{
			min = hipermarket_price;
			min_hipermarket = current_hipermarket;
		}
	}
	if (min_hipermarket == NULL)
	{
		printf("did not found a hipermarket, with all the items on the list\n");
	}
	else
	{
		printf("min hipermarket name: %s, total price: %lf\n", min_hipermarket->name, min);
	}

	delete_linked_list(hipermarket_list, free_HipermarketT);
	free(hipermarket_list);

	fclose(hipermarkets_file);
	fclose(list_of_products_file);

	system("pause");
	return 0;
}