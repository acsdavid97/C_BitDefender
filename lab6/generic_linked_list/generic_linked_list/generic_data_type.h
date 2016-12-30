/*
author: Acs David
file name: generic_data_type
file creation date: 2016-11-20 23:26:24
*/

#ifndef generic_data_type_H
#define generic_data_type_H

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct GenericDataTag {
	char* surname;
	char* name;
	char* address;
	char* city;
	char* telephone;
	char* email;
}GenericDataT;

// define your functions here
GenericDataT* create_GenericDataT(char* surname, char* name, char* address, char* city, char* telephone, char* email);
void print_GenericDataT(GenericDataT* data);

#endif
