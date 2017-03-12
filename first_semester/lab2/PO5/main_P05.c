#include "L2_P05_Acs_David.h"

int main()
{
	MultimeT a, b;
	a.lungime = 4;
	a.elemente = (int*)malloc(sizeof(int)*4);
	a.elemente[0] = 1;
	a.elemente[1] = 2;
	a.elemente[2] = 3;
	a.elemente[3] = 4;
	afisare(a);

	b.lungime = 4;
	b.elemente = (int*)malloc(sizeof(int)*4);
	b.elemente[0] = 1;
	b.elemente[1] = 6;
	b.elemente[2] = 7;
	b.elemente[3] = 8;
	afisare(b);

	MultimeT reun = reuniune(a, b);
	afisare(reun);
	MultimeT inters = intersect(a, b);
	afisare(inters);
	MultimeT diff = diferenta(a, b);
	afisare(diff);


	printf("sfirsitul programului");
	return 0;
}
