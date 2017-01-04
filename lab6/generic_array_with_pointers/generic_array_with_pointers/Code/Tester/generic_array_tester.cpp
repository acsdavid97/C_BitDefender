#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_array_tester.cpp
file creation date: 2016-11-29 22:16:25
*/

#include "generic_array_tester.h"

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

void print_error(ErrorCodeE error_code, FILE* file,const char* message)
{
	switch (error_code)
	{
		case DATA_STRUCTURE_INEXISTENT:
			fprintf(file, "Error: Illegal operation. Data structure does not exist\n");
			break;
		case UNRECOGNIZED_COMMAND:
			fprintf(file, "Error: Unrecognized command: '%s'", message);
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

GenericArrayT** get_generic_array(GenericArrayT** arrays, char* instance, FILE* file)
{
	if (instance == NULL)
	{
		print_error(INSTANCE_UNKNOWN, file, NULL);
		return NULL;
	}

	char letter = instance[0];

	if (strlen(instance) != 1 || letter < 'A' || letter > 'Z')
	{
		return NULL;
	}
	return arrays + letter;
}

void add_element_at_index_with_errors(GenericArrayT* array, void* element, int index, FILE* file)
{
	ReturnCodeE return_code = add_element_at_index(array, element, index);

	switch (return_code)
	{
	case SUCCESS:
		//do nothing
		break;
	case MEMORY_ALLOCATION_ERROR:
		print_error(MEMORY_ALLOCATION_FAILED, file, NULL);
		break;
	case INCORRECT_INDEX:
		print_error(INDEX_OUT_OF_BOUNDS, file, NULL);
		break;
	default:
		print_error(UNKNOWN_ERROR_OCCURED, file, NULL);
		break;
	}
}

void delete_element_at_index_with_errors(GenericArrayT* array, int index, FILE* file, void(*free_generic_data)(void* generic_data))
{
	ReturnCodeE return_code = delete_element_at_index(array, index, free_generic_data);

	switch (return_code)
	{
	case SUCCESS:
		//do nothing
		break;
	case INCORRECT_INDEX:
		print_error(INDEX_OUT_OF_BOUNDS, file, NULL);
		break;
	default:
		print_error(UNKNOWN_ERROR_OCCURED, file, NULL);
		break;
	}

}

void CreateVector(GenericArrayT** array_instance, IOFilesT* files)
{
	*array_instance = create_empty_GenericArrayT();

	if (*array_instance == NULL)
	{
		print_error(MEMORY_ALLOCATION_FAILED, files->output, NULL);
	}
}
//TODO add functions
void PrintVector(GenericArrayT* array_instance, IOFilesT* files,
	void (*print_generic_data)(const void *element, FILE* file))
{
	if (array_instance->length <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output, NULL);
		return;
	}

	print_elements_in_array(array_instance, files->output, print_generic_data);
}

void AddVectorItems(GenericArrayT* array_instance, IOFilesT* files, void* (*read_and_create_generic_data)(FILE* file))
{
	int nr_of_elements = 0;

	if (fscanf(files->input, "%d", &nr_of_elements))
	{
		for (int i = 0; i < nr_of_elements; i++)
		{
			void* element = read_and_create_generic_data(files->input);
			add_element_at_index_with_errors(array_instance, element, array_instance->length, files->output);
		}
	}
	else
	{
		//could not read nr_of_elements, maybe it's missing?
		print_error(MISSING_VALUE, files->output, NULL);
	}

}
void PutVectorItem(GenericArrayT* array_instance, IOFilesT* files, void* (*read_and_create_generic_data)(FILE* file))
{
	int index = 0;
	if (fscanf(files->input, "%d", &index))
	{
		void* element = read_and_create_generic_data(files->input);
		add_element_at_index_with_errors(array_instance, element, index, files->output);
	}
	else
	{
		//could not read index, maybe it's missing?
		print_error(MISSING_VALUE, files->output, NULL);
	}
}

void GetVectorItem(GenericArrayT* array_instance, IOFilesT* files, void (*print_element)(const void *a, FILE* file))
{
	int index = 0;
	if (fscanf(files->input, "%d", &index))
	{
		void* element = get_element_at_index(array_instance, index);
		if (element == NULL)
		{
			print_error(INDEX_OUT_OF_BOUNDS, files->output, NULL);
			return;
		}
		print_element(element, files->output);
		fprintf(files->output, "\n");
	}
	else
	{
		//could not read index, maybe it's missing?
		print_error(MISSING_VALUE, files->output, NULL);
	}
}

void DeleteVectorItem(GenericArrayT* array_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data))
{
	int index = 0;
	if (fscanf(files->input, "%d", &index))
	{
		delete_element_at_index_with_errors(array_instance, index, files->output, free_generic_data);
	}
	else
	{
		//could not read index, maybe it's missing?
		print_error(MISSING_VALUE, files->output, NULL);
	}
	
}

void SortVector(GenericArrayT* array_instance, IOFilesT* files, int(*compare)(const void* a, const void* b)) 
{
	qsort_elements_in_array(array_instance, compare);
}

void SearchVectorItem(GenericArrayT* array_instance, IOFilesT* files, 
	int(*compare)(const void* a, const void* b), void* (*read_and_create_generic_data)(FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	int index = search_element_in_array(array_instance, element, compare);

	if (index == -1)
	{
		print_error(ITEM_NOT_FOUND, files->input, NULL);
		return;
	}

	fprintf(files->output, "%d\n", index);
}

void DeleteVector(GenericArrayT** array_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data))
{
	delete_elements_in_array(*array_instance, free_generic_data);
}

