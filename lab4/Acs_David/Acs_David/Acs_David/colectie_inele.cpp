#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: colectie_inele
file creation date: 2016-11-23 14:21:52
*/

#include "colectie_inele.h"

void adugare_inel(CollectionOfRingsT* rings, int ring_number)
{
	//check if ring already filled
	if (*(rings->rings + ring_number) != NULL)
	{
		printf("inelul numarul %d va vi suprascris, vreti sa continuati? (y/n)", ring_number);
		char choice;
		scanf("%*c%c", &choice);

		if (choice == 'n') { return; }
	}

	//all the elements minus the nr of element a ring below
	int nr_of_elements = ((rings->coreRows + 2 * ring_number) * (rings->coreCols + 2 * ring_number));
	nr_of_elements -= ((rings->coreRows + 2 * (ring_number - 1)) * (rings->coreCols + 2 * (ring_number -1)));

	//base ring differs
	if (ring_number == 0)
	{
		nr_of_elements = rings->coreCols * rings->coreRows;
	}
	printf("number of elements: %d\n", nr_of_elements);
	int *array = (int*)malloc(sizeof(int) * nr_of_elements);
	for (int i = 0; i < nr_of_elements; i++)
	{
		printf("inel[%d]=", i);
		scanf("%d", array + i);
	}
	//add ring to collection of rings
	*(rings->rings + ring_number) = array;
	if (ring_number > rings->numberOfRings)
		rings->numberOfRings = ring_number;
}

void print_menu()
{
	printf("1. initializare\n");
	printf("2. adaugare inel\n");
	printf("3. stergere inel\n");
	printf("4. afisare matrice\n");
	printf("9. afisare meniu\n");
	printf("0. iesire\n");
}

int get_element(CollectionOfRingsT* rings, int i, int j)
{
	//return the element from i, j position 
	return 0;
}

void afisare_matrice(CollectionOfRingsT* rings)
{
	// calcualte rows and colums
	int cols = rings->coreCols + 2 * (rings->numberOfRings - 1); 
	int rows = rings->coreRows + 2 * (rings->numberOfRings - 1); 
	
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			printf("%d ", get_element(rings, i, j));
		}
		printf("\n");
	}
}


void stergere_inel(CollectionOfRingsT* rings, int ring_number)
{
	//check to not free NULL pointer
	if (*(rings->rings + ring_number) != NULL)
	{
		free(*(rings->rings + ring_number));
		*(rings->rings + ring_number) = NULL;
	}
}

CollectionOfRingsT* initializare_structura()
{
	CollectionOfRingsT* inele = (CollectionOfRingsT*)malloc(sizeof(CollectionOfRingsT));

	//100 should suffice for the number of rings
	inele->rings = (int**)calloc(100, sizeof(int*));

	printf("introduce numarul de linii in inelul de baza: ");
	scanf("%d", &(inele->coreRows));

	printf("introduce numarul de coloane in inelul de baza: ");
	scanf("%d", &(inele->coreCols));

	//add base ring
	adugare_inel(inele, 0);
	inele->numberOfRings = 1;

	return inele;
}
