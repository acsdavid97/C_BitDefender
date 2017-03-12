/*
author: Acs David
file name: colectie_inele.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef colectie_inele_H
#define colectie_inele_H

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct CollectionOfRingsTag {
	int coreRows;
	int coreCols;
	int numberOfRings;
	int **rings;
}CollectionOfRingsT;

// define your functions here

CollectionOfRingsT* initializare_structura();
void adugare_inel(CollectionOfRingsT* rings, int ring_number);
void stergere_inel(CollectionOfRingsT* rings, int ring_number);
void print_menu();
void afisare_matrice(CollectionOfRingsT* rings);

#endif
