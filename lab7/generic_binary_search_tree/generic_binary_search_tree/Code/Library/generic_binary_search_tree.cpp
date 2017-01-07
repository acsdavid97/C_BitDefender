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

TreeNodeT* create_TreeNodeT(void* element)
{
	TreeNodeT* empty_node = (TreeNodeT*)malloc(sizeof(TreeNodeT));
	if (empty_node == NULL)
	{
		return NULL;
	}
	empty_node->element = element;
	empty_node->left = NULL;
	empty_node->right = NULL;

	return empty_node;
}

TreeNodeT* insert_element_recursively_in_tree(TreeNodeT* subtree_root, TreeNodeT* node_to_insert, int(*compare)(const void* a, const void* b))
{
	if (subtree_root == NULL)
	{
		return node_to_insert;
	}

	if (compare(node_to_insert->element, subtree_root->element) < 0)
	{
		subtree_root->left = insert_element_recursively_in_tree(subtree_root->left, node_to_insert, compare);
	}
	else
	{
		subtree_root->right = insert_element_recursively_in_tree(subtree_root->right, node_to_insert, compare);
	}

	return subtree_root;// return original root
}

ReturnCodeE add_element_in_tree(GenericSearchTreeT* tree, void* element)
{
	TreeNodeT* node_to_insert = create_TreeNodeT(element);
	if (node_to_insert == NULL)
	{
		return MEMORY_ALLOCATION_ERROR;
	}

	tree->root = insert_element_recursively_in_tree(tree->root, node_to_insert, tree->compare);
	tree->nr_of_elements++;
	return SUCCESS;
}

TreeNodeT* search_element_recursively_in_tree(TreeNodeT* subtree_root, void* element, int(*compare)(const void* a, const void* b))
{
	if (subtree_root == NULL)
	{
		return NULL;
	}

	if (compare(element, subtree_root->element) == 0)
	{
		//element found
		return subtree_root;
	}

	if (compare(element, subtree_root->element) < 0)
	{
		//element in left subtree
		return search_element_recursively_in_tree(subtree_root->left, element, compare);
	}
	else
	{
		//element in right subtree
		return search_element_recursively_in_tree(subtree_root->right, element, compare);
	}
}

TreeNodeT* search_element_in_tree(GenericSearchTreeT* tree, void* element)
{
	return search_element_recursively_in_tree(tree->root, element, tree->compare);
}

void* delete_element_from_tree(GenericSearchTreeT* tree, void* element)
{
	//TODO: implement
	return NULL;
}

void print_elements_recursively_in_tree(TreeNodeT* subtree_root, FILE* file, int level, void(*print_element)(const void *a, FILE* file))
{
	if (subtree_root == NULL)
	{
		return;
	}

	print_elements_recursively_in_tree(subtree_root->right, file, level + 1, print_element);
	for (int i = 0; i < level; i++)
	{
		fprintf(file, "  ");
	}
	print_element(subtree_root->element, file);
	fprintf(file, "\n");
	print_elements_recursively_in_tree(subtree_root->left, file, level + 1, print_element);
}

void print_elements_in_tree(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_in_tree(tree->root, file, 0, print_element);
}

void print_elements_recursively_preorder(TreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	if (subtree_root == NULL)
	{
		return;
	}

	print_element(subtree_root->element, file);
	print_elements_recursively_preorder(subtree_root->left, file, print_element);
	print_elements_recursively_preorder(subtree_root->right, file, print_element);
}

void print_elements_in_preorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_preorder(tree->root, file, print_element);
	fprintf(file, "\n");
}

void print_elements_recursively_inorder(TreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	if (subtree_root == NULL)
	{
		return;
	}

	print_elements_recursively_inorder(subtree_root->left, file, print_element);
	print_element(subtree_root->element, file);
	print_elements_recursively_inorder(subtree_root->right, file, print_element);
}

void print_elements_in_inorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_inorder(tree->root, file, print_element);
	fprintf(file, "\n");
}

void print_elements_recursively_postorder(TreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	if (subtree_root == NULL)
	{
		return;
	}

	print_elements_recursively_postorder(subtree_root->left, file, print_element);
	print_elements_recursively_postorder(subtree_root->right, file, print_element);
	print_element(subtree_root->element, file);
}

void print_elements_in_postorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_postorder(tree->root, file, print_element);
	fprintf(file, "\n");
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

int inline max_of_ints(int a, int b)
{
	return (a > b) ? a : b;
}

int height_of_subtree(TreeNodeT* subtree_root)
{
	if (subtree_root == NULL)
	{
		return 0;
	}

	int left_height = height_of_subtree(subtree_root->left);
	int right_height = height_of_subtree(subtree_root->right);
	
	return max_of_ints(left_height + 1, right_height + 1);
}

void delete_elements_recursively_in_tree(TreeNodeT* subtree_root, void(*free_generic_data)(void* generic_data))
{
	if (subtree_root == NULL)
	{
		return;
	}

	delete_elements_recursively_in_tree(subtree_root->left, free_generic_data);
	delete_elements_recursively_in_tree(subtree_root->right, free_generic_data);

	//delete node
	free_generic_data(subtree_root->element);
	free(subtree_root->element);
	free(subtree_root);
}

void delete_elements_in_tree(GenericSearchTreeT* tree, void(*free_generic_data)(void* generic_data))
{
	delete_elements_recursively_in_tree(tree->root, free_generic_data);
	tree->nr_of_elements = 0;
	tree->root = NULL;
	tree->compare = NULL;
}
