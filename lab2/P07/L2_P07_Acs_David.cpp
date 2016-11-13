#include "L2_P07_Acs_David.h"

VectorT citire_tastatura()
{
	printf("numerul elementelor: ");
	int n = 0;
	scanf("%d", &n);
	VectorT vectorT;
	vectorT.lungime = n;
	vectorT.elemente = (int*)malloc(sizeof(int)*n);
	for (int i = 0; i < n; i++)
	{
		printf("vector[%d]= ", i);
		scanf("%d", &(vectorT.elemente[i]));
	}
	return vectorT;
}

void afisare_elementelor(VectorT vectorT)
{
	for (int i = 0; i < vectorT.lungime; i++)
	{
		printf("%d ", vectorT.elemente[i]);
	}
	printf("\n");
}

void inserare_pozitie(VectorT vectorT, int pozitie, int numar)
{
	if (pozitie >= vectorT.lungime || pozitie < 0)
	{
		printf("pozitie invalida");
		return;
	}
	vectorT.elemente = (int*)realloc(vectorT.elemente, sizeof(int)*(vectorT.lungime + 1));
	// loc pentru inserare
	for (int i = vectorT.lungime; i > pozitie; i--)
	{
		vectorT.elemente[i] = vectorT.elemente[i - 1];
	}
	vectorT.elemente[pozitie] = numar;
	vectorT.lungime++;
}

void stergerea_pozitie(VectorT vectorT, int pozitie)
{
	if (pozitie >= vectorT.lungime || pozitie < 0)
	{
		printf("pozitie invalida");
		return;
	}
	for (int i = pozitie; i < vectorT.lungime - 1; i++)
	{
		vectorT.elemente[i] = vectorT.elemente[i + 1];
	}

	vectorT.lungime--;
	vectorT.elemente = (int*)realloc(vectorT.elemente, vectorT.lungime*sizeof(int));
}

int max_element(VectorT vectorT)
{
	int max = vectorT.elemente[0];
	for (int i = 0; i < vectorT.lungime; i++)
	{
		if (max < vectorT.elemente[i])
		{
			max = vectorT.elemente[i];
		}
	}
	return max;
}

int min_element(VectorT vectorT)
{
	int min = vectorT.elemente[0];
	for (int i = 0; i < vectorT.lungime; i++)
	{
		if (min > vectorT.elemente[i])
		{
			min = vectorT.elemente[i];
		}
	}
	return min;

}

int element_majoritar(VectorT vectorT)
{
	return 1;
}

int comparare(const void *a, const void *b)
{
	return (*(int*)a - *(int*)b);
}

void ordonare(VectorT vectorT)
{
	qsort(vectorT.elemente, vectorT.lungime, sizeof(int), comparare);
}

VectorT intercalare(VectorT a, VectorT b)
{
	VectorT vectorT;
	vectorT.elemente = (int*) malloc(sizeof(int)*(a.lungime + b.lungime));
	vectorT.lungime = 0;
	int iA = 0, iB = 0, iVector = 0;
	while (iA < a.lungime && iB < b.lungime)
	{
		if (a.elemente[iA] < b.elemente[iB])
		{
			vectorT.elemente[iVector] = a.elemente[iA];
			iA++;
		}
		else
		{
			vectorT.elemente[iVector] = b.elemente[iB];
			iB++;
		}
		iVector++;
	}
	if (iA < a.lungime)
	{
		for (int i = iA; i < a.lungime; i++)
		{
			vectorT.elemente[iVector] = a.elemente[i];
			iVector++;
		}
	}
	if(iB < b.lungime)
	{
		for (int i = iB; i < b.lungime; i++)
		{
			vectorT.elemente[iVector] = b.elemente[i];
			iVector++;
		}
	}
	vectorT.lungime = iVector;
	vectorT.elemente = (int*)realloc(vectorT.elemente, sizeof(int)* vectorT.lungime);
	return 	vectorT;
}
