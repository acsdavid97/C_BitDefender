#include "L3_P05_Acs_David.h"

/* presupunem, ca toate elementele din multime apar o singura data si elementele
 * sunt deja sortate
 */

MultimeT reuniune(MultimeT a, MultimeT b)
{
	MultimeT reun;
	int aIndex = 0;
	int bIndex = 0;
	int reunIndex = 0;
	while(aIndex < a.length || bIndex < b.length)
	{
		//realocarea elementelor
		reun.elemente = (int*)realloc(reun.elemente, sizeof(int)*(reunIndex + 1));

		if(a.elemente[aIndex] < b.elemente[bIndex] && aIndex != a.length)
		{
			reun.elemente[reunIndex] = a.elemente[aIndex++];
		}
		if(a.elemente[aIndex] >= b.elemente[bIndex] && bIndex != b.length)
		{
			reun.elemente[reunIndex] = b.elemente[bIndex++];
		}
		if(a.length == aIndex)
		{
			reun.elemente[reunIndex] = b.elemente[bIndex++];
		}
		if(b.length == bIndex)
		{
			reun.elemente[reunIndex] = b.elemente[bIndex++];
		}
		reunIndex++;

	}
	reun.length = reunIndex;


	return reun;
}

MultimeT intersect(MultimeT a, MultimeT b)
{
	MultimeT inters;

	return inters;
}

MultimeT diferenta(MultimeT a, MultimeT b)
{
	MultimeT dif;

	return dif;
}

void afisare(MultimeT a)
{
	for(int i = 0; i < a.length; i++)
	{
		printf("%d ", a.elemente[i]);
	}
	printf("\n");
}

