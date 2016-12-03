/*Test 1, Subi.A – Cerc C



Dezvoltati modulul tablou_dinamic care contine urmatoarele functii(considerand ca elementele sunt de tip intreg) :

	alocare_matrice - citirea dimensiunilor si alocarea dinamica a spatiului pentru matrice
	citire_elemente - popularea matricei cu elemente date linie cu linie
	adaugare_linie - adaugarea unei noi linii la matrice dupa o linie specificata prin numar de ordine si citirea noilor elemente
	adaugare_coloana - adaugarea unei noi coloane la matrice dupa o coloana specificata prin numar de ordine si citirea noilor elemente
	adaugare_inel - expandarea dimensiunii matricei astfel incat vechea matrice sa fie inscrisa intr - un inel exterior si citirea elementelor de pe inel
	afisare matrice - afiseaza matricea linie cu linie


	Scrieti o aplicatie C care foloseste modulul de mai sus si permite gestiunea tablourilor bidimensionale de intregi(meniu cu optiuni).
*/

#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-12-01 13:28:26
*/

#include "matrix_ops.h"

int main()
{
	MatrixT* matrix = allocate_matrix();
	read_matrix(matrix);
	print_matrix(matrix);
	add_line(matrix, 2, FORWARD);
	print_matrix(matrix);
	add_column(matrix, 1, FORWARD);
	print_matrix(matrix);
	add_ring(matrix);
	print_matrix(matrix);

	system("pause");
	return 0;
}