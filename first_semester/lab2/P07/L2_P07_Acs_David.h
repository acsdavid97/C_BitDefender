#ifndef MODULE_H_VECTOR
#define MODULE_H_VECTOR

#include <stdio.h>
#include <stdlib.h>

typedef struct VectorTag
{
	int lungime;
	int *elemente;
}VectorT;

VectorT citire_tastatura();
void afisare_elementelor(VectorT);
void inserare_pozitie(VectorT, int, int);
void stergerea_pozitie(VectorT, int);
int max_element(VectorT);
int min_element(VectorT);
int element_majoritar(VectorT);
void ordonare(VectorT);
VectorT intercalare(VectorT a, VectorT b);

#endif

