#include "L2_P05_Acs_David.h"

/* presupunem, ca toate elementele din multime apar o singura data si elementele
 * sunt deja sortate
 */

MultimeT reuniune(MultimeT a, MultimeT b)
{
	MultimeT multimeT;
	multimeT.elemente = (int*) malloc(sizeof(int)*(a.lungime + b.lungime));
	multimeT.lungime = 0;
	int iA = 0, iB = 0, iMultime = 0;
	while (iA < a.lungime && iB < b.lungime)
	{
		if (a.elemente[iA] < b.elemente[iB])
		{
			multimeT.elemente[iMultime] = a.elemente[iA];
			iA++;
		}
		else
		{
			multimeT.elemente[iMultime] = b.elemente[iB];
			iB++;
		}
		iMultime++;
	}
	if (iA < a.lungime)
	{
		for (int i = iA; i < a.lungime; i++)
		{
			multimeT.elemente[iMultime] = a.elemente[i];
			iMultime++;
		}
	}
	if(iB < b.lungime)
	{
		for (int i = iB; i < b.lungime; i++)
		{
			multimeT.elemente[iMultime] = b.elemente[i];
			iMultime++;
		}
	}
	multimeT.lungime = iMultime;
	multimeT.elemente = (int*)realloc(multimeT.elemente, sizeof(int)* multimeT.lungime);
	return multimeT;
}


MultimeT intersect(MultimeT a, MultimeT b)
{
	MultimeT inters;
	inters.lungime = a.lungime;
	inters.elemente = (int*) malloc(sizeof(int)*a.lungime);
	int iA = 0, iB = 0, iMultime = 0;
	while(iA < a.lungime && iB < b.lungime) 
	{
		if(a.elemente[iA] < b.elemente[iB])
		{
			iA++;
		}
		if(a.elemente[iA] < b.elemente[iB])
		{
			iB++;
		}
		if(a.elemente[iA] == b.elemente[iB])
		{
			inters.elemente[iMultime] = a.elemente[iA];
			iMultime++;
			iA++;
			iB++;
		}
	}
		
	inters.lungime = iMultime;
	inters.elemente = (int*) realloc(inters.elemente, sizeof(int)*inters.lungime);
	return inters;
}

MultimeT diferenta(MultimeT a, MultimeT b)
{
	MultimeT dif;
	dif.lungime = a.lungime;
	dif.elemente = (int*) malloc(sizeof(int)*a.lungime);
	int iA = 0, iB = 0, iMultime = 0;
	while(iA < a.lungime && iB < b.lungime)
	{
		if(a.elemente[iA] < b.elemente[iB])
		{
			dif.elemente[iMultime] = a.elemente[iA];
			iA++;
			iMultime++;
		}
		if(a.elemente[iA] > b.elemente[iB])
		{
			iB++;
		}
		if(a.elemente[iA] == b.elemente[iB])
		{
			iA++;
			iB++;
		}
	}
	dif.lungime = iMultime;
	dif.elemente = (int*) realloc(dif.elemente, sizeof(int)*iMultime);
	
	return dif;
}

void afisare(MultimeT a)
{
	for(int i = 0; i < a.lungime; i++)
	{
		printf("%d ", a.elemente[i]);
	}
	printf("\n");
}

