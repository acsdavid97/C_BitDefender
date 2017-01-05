#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_hash_table_tester.cpp
file creation date: 2017-01-04 22:16:25
*/

#include "generic_hash_table_tester.h"

IOFilesT* create_IOFilesT(const char* file_name)
{
	IOFilesT* files = (IOFilesT*)malloc(sizeof(IOFilesT));
	if (files == NULL)
	{
		return NULL;
	}

	//auxiliary file path
	char* aux_infile_name = (char*)malloc(sizeof(char)*(strlen(file_name) + 20));
	if (aux_infile_name == NULL)
	{
		free(files);
		return NULL;
	}

#pragma warning(suppress: 6386)
	strcpy(aux_infile_name, "Input/");
	strcat(aux_infile_name, file_name);
	strcat(aux_infile_name, ".in");
	files->input = fopen(aux_infile_name, "r");

	if (files->input == NULL)
	{
		printf("input file: %s could not be opened!\n", aux_infile_name);
		free(files);
		return NULL;
	}
	free(aux_infile_name);

	char* aux_out_file_name = (char*)malloc(sizeof(char)*(strlen(file_name) + 20));
	if (aux_out_file_name == NULL)
	{
		free(files);
		return NULL;
	}
#pragma warning(suppress: 6386)
	strcpy(aux_out_file_name, "Results/");
	strcat(aux_out_file_name, file_name);
	strcat(aux_out_file_name, ".res");
	files->output = fopen(aux_out_file_name, "w");

	if (files->output == NULL)
	{
		printf("output file: %s could not be opened!\n", aux_out_file_name);
		fclose(files->input);
		free(files);
		return NULL;
	}
	free(aux_out_file_name);

	return files;
}

void close_IOFiles(IOFilesT* files)
{
	fclose(files->input);
	fclose(files->output);
}

int copy_files(IOFilesT* files)
{
	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, files->input))
	{
		fprintf(files->output, "%s", buffer);
	}
	return 1;
}

void print_error(ErrorCodeE error_code, FILE* file)
{
	switch (error_code)
	{
		case DATA_STRUCTURE_INEXISTENT:
			fprintf(file, "Error: Illegal operation. Data structure does not exist\n");
			break;
		case TYPE_MISMATCH:
			fprintf(file, "Error: Type mismatch\n");
			break;
		case MISSING_VALUE:
			fprintf(file, "Error: Missing value\n");
			break;
		case INDEX_OUT_OF_BOUNDS:
			fprintf(file, "Error: Index out of bounds\n");
			break;
		case UNRECOGNIZED_HASH_FUNCTION:
			fprintf(file, "Error: Unrecognized hash function\n");
			break;
		case MEMORY_ALLOCATION_FAILED:
			fprintf(file, "Error: Memory allocation failed\n");
			break;
		case ITEM_NOT_FOUND:
			fprintf(file, "Cannot delete: Item not found\n");
			break;
		case STRUCTURE_IS_EMPTY:
			fprintf(file, "Printing: Structure is empty\n");
			break;
		case INSTANCE_UNKNOWN:
			fprintf(file, "Error: Instance unknown\n");
			break;
		case DATA_STRUCTURE_OVERWRITTEN:
			fprintf(file, "Error: a structure must be deleted before it can be created again\n");
			break;
		case UNKNOWN_ERROR_OCCURED:
			fprintf(file, "Uknown error.\n");
			break;
		default:
			fprintf(file, "Default-Unkown error.\n");
			break;
	}
}

GenericHashTableT** get_generic_table(GenericHashTableT** tables, char* instance, FILE* file)
{
	if (instance == NULL)
	{
		print_error(INSTANCE_UNKNOWN, file);
		return NULL;
	}

	char letter = instance[0];

	// check if it's valid
	if (strlen(instance) != 1 || letter < 'A' || letter > 'Z')
	{
		return NULL;
	}
	return tables + letter;
}

void CreateHashTable(GenericHashTableT** table_instance, IOFilesT* files)
{
	*table_instance = create_empty_GenericHashTableT();

	if (*table_instance == NULL)
	{
		print_error(MEMORY_ALLOCATION_FAILED, files->output);
	}
}

