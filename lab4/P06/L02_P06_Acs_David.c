#include "L02_P06_Acs_David.h"

int palindrom(int numar)
{
	int aux = numar;
	int inversat = 0;
	//inversarea numerelor, numere negative nu pot fii palindrome
	while(numar > 0)
	{
		inversat = 10*inversat + numar % 10;
		numar /= 10;
	}

	if(inversat == aux)
	{
		printf("palindrom\n");
		return 1;
	}
	else
	{
		printf("nu e palindrom\n");
		return 0;
	}
}

int numar_cifre(int numar)
{
	int nr_cifre = 0;
	while(numar !=0)
	{
		numar /= 10;
		nr_cifre++;
	}
	return nr_cifre;
}

int max_din_trei(int a, int b, int c)
{
	int aux = (a>b ? a : b);
	return (aux>c ? aux : c);
}

//formula pentru numere perfecte: (2^(n-1))*(2^n - 1)
void numere_perfecte_mai_mic_n(int n)
{
	int pow_2_n = 1;
}

int numar_aleator(int n, int m)
{
	srand(time(NULL));
	int diff = m - n;
	int aleator = (rand() % diff) + n;
	return aleator;
}

void factori_primi(long long numar)
{
	int divizor = 2;
	printf("numar= ");
	while(numar > 1)
	{
		int puterea = 0;
		while(numar % divizor == 0)
		{
			puterea++;
			numar /= divizor;
		}

		if(puterea)
		{
			printf("%d^%d * ", divizor, puterea);
		}
		divizor++;
	}
	printf("1");
}
