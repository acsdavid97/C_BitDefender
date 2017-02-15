#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_binary_search_tree.cpp
file creation date: 2017-01-08 19:48:52
*/

#include "generic_balanced_search_tree.h"

GenericBalancedSearchTreeT* create_empty_GenericBalancedSearchTreeT(int(*compare)(const void* a, const void* b))
{
	if (compare == NULL)
	{
		// tree is useless without a compare function.
		return NULL;
	}

	GenericBalancedSearchTreeT* empty_tree = (GenericBalancedSearchTreeT*)malloc(sizeof(GenericBalancedSearchTreeT));
	if (empty_tree == NULL)
	{
		return NULL;
	}

	empty_tree->compare = compare;
	empty_tree->root = NULL;
	empty_tree->nr_of_elements = 0;

	return empty_tree;
}

int get_nr_of_elements_in_tree(GenericBalancedSearchTreeT* tree)
{
	return tree->nr_of_elements;
}

BTreeNodeT* create_BTreeNodeT(void* element)
{
	BTreeNodeT* empty_node = (BTreeNodeT*)malloc(sizeof(BTreeNodeT));
	if (empty_node == NULL)
	{
		return NULL;
	}
	empty_node->element = element;
	empty_node->left = NULL;
	empty_node->right = NULL;

	return empty_node;
}


ReturnCodeE add_element_in_tree(GenericBalancedSearchTreeT* tree, void* element)
{
	BTreeNodeT* node_to_insert = create_BTreeNodeT(element);
	if (node_to_insert == NULL)
	{
		return MEMORY_ALLOCATION_ERROR;
	}

	//TODO: implement
	tree->nr_of_elements++;
	return SUCCESS;
}

BTreeNodeT* search_element_recursively_in_tree(BTreeNodeT* subtree_root, void* element, int(*compare)(const void* a, const void* b))
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

BTreeNodeT* search_element_in_tree(GenericBalancedSearchTreeT* tree, void* element)
{
	return search_element_recursively_in_tree(tree->root, element, tree->compare);
}

BTreeNodeT* find_leftmost_subnode(BTreeNodeT* subtree_root)
{
	if (subtree_root->left == NULL)
	{
		return subtree_root;
	}

	return find_leftmost_subnode(subtree_root->left);
}

BTreeNodeT* find_rightmost_subnode(BTreeNodeT* subtree_root)
{
	if (subtree_root->right == NULL)
	{
		return subtree_root;
	}

	return find_leftmost_subnode(subtree_root->right);
}

void* delete_element_from_tree(GenericBalancedSearchTreeT* tree, void* element)
{
	void* deleted_element = NULL;

	//TODO: implement
	
	if (deleted_element == NULL)
	{
		return NULL;
	}

	return deleted_element;
}

void print_elements_recursively_in_tree(BTreeNodeT* subtree_root, FILE* file, int level, void(*print_element)(const void *a, FILE* file))
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

void print_elements_in_tree(GenericBalancedSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_in_tree(tree->root, file, 0, print_element);
}

void print_elements_recursively_preorder(BTreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	if (subtree_root == NULL)
	{
		return;
	}

	print_element(subtree_root->element, file);
	print_elements_recursively_preorder(subtree_root->left, file, print_element);
	print_elements_recursively_preorder(subtree_root->right, file, print_element);
}

void print_elements_in_preorder(GenericBalancedSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_preorder(tree->root, file, print_element);
	fprintf(file, "\n");
}

void print_elements_recursively_inorder(BTreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	if (subtree_root == NULL)
	{
		return;
	}

	print_elements_recursively_inorder(subtree_root->left, file, print_element);
	print_element(subtree_root->element, file);
	print_elements_recursively_inorder(subtree_root->right, file, print_element);
}

void print_elements_in_inorder(GenericBalancedSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_inorder(tree->root, file, print_element);
	fprintf(file, "\n");
}

void print_elements_recursively_postorder(BTreeNodeT* subtree_root, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	if (subtree_root == NULL)
	{
		return;
	}

	print_elements_recursively_postorder(subtree_root->left, file, print_element);
	print_elements_recursively_postorder(subtree_root->right, file, print_element);
	print_element(subtree_root->element, file);
}

void print_elements_in_postorder(GenericBalancedSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recursively_postorder(tree->root, file, print_element);
	fprintf(file, "\n");
}

void merge_trees_recursively(BTreeNodeT* subtree_root, GenericBalancedSearchTreeT* destination_tree)
{
	if (subtree_root == NULL)
	{
		return;
	}

	merge_trees_recursively(subtree_root->left, destination_tree);
	add_element_in_tree(destination_tree, subtree_root->element);
	merge_trees_recursively(subtree_root->right, destination_tree);
}


ReturnCodeE merge_trees(GenericBalancedSearchTreeT* tree_destination, GenericBalancedSearchTreeT* tree_source)
{
	if (tree_destination->compare != tree_source->compare)
	{
		return COMPARE_FUNCTION_MISMATCH;
	}

	merge_trees_recursively(tree_source->root, tree_destination);
	
	return SUCCESS;
}

int inline max_of_ints(int a, int b)
{
	return (a > b) ? a : b;
}

int height_of_subtree(BTreeNodeT* subtree_root)
{
	if (subtree_root == NULL)
	{
		return 0;
	}

	int left_height = height_of_subtree(subtree_root->left);
	int right_height = height_of_subtree(subtree_root->right);
	
	return max_of_ints(left_height + 1, right_height + 1);
}

void delete_elements_recursively_in_tree(BTreeNodeT* subtree_root, void(*free_generic_data)(void* generic_data))
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

void delete_elements_in_tree(GenericBalancedSearchTreeT* tree, void(*free_generic_data)(void* generic_data))
{
	delete_elements_recursively_in_tree(tree->root, free_generic_data);
	tree->nr_of_elements = 0;
	tree->root = NULL;
	tree->compare = NULL;
}
