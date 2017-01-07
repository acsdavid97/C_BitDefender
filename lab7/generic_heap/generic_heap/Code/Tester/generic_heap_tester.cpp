#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_heap_tester.cpp
file creation date: 2017-01-04 22:16:25
*/


#include "generic_heap_tester.h"

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
		case COMPARE_FUNCTION_DIFFERENT:
			fprintf(file, "Error: compare function does not match in heaps, cannot merge heaps\n");
			break;
		case UNKNOWN_ERROR_OCCURED:
			fprintf(file, "Uknown error.\n");
			break;
		default:
			fprintf(file, "Default-Unkown error.\n");
			break;
	}
}

GenericHeapT** get_generic_heap(GenericHeapT** heaps, char* instance, FILE* file)
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
	return heaps + letter;
}

void CreateHeap(GenericHeapT** heap_instance, IOFilesT* files, int(*compare)(const void* a, const void* b))
{
	*heap_instance = create_empty_GenericHeapT(compare);

	if (*heap_instance == NULL)
	{
		print_error(MEMORY_ALLOCATION_FAILED, files->output);
	}
}

void PrintHeap(GenericHeapT* heap_instance, IOFilesT* files,
	void (*print_generic_data)(const void *element, FILE* file))
{
	if (get_nr_of_elements_in_heap(heap_instance) <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_elements_in_heap(heap_instance, files->output, print_generic_data);
}

void AddHeapItem(GenericHeapT* heap_instance, IOFilesT* files, void* (*read_and_create_generic_data)(FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	ReturnCodeE return_code = add_element_in_heap(heap_instance, element);

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

void GetHeapMin(GenericHeapT* heap_instance, IOFilesT* files, void(*print_element)(const void *a, FILE* file))
{
	void* element = get_heap_top(heap_instance);
	if (element == NULL)
	{
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_element(element, files->output);
	fprintf(files->output, "\n");
}

void DeleteHeapMin(GenericHeapT* heap_instance, IOFilesT* files, 
	void(*print_element)(const void *a, FILE* file), void(*free_generic_data)(void* generic_data))
{
	void* element = delete_heap_top(heap_instance);
	if (element == NULL)
	{
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_element(element, files->output);
	fprintf(files->output, "\n");

	free_GenericDataT(element);
	free(element);
}

void DeleteHeapItem(GenericHeapT* heap_instance, IOFilesT* files,
	void(*free_generic_data)(void* generic_data), void* (*read_and_create_generic_data)(FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	void* deleted_element = delete_element_from_heap(heap_instance, element);

	if (deleted_element == NULL)
	{
		print_error(ITEM_NOT_FOUND, files->output);
		return;
	}
	free_generic_data(element);
	free(element);
}
void MergeHeaps(GenericHeapT** heap_instance, GenericHeapT**  heap_instance2, IOFilesT* files)
{
	ReturnCodeE return_code = merge_heaps(*heap_instance, *heap_instance2);

	switch (return_code)
	{
	case SUCCESS:
		free(*heap_instance2);
		*heap_instance2 = NULL;
		break;
	case MEMORY_ALLOCATION_ERROR:
		print_error(MEMORY_ALLOCATION_FAILED, files->output);
		break;
	case COMPARE_FUNCTION_MISMATCH:
		print_error(COMPARE_FUNCTION_DIFFERENT, files->output);
		break;
	default:
		print_error(UNKNOWN_ERROR_OCCURED, files->output);
		break;
	}
}

void DeleteHeap(GenericHeapT** heap_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data))
{
	delete_elements_in_heap(*heap_instance, free_generic_data);
	free(*heap_instance);
	*heap_instance = NULL;
}

PossibleCommandsE get_command(char* command)
{
	if (strcmp(command, "CreateHeap") == 0)
	{
		return CREATE_HEAP;
	}
	else if (strcmp(command, "PrintHeap") == 0)
	{
		return PRINT_HEAP;
	}
	else if (strcmp(command, "AddHeapItem") == 0)
	{
		return ADD_HEAP_ITEM;
	}
	else if (strcmp(command, "GetHeapMin") == 0)
	{
		return GET_HEAP_TOP;
	}
	else if (strcmp(command, "DeleteHeapMin") == 0)
	{
		return DELETE_HEAP_TOP;
	}
	else if (strcmp(command, "DeleteHeapItem") == 0)
	{
		return DELETE_HEAP_ITEM;
	}
	else if (strcmp(command, "MergeMinHeaps") == 0)
	{
		return MERGE_HEAPS;
	}
	else if (strcmp(command, "DeleteHeap") == 0)
	{
		return DELETE_HEAP;
	}
	else
	{
		return UNKOWN_COMMAND;
	}
}

void test_generic_heap(IOFilesT* files)
{
	//256 heaps, so each character corresponds to a GenericHeapT.
	GenericHeapT** heaps = (GenericHeapT**)calloc(256, sizeof(GenericHeapT*));
	if (heaps == NULL)
	{
		fprintf(files->output, "Fatal error: could not allocate memory for heaps\n");
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
		GenericHeapT** heap_instance = get_generic_heap(heaps, first_instance, files->output);

		if (heap_instance == NULL || 
			(*heap_instance == NULL && command != CREATE_HEAP))
		{
			//instance does not correspond to a valid data structure
			//or instance is not yet created
			print_error(DATA_STRUCTURE_INEXISTENT, files->output);
			continue;
		}
		switch (command)
		{
		case CREATE_HEAP:
			printf("CreateHeap\n");
			CreateHeap(heap_instance, files, compare_GenericDataT);
			break;
		case PRINT_HEAP:
			printf("PrintHeap\n");
			PrintHeap(*heap_instance, files, print_GenericDataT);
			break;
		case ADD_HEAP_ITEM:
			printf("AddHeapItem\n");
			AddHeapItem(*heap_instance, files, read_and_create_GenericDataT);
			break;
		case GET_HEAP_TOP:
			printf("GetHeapMin\n");
			GetHeapMin(*heap_instance, files, print_GenericDataT);
			break;
		case DELETE_HEAP_TOP:
			printf("DeleteHeapMin\n");
			DeleteHeapMin(*heap_instance, files, print_GenericDataT, free_GenericDataT);
			break;
		case DELETE_HEAP_ITEM:
			printf("DeleteHeapItem\n");
			DeleteHeapItem(*heap_instance, files, free_GenericDataT, read_and_create_GenericDataT);
			break;
		case MERGE_HEAPS:
		{
			printf("MergeMinHeaps\n");
			char* second_instance = strtok(NULL, " \n");
			GenericHeapT** heap_instance2 = get_generic_heap(heaps, second_instance, files->output);

			if (heap_instance2 == NULL || *heap_instance2 == NULL)
			{
				//instance does not correspond to a valid data structure
				//or instance is not yet created
				print_error(DATA_STRUCTURE_INEXISTENT, files->output);
				continue;
			}
			MergeHeaps(heap_instance, heap_instance2, files);
			break;
		}
		case DELETE_HEAP:
			printf("DeleteHeap\n");
			DeleteHeap(heap_instance, files, free_GenericDataT);
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
	free(heaps);
}
