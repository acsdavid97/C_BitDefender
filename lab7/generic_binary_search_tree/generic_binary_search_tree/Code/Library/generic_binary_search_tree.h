/*
author: Acs David
file name: generic_binary_search_tree.h
file creation date: 2017-01-07 13:26:24
*/

#ifndef GENERIC_BINARY_SEARCH_TREE_H
#define GENERIC_BINARY_SEARCH_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "return_type_enum.h"

//struct for each node in tree
typedef struct TreeNodeTag {
	void* element;
	struct TreeNodeTag* left;
	struct TreeNodeTag* right;
}TreeNodeT;

typedef struct GenericSearchTreeTag {
	TreeNodeT* root;
	int nr_of_elements;
	int(*compare)(const void* a, const void* b); // compare function, used to keep the tree invariant.
}GenericSearchTreeT;

// define your functions here

/*
	Creates an empty generic tree.

	@return:  pointer to new generic tree, if memory allocation failed returns NULL.
*/
GenericSearchTreeT* create_empty_GenericSearchTreeT(int(*compare)(const void* a, const void* b));

/*
	Returns the number of elements in the tree.

	@return: number of elements in the tree.
*/
int get_nr_of_elements_in_tree(GenericSearchTreeT* tree);

/*
	Adds an element in tree.
*/
ReturnCodeE add_element_in_tree(GenericSearchTreeT* tree, void* element);

/*
	Searches for an element in tree, uses the compare fucntion defined in tree.

	@return MinElementT*: pointer to a struct containing the element found, and it's index.
	If element is not found, returns NULL.
*/
TreeNodeT* search_element_in_tree(GenericSearchTreeT* tree, void* element);

/*
	Deletes an element from the tree.

	@return pointer to deleted element, if element is not found returns NULL.
*/
void* delete_element_from_tree(GenericSearchTreeT* tree, void* element);

/*
	Prints all the elements of tree in tree format, using the function print_element

	@param file: output to be written to. Should not be NULL, should be opened in write mode.
*/
void print_elements_in_tree(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file));

/*
	Prints elements in tree, in preorder.
*/
void print_elements_in_preorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file));

/*
	Prints elements in tree, in inorder.
*/
void print_elements_in_inorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file));

/*
	Prints elements in tree, in postorder.
*/
void print_elements_in_postorder(GenericSearchTreeT* tree, FILE* file, void(*print_element)(const void *a, FILE* file));

/*
	Merges two trees. The compare function of the two trees should be identical.

	@param tree_destination: tree where the merged tree will be found after a successful operation.

	@param tree_source: tree from where data will be added to tree_destination. Should be freed after this operation.

	@return SUCCESS: operation successful.
	@return MEMORY_ALLOCATION_ERROR: could not resize tree to hold all elements.
	Everything remains as it was before attempting the operation
*/
ReturnCodeE merge_trees(GenericSearchTreeT* tree_destination, GenericSearchTreeT* tree_source);

/*
	Returns the height of the specified subtree.

	@return: height of subtree
*/
int height_of_subtree(TreeNodeT* subtree_root);

/*
	Frees all elements inside tree.
	Sets tree->nr_of_elements to zero, tree->compare and tree->root to NULL.
	GenericSearchTreeT* tree should be freed afterwards.

	@param tree: tree from which all elements will be deleted.

	@param free_generic_data: a pointer to a function, which can free an instance of generic_data contained in the tree.
*/
void delete_elements_in_tree(GenericSearchTreeT* tree, void(*free_generic_data)(void* generic_data));

#endif