void MergeVectors(GenericArrayT** array_destination, GenericArrayT** array_source, IOFilesT* files)
{
	ReturnCodeE return_code = merge_arrays(*array_destination, *array_source);

	switch (return_code)
	{
	case SUCCESS:
		//do nothing
		break;
	case MEMORY_ALLOCATION_ERROR:
		print_error(MEMORY_ALLOCATION_FAILED, files->output, NULL);
	default:
		print_error(UNKNOWN_ERROR_OCCURED, files->output, NULL);
		break;
	}

}

PossibleCommandsE get_command(char* command)
{
		if (strcmp(command, "CreateVector") == 0)
		{
			return CREATE_VECTOR;
		}
		else if (strcmp(command, "PrintVector") == 0)
		{
			return PRINT_VECTOR;
		}
		else if (strcmp(command, "AddVectorItems") == 0)
		{
			return ADD_VECTOR_ITEMS;
		}
		else if (strcmp(command, "PutVectorItem") == 0)
		{
			return PUT_VECTOR_ITEM;
		}
		else if (strcmp(command, "GetVectorItem") == 0)
		{
			return GET_VECTOR_ITEM;
		}
		else if (strcmp(command, "DeleteVectorItem") == 0)
		{
			return DELETE_VECTOR_ITEM;
		}
		else if (strcmp(command, "SearchVectorItem") == 0)
		{
			return SEARCH_VECTOR_ITEM;
		}
		else if (strcmp(command, "SortVector") == 0)
		{
			return SORT_VECTOR;
		}
		else if (strcmp(command, "MergeVectors") == 0)
		{
			return MERGE_VECTORS;
		}
		else if (strcmp(command, "DeleteVector") == 0)
		{
			return DELETE_VECTOR;
		}
		else
		{
			return UNKOWN_COMMAND;
		}


}

void test_generic_array(IOFilesT* files)
{
	//256 arrays, so each character corresponds to a GenericArrayT.
	GenericArrayT** arrays = (GenericArrayT**)calloc(256, sizeof(GenericArrayT*));
	if (arrays == NULL)
	{
		fprintf(files->output, "Fatal error: could not allocate memory for arrays\n");
		fprintf(files->output, "exiting...\n");
		exit(1);
	}

	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, files->input))
	{
		char* command_string = strtok(buffer, " \n");

		//line with spaces or just a new line, so we can skip it
		if (command_string == NULL)
		{
			continue;
		}

		PossibleCommandsE command = get_command(command_string);
		if (command == UNKOWN_COMMAND)
		{
			print_error(UNRECOGNIZED_COMMAND, files->output, command_string);
			continue;
		}

		char* first_instance = strtok(NULL, " \n");
		GenericArrayT** array_instance = get_generic_array(arrays, first_instance, files->output);

		if (array_instance == NULL || 
			(*array_instance == NULL && command != CREATE_VECTOR))
		{
			//instance does not correspond to a valid data structure
			//or instance is not yet created
			print_error(DATA_STRUCTURE_INEXISTENT, files->output, first_instance);
			continue;
		}
		switch (command)
		{
		case CREATE_VECTOR:
			printf("CreateVector\n");
			CreateVector(array_instance, files);
			break;
		case PRINT_VECTOR:
			printf("PrintVector\n");
			PrintVector(*array_instance, files, print_GenericDataT);
			break;
		case ADD_VECTOR_ITEMS:
			printf("AddVectorItems\n");
			AddVectorItems(*array_instance, files, read_and_create_GenericDataT);
			break;
		case PUT_VECTOR_ITEM:
			printf("PutVectorItem\n");
			PutVectorItem(*array_instance, files, read_and_create_GenericDataT);
			break;
		case GET_VECTOR_ITEM:
			printf("GetVectorItem\n");
			GetVectorItem(*array_instance, files, print_GenericDataT);
			break;
		case DELETE_VECTOR_ITEM:
			printf("DeleteVectorItem\n");
			DeleteVectorItem(*array_instance, files, free_GenericDataT);
			break;
		case SEARCH_VECTOR_ITEM:
			printf("SearchVectorItem\n");
			SearchVectorItem(*array_instance, files, compare_GenericDataT, read_and_create_GenericDataT);
			break;
		case SORT_VECTOR:
			printf("SortVector\n");
			SortVector(*array_instance, files, compare_GenericDataT);
			break;
		case MERGE_VECTORS:
		{
			printf("MergeVectors\n");
			char* second_instance = strtok(NULL, " \n");
			GenericArrayT** array_instance2 = get_generic_array(arrays, second_instance, files->output);

			if (array_instance2 == NULL || *array_instance2 == NULL)
			{
				//instance does not correspond to a valid data structure
				//or instance is not yet created
				print_error(DATA_STRUCTURE_INEXISTENT, files->output, first_instance);
				continue;
			}
			MergeVectors(array_instance, array_instance2, files);
			free(*array_instance2);
			*array_instance2 = NULL;
			break;
		}
		case DELETE_VECTOR:
			printf("DeleteVector\n");
			DeleteVector(array_instance, files, free_GenericDataT);
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

	free(arrays);
}
