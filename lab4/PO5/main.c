#include "L3_P05_Acs_David.h"

int main()
{
	MultimeT first, second;
	first.length = 4;
	first.elemente = (int*)malloc(sizeof(int)*4);
	first.elemente[0] = 1;
	first.elemente[1] = 2;
	first.elemente[2] = 3;
	first.elemente[3] = 4;
	afisare(first);

	second.length = 4;
	second.elemente = (int*)malloc(sizeof(int)*4);
	second.elemente[0] = 1;
	second.elemente[1] = 6;
	second.elemente[2] = 7;
	second.elemente[3] = 8;
	afisare(second);

	MultimeT reun = reuniune(first, second);
	afisare(reun);

	printf("sfirsitul programului");
	return 0;
}
