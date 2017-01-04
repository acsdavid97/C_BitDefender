#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_linked_list_tester.cpp
file creation date: 2017-01-04 22:16:25
*/

#include "generic_linked_list_tester.h"

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

GenericLinkedListT** get_generic_linked_list(GenericLinkedListT** lists, char* instance, FILE* file)
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
	return lists + letter;
}

void add_element_at_index_with_errors(GenericLinkedListT* list, void* element, int index, FILE* file)
{
	NodeT* node = create_NodeT(element);
	if (node == NULL)
	{
		print_error(MEMORY_ALLOCATION_FAILED, file);
		return;
	}

	ReturnCodeE return_code = add_node_at_index(list, node, index);

	switch (return_code)
	{
	case SUCCESS:
		//do nothing
		break;
	case MEMORY_ALLOCATION_ERROR:
		print_error(MEMORY_ALLOCATION_FAILED, file);
		break;
	case INCORRECT_INDEX:
		print_error(INDEX_OUT_OF_BOUNDS, file);
		break;
	default:
		print_error(UNKNOWN_ERROR_OCCURED, file);
		break;
	}
}

void CreateLinkedList(GenericLinkedListT** list_instance, IOFilesT* files)
{
	*list_instance = create_empty_GenericLinkedListT();

	if (*list_instance == NULL)
	{
		print_error(MEMORY_ALLOCATION_FAILED, files->output);
	}
}
//TODO add functions
void PrintLinkedList(GenericLinkedListT* list_instance, IOFilesT* files,
	void(*print_generic_data)(const void *element, FILE* file))
{
	if (list_instance->length <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_linked_list(list_instance, files->output, print_generic_data);
}

void AddLinkedListItem(GenericLinkedListT* list_instance, IOFilesT* files, void* (*read_and_create_generic_data)(FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	add_element_at_index_with_errors(list_instance, element, list_instance->length, files->output);
}
void PutLinkedListItem(GenericLinkedListT* list_instance, IOFilesT* files, void* (*read_and_create_generic_data)(FILE* file))
{
	int index = 0;
	if (fscanf(files->input, "%d", &index))
	{
		void* element = read_and_create_generic_data(files->input);
		add_element_at_index_with_errors(list_instance, element, index, files->output);
	}
	else
	{
		//could not read index, maybe it's missing?
		print_error(MISSING_VALUE, files->output);
	}
}

void GetLinkedListItem(GenericLinkedListT* list_instance, IOFilesT* files, 
	void(*print_element)(const void *data, FILE* file))
{
	int index = 0;
	if (fscanf(files->input, "%d", &index))
	{
		NodeT* node = get_node_at_index(list_instance, index);
		if (node == NULL)
		{
			print_error(INDEX_OUT_OF_BOUNDS, files->output);
			return;
		}
		void* element = node->data;
		print_element(element, files->output);
		fprintf(files->output, "\n");
	}
	else
	{
		//could not read index, maybe it's missing?
		print_error(MISSING_VALUE, files->output);
	}
}

void DeleteLinkedListItem(GenericLinkedListT* list_instance, IOFilesT* files, 
	int(*compare)(const void* a, const void *b), void(*print_data)(const void* data, FILE* file))
{
	void* data = read_and_create_GenericDataT(files->input);
	void* element = search_and_delete_element_in_list(list_instance, data, compare);

	if (element == NULL)
	{
		print_error(ITEM_NOT_FOUND, files->output);
	}

	print_data(element, files->output);
}

void SortLinkedList(GenericLinkedListT* list_instance, IOFilesT* files, int(*compare)(const void* a, const void* b))
{
	sort_elements_in_list(list_instance, compare);
}

void SearchLinkedListItem(GenericLinkedListT* list_instance, IOFilesT* files, int(*compare)(const void* a, const void* b), 
	void* (*read_and_create_generic_data)(FILE* file), void(*print_data)(const void* data, FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	void* element_found = search_element_in_list(list_instance, element, compare);

	if (element_found == NULL)
	{
		print_error(ITEM_NOT_FOUND, files->input);
		return;
	}

	print_data(element_found, files->output);
	fprintf(files->output, "\n");

}

void DeleteLinkedList(GenericLinkedListT** list_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data))
{
	delete_linked_list(*list_instance, free_generic_data);
	free(*list_instance);
	*list_instance = NULL;
}

void MergeLinkedLists(GenericLinkedListT* list_destination, GenericLinkedListT* list_source, IOFilesT* files)
{
	merge_linked_lists(list_destination, list_source);
}

PossibleCommandsE get_command(char* command)
{
	if (strcmp(command, "CreateLinkedList") == 0)
	{
		return CREATE_LINKED_LIST;
	}
	else if (strcmp(command, "PrintLinkedList") == 0)
	{
		return PRINT_LINKED_LIST;
	}
	else if (strcmp(command, "AddLinkedListItem") == 0)
	{
		return ADD_LINKED_LIST_ITEM;
	}
	else if (strcmp(command, "PutLinkedListItem") == 0)
	{
		return PUT_LINKED_LIST_ITEM;
	}
	else if (strcmp(command, "GetLinkedListItem") == 0)
	{
		return GET_LINKED_LIST_ITEM;
	}
	else if (strcmp(command, "DeleteLinkedListItem") == 0)
	{
		return DELETE_LINKED_LIST_ITEM;
	}
	else if (strcmp(command, "SearchLinkedListItem") == 0)
	{
		return SEARCH_LINKED_LIST_ITEM;
	}
	else if (strcmp(command, "SortLinkedList") == 0)
	{
		return SORT_LINKED_LIST;
	}
	else if (strcmp(command, "MergeLinkedLists") == 0)
	{
		return MERGE_LINKED_LISTS;
	}
	else if (strcmp(command, "DeleteLinkedList") == 0)
	{
		return DELETE_LINKED_LIST;
	}
	else
	{
		return UNKOWN_COMMAND;
	}


}

void test_generic_linked_list(IOFilesT* files)
{
	//256 arrays, so each character corresponds to a GenericArrayT.
	GenericLinkedListT** lists = (GenericLinkedListT**)calloc(256, sizeof(GenericLinkedListT*));
	if (lists == NULL)
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
			fprintf(files->output, "Error: Unrecognized command: %s\n", command_string);
			continue;
		}

		char* first_instance = strtok(NULL, " \n");
		GenericLinkedListT** list_instance = get_generic_linked_list(lists, first_instance, files->output);

		if (list_instance == NULL ||
			(*list_instance == NULL && command != CREATE_LINKED_LIST))
		{
			//instance does not correspond to a valid data structure
			//or instance is not yet created
			print_error(DATA_STRUCTURE_INEXISTENT, files->output);
			continue;
		}
		switch (command)
		{
		case CREATE_LINKED_LIST:
			printf("CreateLinkedList\n");
			CreateLinkedList(list_instance, files);
			break;
		case PRINT_LINKED_LIST:
			printf("PrintLinkedList\n");
			PrintLinkedList(*list_instance, files, print_GenericDataT);
			break;
		case ADD_LINKED_LIST_ITEM:
			printf("AddLinkedListItems\n");
			AddLinkedListItem(*list_instance, files, read_and_create_GenericDataT);
			break;
		case PUT_LINKED_LIST_ITEM:
			printf("PutLinkedListItem\n");
			PutLinkedListItem(*list_instance, files, read_and_create_GenericDataT);
			break;
		case GET_LINKED_LIST_ITEM:
			printf("GetLinkedListItem\n");
			GetLinkedListItem(*list_instance, files, print_GenericDataT);
			break;
		case DELETE_LINKED_LIST_ITEM:
			printf("DeleteLinkedListItem\n");
			DeleteLinkedListItem(*list_instance, files, compare_GenericDataT, print_GenericDataT);
			break;
		case SEARCH_LINKED_LIST_ITEM:
			printf("SearchLinkedListItem\n");
			SearchLinkedListItem(*list_instance, files, compare_GenericDataT,
				read_and_create_GenericDataT, print_GenericDataT);
			break;
		case SORT_LINKED_LIST:
			printf("SortLinkedList\n");
			SortLinkedList(*list_instance, files, compare_GenericDataT);
			break;
		case MERGE_LINKED_LISTS:
		{
			printf("MergeLinkedLists\n");
			char* second_instance = strtok(NULL, " \n");
			GenericLinkedListT** list_instance2 = get_generic_linked_list(lists, second_instance, files->output);

			if (list_instance2 == NULL || *list_instance2 == NULL)
			{
				//instance does not correspond to a valid data structure
				//or instance is not yet created
				print_error(DATA_STRUCTURE_INEXISTENT, files->output);
				continue;
			}
			MergeLinkedLists(*list_instance, *list_instance2, files);
			free(*list_instance2);
			*list_instance2 = NULL;
			break;
		}
		case DELETE_LINKED_LIST:
			printf("DeleteLinkedList\n");
			DeleteLinkedList(list_instance, files, free_GenericDataT);
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

	free(lists);
}
