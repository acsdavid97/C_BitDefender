#define _CRT_SECURE_NO_WARNINGS
/*Test 1, Subi.B – Cerc C



Dezvoltati modulul colectie_inele care contine urmatoarele functii(considerand ca elementele sunt de tip intreg) :

	initializare_structura - citirea dimensiunilor si alocarea dinamica a spatiului pentru primul inel - cel mai interior + citire elemente
	adaugare_inel - adaugarea unui inel la pozitia specificata - numarul de elemente necesare se determina de functie si se aloca dinamic + citire elemente
	intre doua inele pot fi inele lipsa
	daca inelul respectiv exista deja - se va semnala utilizatorului si se trece la suprascriere doar daca utilizatorul alege aceasta
	inainte de a adauga un inel oarecare, trebuie initializata structura de baza(o singura data)
	stergere_inel - stergerea inelului de la pozitia specificata
	afisare_matrice - afisarea colectiei de inele sub forma de matrice(linie cu linie)


	Scrieti o aplicatie C care foloseste modulul de mai sus si permite gestiunea colectiilor de inele de intregi(meniu cu optiuni).

author: Acs David
file name: main.cpp
file creation date: 2016-11-23 14:14:03
*/

#include "colectie_inele.h"

int main()
{
	int aux;
	CollectionOfRingsT* rings = (CollectionOfRingsT*)malloc(sizeof(CollectionOfRingsT));
	while (true) {
		print_menu();
		char choice;
		scanf("%c%*c", &choice);
		printf("you chose %c\n", choice);
		switch (choice)
		{
			case '1':
				rings = initializare_structura();
				break;
			case '2':
				printf("numar de inel: ");
				scanf("%d", &aux);
				adugare_inel(rings, aux);

			case '3':
				printf("numar de inel: ");
				scanf("%d", &aux);
				stergere_inel(rings, aux);
			case '4':
				afisare_matrice(rings);
				break;

			case '9':
				print_menu();
				break;
			case '0':
				exit(0);
				break;
			default:
				printf("invalid choice\n");
				break;
		}
	}
	system("pause");
	return 0;
}