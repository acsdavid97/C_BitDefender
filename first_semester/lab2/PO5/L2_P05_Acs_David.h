#ifndef MODULE_H_MULTIME
#define MODULE_H_MULTIME

#include <stdlib.h>
#include <stdio.h>

typedef struct MultimeTag {
	int lungime;
	int *elemente;
}MultimeT;

MultimeT reuniune(MultimeT a, MultimeT b);

MultimeT diferenta(MultimeT a, MultimeT b);

MultimeT intersect(MultimeT a, MultimeT b);

void afisare(MultimeT a);

#endif
