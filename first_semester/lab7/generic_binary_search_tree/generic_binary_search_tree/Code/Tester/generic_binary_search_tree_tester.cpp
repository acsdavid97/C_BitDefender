#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_binary_search_tree_tester.cpp
file creation date: 2017-01-08 22:16:25
*/


#include "generic_binary_search_tree_tester.h"

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
		fprintf(file, "Error: compare function does not match in trees, cannot merge trees\n");
		break;
	case UNKNOWN_ERROR_OCCURED:
		fprintf(file, "Uknown error.\n");
		break;
	default:
		fprintf(file, "Default-Unkown error.\n");
		break;
	}
}

GenericSearchTreeT** get_generic_tree(GenericSearchTreeT** trees, char* instance, FILE* file)
{
	char letter = instance[0];

	// check if it's valid
	if (strlen(instance) != 1 || letter < 'A' || letter > 'Z')
	{
		return NULL;
	}
	return trees + letter;
}

void CreateBST(GenericSearchTreeT** tree_instance, IOFilesT* files, int(*compare)(const void* a, const void* b))
{
	*tree_instance = create_empty_GenericSearchTreeT(compare);

	if (*tree_instance == NULL)
	{
		print_error(MEMORY_ALLOCATION_FAILED, files->output);
	}
}

void PrintBST(GenericSearchTreeT* tree_instance, IOFilesT* files,
	void(*print_generic_data)(const void *element, FILE* file))
{
	if (get_nr_of_elements_in_tree(tree_instance) <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_elements_in_tree(tree_instance, files->output, print_generic_data);
}

void PreorderBST(GenericSearchTreeT* tree_instance, IOFilesT* files,
	void(*print_generic_data)(const void *element, FILE* file))
{
	if (get_nr_of_elements_in_tree(tree_instance) <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_elements_in_postorder(tree_instance, files->output, print_generic_data);
}
	
void InorderBST(GenericSearchTreeT* tree_instance, IOFilesT* files,
	void(*print_generic_data)(const void *element, FILE* file))
{
	if (get_nr_of_elements_in_tree(tree_instance) <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_elements_in_inorder(tree_instance, files->output, print_generic_data);
}

void PostorderBST(GenericSearchTreeT* tree_instance, IOFilesT* files,
	void(*print_generic_data)(const void *element, FILE* file))
{
	if (get_nr_of_elements_in_tree(tree_instance) <= 0)
	{
		//data structure empty
		print_error(STRUCTURE_IS_EMPTY, files->output);
		return;
	}

	print_elements_in_postorder(tree_instance, files->output, print_generic_data);
}

void AddBSTItem(GenericSearchTreeT* tree_instance, IOFilesT* files, void* (*read_and_create_generic_data)(FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	ReturnCodeE return_code = add_element_in_tree(tree_instance, element);

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
void SearchBSTItem(GenericSearchTreeT* tree_instance, IOFilesT* files, 
	void* (*read_and_create_generic_data)(FILE* file), void(*print_element)(const void *a, FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	TreeNodeT* node_found = search_element_in_tree(tree_instance, element);
	if (node_found == NULL)
	{
		fprintf(files->output, "Element is not in tree\n");
		return;
	}

	print_element(node_found->element, files->output);
	fprintf(files->output, "\n");
}

void DeleteBSTItem(GenericSearchTreeT* tree_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data),
	void* (*read_and_create_generic_data)(FILE* file), void(*print_element)(const void *a, FILE* file))
{
	void* element = read_and_create_generic_data(files->input);
	void* deleted_element = delete_element_from_tree(tree_instance, element);

	if (deleted_element == NULL)
	{
		print_error(ITEM_NOT_FOUND, files->output);
		return;
	}
	
	print_element(deleted_element, files->output);
	fprintf(files->output, "\n");
	free_generic_data(element);
	free(element);
}

void MergeBSTs(GenericSearchTreeT** tree_instance, GenericSearchTreeT**  tree_instance2, IOFilesT* files)
{
	ReturnCodeE return_code = merge_trees(*tree_instance, *tree_instance2);

	switch (return_code)
	{
	case SUCCESS:
		free(*tree_instance2);
		*tree_instance2 = NULL;
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

void HeightBST(GenericSearchTreeT* tree_instance, IOFilesT* files)
{
	int height = height_of_subtree(tree_instance->root);
	fprintf(files->output, "%d\n", height);
}

void DeleteBST(GenericSearchTreeT** tree_instance, IOFilesT* files, void(*free_generic_data)(void* generic_data))
{
	delete_elements_in_tree(*tree_instance, free_generic_data);
	free(*tree_instance);
	*tree_instance = NULL;
}

PossibleCommandsE get_command(char* command)
{
	if (strcmp(command, "CreateBST") == 0)
	{
		return CREATE_TREE;
	}
	else if (strcmp(command, "PrintBST") == 0)
	{
		return PRINT_TREE;
	}
	else if (strcmp(command, "PreorderBST") == 0)
	{
		return PREORDER_TREE;
	}
	else if (strcmp(command, "InorderBST") == 0)
	{
		return INORDER_TREE;
	}
	else if (strcmp(command, "PostorderBST") == 0)
	{
		return POSTORDER_TREE;
	}
	else if (strcmp(command, "AddBSTItem") == 0)
	{
		return ADD_TREE_ITEM;
	}
	else if (strcmp(command, "SearchBSTItem") == 0)
	{
		return SEARCH_TREE_ITEM;
	}
	else if (strcmp(command, "DeleteBSTItem") == 0)
	{
		return DELETE_TREE_ITEM;
	}
	else if (strcmp(command, "MergeBSTs") == 0)
	{
		return MERGE_TREES;
	}
	else if (strcmp(command, "HeightBST") == 0)
	{
		return SUBTREE_HEIGHT;
	}
	else if (strcmp(command, "DeleteBST") == 0)
	{
		return DELETE_TREE;
	}
	else
	{
		return UNKOWN_COMMAND;
	}
}

void test_generic_binary_search_tree(IOFilesT* files)
{
	//256 trees, so each character corresponds to a GenericSearchTreeT.
	GenericSearchTreeT** trees = (GenericSearchTreeT**)calloc(256, sizeof(GenericSearchTreeT*));
	if (trees == NULL)
	{
		fprintf(files->output, "Fatal error: could not allocate memory for trees\n");
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
		if (first_instance == NULL)
		{
			print_error(INSTANCE_UNKNOWN, files->output);
			continue;
		}
		GenericSearchTreeT** tree_instance = get_generic_tree(trees, first_instance, files->output);

		if (tree_instance == NULL || 
			(*tree_instance == NULL && command != CREATE_TREE))
		{
			//instance does not correspond to a valid data structure
			//or instance is not yet created
			print_error(DATA_STRUCTURE_INEXISTENT, files->output);
			continue;
		}
		switch (command)
		{
		case CREATE_TREE:
			printf("CreateBST\n");
			CreateBST(tree_instance, files, compare_GenericDataT);
			break;
		case PRINT_TREE:
			printf("PrintBST\n");
			PrintBST(*tree_instance, files, print_GenericDataT);
			break;
		case PREORDER_TREE:
			printf("PreorderBST\n");
			PreorderBST(*tree_instance, files, print_GenericDataT);
			break;
		case INORDER_TREE:
			printf("InorderBST\n");
			InorderBST(*tree_instance, files, print_GenericDataT);
			break;
		case POSTORDER_TREE:
			printf("PostorderBST\n");
			PostorderBST(*tree_instance, files, print_GenericDataT);
			break;
		case ADD_TREE_ITEM:
			printf("AddBSTItem\n");
			AddBSTItem(*tree_instance, files, read_and_create_GenericDataT);
			break;
		case SEARCH_TREE_ITEM:
			printf("SearchBSTItem\n");
			SearchBSTItem(*tree_instance, files, read_and_create_GenericDataT, print_GenericDataT);
			break;
		case DELETE_TREE_ITEM:
			printf("DeleteBSTItem\n");
			DeleteBSTItem(*tree_instance, files, free_GenericDataT, read_and_create_GenericDataT, print_GenericDataT);
			break;
		case MERGE_TREES:
		{
			printf("MergeBSTs\n");
			char* second_instance = strtok(NULL, " \n");
			if (second_instance == NULL)
			{
				print_error(INSTANCE_UNKNOWN, files->output);
				continue;
			}

			GenericSearchTreeT** tree_instance2 = get_generic_tree(trees, second_instance, files->output);

			if (tree_instance2 == NULL || *tree_instance2 == NULL)
			{
				//instance does not correspond to a valid data structure
				//or instance is not yet created
				print_error(DATA_STRUCTURE_INEXISTENT, files->output);
				continue;
			}
			MergeBSTs(tree_instance, tree_instance2, files);
			break;
		}
		case SUBTREE_HEIGHT:
			printf("HeightBST\n");
			HeightBST(*tree_instance, files);
			break;
		case DELETE_TREE:
			printf("DeleteBST\n");
			DeleteBST(tree_instance, files, free_GenericDataT);
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
	free(trees);
}
