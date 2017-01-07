#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_binary_search_tree.cpp
file creation date: 2017-01-08 19:48:52
*/

#include "generic_binary_search_tree.h"

GenericSearchTreeT* create_empty_GenericSearchTreeT(int(*compare)(const void* a, const void* b))
{
	if (compare == NULL)
	{
		// tree is useless without a compare function.
		return NULL;
	}

	GenericSearchTreeT* empty_tree = (GenericSearchTreeT*)malloc(sizeof(GenericSearchTreeT));
	if (empty_tree == NULL)
	{
		return NULL;
	}

	empty_tree->compare = compare;
	empty_tree->root = NULL;
	empty_tree->nr_of_elements = 0;

	return empty_tree;
}

int get_nr_of_elements_in_tree(GenericSearchTreeT* tree)
{
	return tree->nr_of_elements;
}

ReturnCodeE add_element_in_tree(GenericSearchTreeT* tree, void* element)
{
	//TODO: implement
	return SUCCESS;
}

TreeNodeT* search_element_recursively_in_tree(TreeNodeT* subtree_root, void* element)
{
	//TODO: implement
	return NULL;
}

TreeNodeT* search_element_in_tree(GenericSearchTreeT* tree, void* element)
{
	return search_element_recursively_in_tree(tree->root, element);
}

void* delete_element_from_tree(GenericSearchTreeT* tree, void* element)
{
	//TODO: implement
	return NULL;
}

void print_elements_in_tree(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	//TODO: implement
}

void print_elements_recursively_preorder(TreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	//TODO: implement
}

void print_elements_in_preorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_preorder(tree->root, file, print_element);
}

void print_elements_recursively_inorder(TreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	//TODO: implement
}

void print_elements_in_inorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_inorder(tree->root, file, print_element);
}

void print_elements_recursively_postorder(TreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	//TODO: implement
}

void print_elements_in_postorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_inorder(tree->root, file, print_element);
}


ReturnCodeE merge_trees(GenericSearchTreeT* tree_destination, GenericSearchTreeT* tree_source)
{
	if (tree_destination->compare != tree_source->compare)
	{
		return COMPARE_FUNCTION_MISMATCH;
	}

	//TODO: implement
	return SUCCESS;
}

int height_of_subtree(TreeNodeT* subtree_root)
{
	//TODO: implement
	return 0;
}

void delete_elements_recursively_in_tree(TreeNodeT* subtree_root, void(*free_generic_data)(void* generic_data))
{
	//TODO: implement
}

void delete_elements_in_tree(GenericSearchTreeT* tree, void(*free_generic_data)(void* generic_data))
{
	//TODO: implement
}
