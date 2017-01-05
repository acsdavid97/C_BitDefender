#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_hash_table.cpp
file creation date: 2016-12-18 19:48:52
*/

#include "generic_hash_table.h"


int is_prime(int number)
{
	for (int i = 2; i <= sqrt(number); i++)
	{
		if (number % i == 0)
			return 0;
	}
	return 1;
}

int get_double_size(int size)
{
	int new_size = 2 * size;
	while (is_prime(new_size) == 0)
	{
		new_size++;
	}
	return new_size;
}

GenericHashTableT* create_empty_GenericHashTableT()
{
	GenericHashTableT* empty_table = (GenericHashTableT*)malloc(sizeof(GenericHashTableT));
	if (empty_table == NULL)
	{
		return NULL;
	}

	empty_table->array_of_lists = (GenericLinkedListT**)malloc(sizeof(GenericLinkedListT*) * STARTING_TABLE_SIZE);
	if (empty_table->array_of_lists == NULL)
	{
		free(empty_table);
		return NULL;
	}
	for (int i = 0; i < STARTING_TABLE_SIZE; i++)
	{
		*(empty_table->array_of_lists + i) = create_empty_GenericLinkedListT();
		if (*(empty_table->array_of_lists + i) == NULL)
		{
			free(empty_table->array_of_lists);
			free(empty_table);
			return NULL;
		}
	}

	empty_table->nr_of_elements = 0;
	empty_table->size = STARTING_TABLE_SIZE;
	empty_table->hash_function = NULL;

	return empty_table;
}

unsigned int get_hash(GenericHashTableT* table, void* element)
{
	if (table->hash_function == NULL)
	{
		//default function, we cannot use something better, since the elements are generic. 
		return ((*(unsigned int*)element) % table->size);
	}

	return table->hash_function(element) % table->size;
}

double get_load_factor(GenericHashTableT* table)
{
	return (double)table->nr_of_elements / table->size;
}

ReturnCodeE rehash_table(GenericHashTableT* hash_table,unsigned int new_size, unsigned int(*hash_function)(const void* element))
{
	//allocating memory for data
	GenericLinkedListT** rehashed_data = (GenericLinkedListT**)malloc(sizeof(GenericLinkedListT*) * new_size);

	if (rehashed_data == NULL)
	{
		return MEMORY_ALLOCATION_ERROR;
	}
	for (unsigned int i = 0; i < new_size; i++)
	{
		*(rehashed_data + i) = create_empty_GenericLinkedListT();
		if (*(rehashed_data + i) == NULL)
		{
			free(rehashed_data);
			return MEMORY_ALLOCATION_ERROR;
		}
	}

	hash_table->hash_function = hash_function;
	int old_size = hash_table->size;
	hash_table->size = new_size;

	//inserting all of the elements again, using the new hash function.
	for (int i = 0; i < old_size; i++)
	{
		GenericLinkedListT* list = *(hash_table->array_of_lists + i);
		NodeT* node;
		while ((node = get_node_at_index(list, 0)) != NULL)
		{
			void* element = node->data;
			unsigned int hashed = get_hash(hash_table, element);
			GenericLinkedListT* list_to_be_inserted = *(rehashed_data + hashed);
			NodeT* new_node = create_NodeT(element);
			add_node_at_end(list_to_be_inserted, new_node);

			remove_node_from_beginning(list);
		}

		free(list);
	}

	free(hash_table->array_of_lists);
	hash_table->array_of_lists = rehashed_data;
	return SUCCESS;
}

ReturnCodeE add_element_in_hash_table(GenericHashTableT* table, void* element)
{
	double load_factor = get_load_factor(table);

	if (load_factor >= MAX_LOAD_FACTOR)
	{
		int new_size = get_double_size(table->size);
		ReturnCodeE return_code = rehash_table(table, new_size, table->hash_function);
		if (return_code != SUCCESS)
		{
			return return_code;
		}
	}

	unsigned int hashed = get_hash(table, element);
	GenericLinkedListT* list = *(table->array_of_lists + hashed);
	NodeT* node = create_NodeT(element);
	if (node == NULL)
	{
		return MEMORY_ALLOCATION_ERROR;
	}
	table->nr_of_elements++;
	return add_node_at_end(list, node);
}

void* delete_element_from_hash_table(GenericHashTableT* table, void* element, int(*compare)(const void* a, const void* b))
{
	int hashed = get_hash(table, element);
	GenericLinkedListT* list = *(table->array_of_lists + hashed);
	void* deleted_element = search_and_delete_element_in_list(list, element, compare);
	if (deleted_element == NULL)
	{
		return NULL;
	}

	table->nr_of_elements--;
	return deleted_element;
}

void* search_element_in_hash_table(GenericHashTableT* table, void* element, int (*compare)(const void* a, const void* b))
{
	unsigned int hashed = get_hash(table, element);
	GenericLinkedListT* list = *(table->array_of_lists + hashed);
	return search_element_in_list(list, element, compare);
}

void print_elements_in_hash_table(GenericHashTableT* table, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	for (int i = 0; i < table->size; i++)
	{
		fprintf(file, "%d: ", i);
		GenericLinkedListT* list = *(table->array_of_lists + i);
		if (list->length > 0)
			print_linked_list(list, file, print_element);
		else
			fprintf(file, "<empty>\n");
	}
}

void delete_elements_in_hash_table(GenericHashTableT* table, void(*free_generic_data)(void* generic_data))
{
	for (int i = 0; i < table->size; i++)
	{
		GenericLinkedListT* list = *(table->array_of_lists + i);
		delete_linked_list(list, free_generic_data);
		free(list);
	}
	free(table->array_of_lists);
	table->nr_of_elements = 0;
	table->size = 0;
}

