#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: generic_data_type.cpp
file creation date: 2016-12-20 10:34:44
*/

#include "generic_data_type.h"

GenericDataT* create_GenericDataT(char* surname, char* name, char* address, char* city, char* telephone, char* email)
{
	GenericDataT* generic_data = (GenericDataT*)malloc(sizeof(GenericDataT));
	generic_data->surname = surname;
	generic_data->name = name;
	generic_data->address = address;
	generic_data->city = city;
	generic_data->telephone = telephone;
	generic_data->email = email;

	return generic_data;
}

void print_GenericDataT(GenericDataT* data)
{
	printf("surname: %s; name: %s; address: %s; city %s; telephone: %s; email: %s\n",
		data->surname, data->name, data->address, data->city, data->telephone, data->email);
}

