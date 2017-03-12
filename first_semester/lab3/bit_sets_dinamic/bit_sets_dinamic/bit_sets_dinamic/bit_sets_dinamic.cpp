#define _CRT_SECURE_NO_WARNINGS

/*
author: David
file name: bit_sets_dinamic.cpp
file creation date: 2016-11-22 10:41:36
*/

#include "bit_sets_dinamic.h"

void print_menu()
{
	printf("1. insert a new set\n");
	printf("2. print elements of a set\n");
	printf("3. print cardinality of a set\n");
	printf("4. reunion of two set\n");
	printf("5. intersection of two sets\n");
	printf("6. difference of two sets\n");
	printf("7. check if number is part of a set\n");
	printf("8. show menu\n");
	printf("0. exit\n");
}

//dinamically resizes the sets
void resize_sets(SetsT* sets, int NEW_ARRAY_SIZE)
{
	sets->a = (int*)realloc(sets->a, NEW_ARRAY_SIZE * sizeof(int));
	sets->b = (int*)realloc(sets->b, NEW_ARRAY_SIZE * sizeof(int));
	sets->c = (int*)realloc(sets->c, NEW_ARRAY_SIZE * sizeof(int));

	for (int i = sets->ARRAY_SIZE; i < NEW_ARRAY_SIZE; i++)
	{
		*(sets->a + i) = 0;
		*(sets->b + i) = 0;
		*(sets->c + i) = 0;
	}
	sets->ARRAY_SIZE = NEW_ARRAY_SIZE;
}

//user input, used for the menu
void get_input(SetsT *sets)
{
	int response = 0;
	printf("choose a menu option: ");
	scanf("%d", &response);
	if (response < 0 || response > 8)
	{
		printf("not a valid option\n");
		print_menu();
		return;
	}

	int *first = NULL;
	int *second = NULL;
	int *result = NULL;
	int aux = 0;

	switch (response) {
	case 0:
		exit(0);
		break;
	case 1:
		first = get_set(sets);
		insert_set(first, sets);
		break;
	case 2:
		first = get_set(sets);
		print_elements(first, sets);
		break;
	case 3:
		first = get_set(sets);
		print_cardinality(first, sets);
		break;
	case 4:
		printf("first set:\n");
		first = get_set(sets);
		printf("second set:\n");
		second = get_set(sets);
		printf("result set:\n");
		result = get_set(sets);
		reunion_sets(first, second, result, sets);
		break;
	case 5:
		printf("first set:\n");
		first = get_set(sets);
		printf("second set:\n");
		second = get_set(sets);
		printf("result set:\n");
		result = get_set(sets);
		intersection_sets(first, second, result, sets);
		break;
	case 6:
		printf("first set:\n");
		first = get_set(sets);
		printf("second set:\n");
		second = get_set(sets);
		printf("result set:\n");
		result = get_set(sets);
		difference_sets(first, second, result, sets);
		break;
	case 7:
		first = get_set(sets);
		printf("what number do you want to check? :");
		scanf("%d", &aux);
		check_number_in_set(first, aux, sets);
		break;
	case 8:
		print_menu();
		break;
	default:
		printf("what?");
	}
}

//checks if a number is in the set
void check_number_in_set(int *bit_set, int number, SetsT* sets)
{
	printf("check if number is in set\n");
	int index_in_array = number / sets->INT_SIZE;
	int index_in_int = number % sets->INT_SIZE;
	int mask = 1;
	mask = mask << index_in_int;
	if (mask & bit_set[index_in_array])
	{
		printf("number %d is in the set\n", number);
	}
	else {
		printf("number %d in not in the set\n", number);
	}
}

//the result set will be the difference of the two other sets
void difference_sets(int *first, int *second, int *result, SetsT* sets)
{
	printf("difference of two sets\n");
	for (int i = 0; i < sets->ARRAY_SIZE; i++)
	{
		result[i] = first[i] & (~second[i]);
	}
}

//the result set will be the intersection of the two other sets
void intersection_sets(int *first, int *second, int *result, SetsT* sets)
{
	printf("intersection of two sets\n");
	for (int i = 0; i < sets->ARRAY_SIZE; i++)
	{
		result[i] = first[i] & second[i];
	}
}

//the result set will be the reunion of the two other sets
void reunion_sets(int *first, int *second, int *result, SetsT* sets)
{
	printf("reunion of two sets\n");
	for (int i = 0; i < sets->ARRAY_SIZE; i++)
	{
		result[i] = first[i] | second[i];
	}
}

//inserts a number in a set, it is supposed that the set is large enough
void insert_number_in_set(int *bit_set, int number, SetsT* sets)
{
	int index_in_array = number / sets->INT_SIZE;
	int index_in_int = number % sets->INT_SIZE;
	int mask = 1;
	mask = mask << index_in_int;
	bit_set[index_in_array] = bit_set[index_in_array] | mask;
}

//prints the cardinality of a set
void print_cardinality(int *bit_set, SetsT* sets)
{
	printf("printing cardinality of a set\n");
	int sum = 0;
	for (int i = 0; i < sets->ARRAY_SIZE; i++)
	{
		int copy = bit_set[i];
		for (int j = 0; j < sets->INT_SIZE && copy != 0; j++)
		{
			if (copy & 1)
			{
				sum++;
			}
			copy = copy >> 1;
		}
	}
	printf("the cardinality of the set is :%d\n", sum);
}

//reads elements from the keyboard and inserts them in the given set. Sets resized as necessary
void insert_set(int *bit_set, SetsT* sets)
{
	printf("inserting a set\n");
	printf("insert the number of elements in the set: ");
	int n = 0;
	scanf("%d", &n);
	int k = 0;
	int aux;
	int *array = (int*)malloc(sizeof(int)*n);
	for (int i = 0; i<n; i++)
	{
		printf("set[%d]=", i);
		scanf("%d", &aux);
		if (aux >= 0)
		{
			array[k++] = aux;
		}
		else {
			printf("only accepitng elements in the interval [0, inf)\n");
			printf("ommiting insereted value\n");
		}
	}

	for (int i = 0; i < sets->ARRAY_SIZE; i++)
	{
		bit_set[i] = 0;
	}

	for (int i = 0; i < k; i++)
	{
		if (array[i] >= sets->ARRAY_SIZE * sets->INT_SIZE)
		{
			printf("resing sets...\n");
			resize_sets(sets, (array[i] / 32) + 1);
			printf("wher to save resized set?");
			bit_set = get_set(sets);
		}
		insert_number_in_set(bit_set, array[i], sets);
	}
}

//prints the elements of a set
void print_elements(int *bit_set, SetsT* sets)
{
	printf("printing elements of a set\n");
	for (int i = 0; i < sets->ARRAY_SIZE; i++)
	{
		int copy = bit_set[i];
		for (int j = 0; j < sets->INT_SIZE && copy != 0; j++)
		{
			if (copy & 1)
			{
				printf("%d ", i * sets->INT_SIZE + j);
			}
			copy = copy >> 1;

		}
	}
	printf("\n");
}

//returns the selected set
int* get_set(SetsT *sets)
{
	printf("choose a set from a, b or c: ");
	char aux;
	scanf("%*c%c", &aux);
	printf("you chose %c\n", aux);
	switch (aux) {
	case 'a':
		return sets->a;
	case 'b':
		return sets->b;
	case 'c':
		return sets->c;
	default:
		printf("set not existing, using default set a");
	}
	return sets->a;
}