void PrintHashTable(GenericHashTableT* table_instance, IOFilesT* files,
	void (*print_generic_data)(const void *element, FILE* file))
{
	if (table_instance->nr_of_elements <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	fprintf(files->output, "*Beginning of hash table*\n");
	print_elements_in_hash_table(table_instance, files->output, print_generic_data);
	fprintf(files->output, "*End of hash table*\n");
}

void AddHashTableItem(GenericHashTableT* table_instance, IOFilesT* files, void* (*read_and_create_generic_data)(FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	ReturnCodeE return_code = add_element_in_hash_table(table_instance, element);

	switch (return_code)
	{
	case SUCCESS:
		//do nothing
		break;
	case MEMORY_ALLOCATION_ERROR:
		print_error(MEMORY_ALLOCATION_FAILED, files->output);
	default:
		print_error(UNKNOWN_ERROR_OCCURED, files->output);
		break;
	}

}

void DeleteHashTableItem(GenericHashTableT* table_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data),
	int(*compare)(const void* a, const void *b), void* (*read_and_create_generic_data)(FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	void* deleted_element = delete_element_from_hash_table(table_instance, element, compare);

	if (deleted_element == NULL)
	{
		print_error(ITEM_NOT_FOUND, files->output);
		return;
	}
	free_generic_data(element);
	free(element);
}

void SearchHashTableItem(GenericHashTableT* table_instance, IOFilesT* files, int(*compare)(const void* a, const void* b),
	void* (*read_and_create_generic_data)(FILE* file), void(*print_element)(const void *a, FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	void* element_found = search_element_in_hash_table(table_instance, element, compare);

	if (element_found == NULL)
	{
		fprintf(files->output, "Item not found.\n");
		return;
	}

	print_element(element_found, files->output);
	fprintf(files->output, "\n");
}

//function which returns a hash function, based on string.
unsigned int(*get_hash_function(char* string))(const void*)
{
	if (strcmp(string, "FirstHash\n") == 0)
	{
		return first_hash_for_GenericDataT;
	}
	else if (strcmp(string, "SecondHash\n") == 0)
	{
		return second_hash_for_GenericDataT;
	}
	return NULL;
}

void ReHashTable(GenericHashTableT* table_instance, IOFilesT* files)
{
	char buffer[BUFFER_SIZE];
	fgets(buffer, BUFFER_SIZE, files->input);
	
	unsigned int(*hash_function)(const void* element) = get_hash_function(buffer);
	if (hash_function == NULL)
	{
		print_error(UNRECOGNIZED_HASH_FUNCTION, files->output);
		return;
	}

	ReturnCodeE return_code = rehash_table(table_instance, table_instance->size, hash_function);

	switch (return_code)
	{
	case SUCCESS:
		//do nothing
		break;
	case MEMORY_ALLOCATION_ERROR:
		print_error(MEMORY_ALLOCATION_FAILED, files->output);
		break;
	default:
		print_error(UNKNOWN_ERROR_OCCURED, files->output);
		break;
	}

}

void DeleteHashTable(GenericHashTableT** table_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data))
{
	//TODO: delete_hash_talbe
}

PossibleCommandsE get_command(char* command)
{
	if (strcmp(command, "CreateHashTable") == 0)
	{
		return CREATE_HASH_TABLE;
	}
	else if (strcmp(command, "PrintHashTable") == 0)
	{
		return PRINT_HASH_TABLE;
	}
	else if (strcmp(command, "AddHashTableItem") == 0)
	{
		return ADD_HASH_TABLE_ITEM;
	}
	else if (strcmp(command, "DeleteHashTableItem") == 0)
	{
		return DELETE_HASH_TABLE_ITEM;
	}
	else if (strcmp(command, "SearchHashTableItem") == 0)
	{
		return SEARCH_HASH_TABLE_ITEM;
	}
	else if (strcmp(command, "ReHashTable") == 0)
	{
		return RE_HASH_TABLE;
	}
	else if (strcmp(command, "DeleteHashTable") == 0)
	{
		return DELETE_HASH_TABLE;
	}
	else
	{
		return UNKOWN_COMMAND;
	}
}

void test_generic_hash_table(IOFilesT* files)
{
	//256 tables, so each character corresponds to a GenericHashTableT.
	GenericHashTableT** tables = (GenericHashTableT**)calloc(256, sizeof(GenericHashTableT*));
	if (tables == NULL)
	{
		fprintf(files->output, "Fatal error: could not allocate memory for tables\n");
		fprintf(files->output, "exiting...\n");
		exit(1);
	}

	char buffer[BUFFER_SIZE];
	char buffer_copy[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, files->input))
	{

		strcpy(buffer_copy, buffer);
		char* command_string = strtok(buffer, " \n");

		//line with spaces or just a new line, so we can skip it
		if (command_string == NULL)
		{
			continue;
		}

		PossibleCommandsE command = get_command(command_string);
		if (command == UNKOWN_COMMAND)
		{
			fprintf(files->output, "Error: Unrecognized command: %s\n", command_string);
			continue;
		}

		char* first_instance = strtok(NULL, " \n");
		GenericHashTableT** table_instance = get_generic_table(tables, first_instance, files->output);

		if (table_instance == NULL || 
			(*table_instance == NULL && command != CREATE_HASH_TABLE))
		{
			//instance does not correspond to a valid data structure
			//or instance is not yet created
			print_error(DATA_STRUCTURE_INEXISTENT, files->output);
			continue;
		}
		switch (command)
		{
		case CREATE_HASH_TABLE:
			printf("CreateHashTable\n");
			CreateHashTable(table_instance, files);
			break;
		case PRINT_HASH_TABLE:
			printf("PrintHashTable\n");
			PrintHashTable(*table_instance, files, print_GenericDataT);
			break;
		case ADD_HASH_TABLE_ITEM:
			printf("AddHashTableItem\n");
			AddHashTableItem(*table_instance, files, read_and_create_GenericDataT);
			break;
		case DELETE_HASH_TABLE_ITEM:
			printf("DeleteHashTableItem\n");
			DeleteHashTableItem(*table_instance, files, free_GenericDataT,
				compare_GenericDataT, read_and_create_GenericDataT);
			break;
		case SEARCH_HASH_TABLE_ITEM:
			printf("SearchHashTableItem\n");
			SearchHashTableItem(*table_instance, files, compare_GenericDataT,
				read_and_create_GenericDataT, print_GenericDataT);
			break;
		case RE_HASH_TABLE:
			printf("ReHashTable\n");
			ReHashTable(*table_instance, files);
			break;
		case DELETE_HASH_TABLE:
			printf("DeleteHashTable\n");
			DeleteHashTable(table_instance, files, free_GenericDataT);
			break;
		case UNKOWN_COMMAND:
			printf("Unknown command\n");
			break;
		default:
			printf("What?\n");
			fprintf(files->output, "WHaAaAaT?\n");
			exit(42); // the answer to life the universe and everything
			break;
		}

	}

	free(tables);
}
