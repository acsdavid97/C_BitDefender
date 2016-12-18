#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-12-18 19:48:35
*/

#include "generic_array.h"

int main()
{
	GenericArrayT* array = create_empty_GenericArrayT();
	GenericDataT* element = create_GenericDataT(123);
	GenericDataT* element2 = create_GenericDataT(12);
	GenericDataT* element3 = create_GenericDataT(2);

	add_element_at_index(array, element, 0);
	add_element_at_index(array, element2, 1);
	add_element_at_index(array, element3, 1);

	delete_element_at_index(array, 0);

	print_elements_array(array);

	system("pause");
	return 0;
}