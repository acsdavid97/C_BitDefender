#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: pagini_aurii.cpp
file creation date: 2016-12-13 09:30:12
*/

#include "pagini_aurii.h"

EntryT* create_EntryT(char *name, char *surname, char *telephone, char *address)
{
	EntryT *new_nodet = (EntryT*)malloc(sizeof(EntryT));
	new_nodet->name = name;
	new_nodet->surname = surname;
	new_nodet->telephone = telephone;
	new_nodet->address = address;
	
	return new_nodet;
}

void print_EntryT(EntryT* node)
{
	printf("surname: %s; name: %s; telephone: %s; address: %s;\n",
		node->surname, node->name, node->telephone, node->address);
}

NodeT* create_NodeT(EntryT* entry)
{
	NodeT* newnode = (NodeT*)malloc(sizeof(NodeT));
	newnode->left = newnode->right = NULL;
	newnode->entry = entry;
	
	return newnode;
}


NodeT* insert(NodeT* toinsert, NodeT* node)
{
	if (node == NULL)
		return toinsert;

	char node_name[1024];
	strcpy(node_name, node->entry->surname);
	strcat(node_name, node->entry->name);

	char toinsert_name[1024];
	strcpy(toinsert_name, toinsert->entry->surname);
	strcat(toinsert_name, toinsert->entry->name);

	if (_stricmp(node_name, toinsert_name) < 0)
		node->right = insert(toinsert, node->right);
	else
		if (_stricmp(node_name, toinsert_name) > 0)
			node->left = insert(toinsert, node->left);
		else
			printf("Node exists\n");
	return node;
}
void print_tree(NodeT* node, int zero_curent_print)
{
	static int curent_print;
	if (node == NULL)
		return;
	//in order tree traversal

	if (zero_curent_print)
		curent_print = 0;
	print_tree(node->left, 0);
	if (curent_print % 10 == 0 && zero_curent_print == 0)
	{
		system("pause");
	}
	print_EntryT(node->entry);
	curent_print++;
	print_tree(node->right, 0);
}


void printtree(NodeT* node)
{
	print_tree(node, 1);
}

NodeT* deletenode(NodeT* parent, NodeT* curr, NodeT* root, char *to_delete_name, int dir)//dir=1 right //dir=0 root //dir=-1 left
{
	if (curr == NULL)
	{
		printf("node to delete not found");
		return root;
	}

	char current_name[1024];
	strcpy(current_name, curr->entry->surname);
	strcat(current_name, curr->entry->name);

	if (_stricmp(to_delete_name, current_name) < 0)
		return deletenode(curr, curr->left, root, to_delete_name, -1);
	if (_stricmp(to_delete_name, current_name) > 0)
		return deletenode(curr, curr->right, root, to_delete_name, 1);
	if (curr == root&&root->left == NULL&&root->right == NULL)
		return NULL;
	if (curr->left == NULL&&curr->right == NULL)
	{
		if (dir == -1)
			parent->left = NULL;
		if (dir == 1)
			parent->right = NULL;
		return root;
	}
	if (curr->left == NULL)
	{
		if (dir == 1)
			parent->right = curr->right;
		if (dir == -1)
			parent->left = curr->right;
		return root;
	}
	if (curr->right == NULL)
	{
		if (dir == 1)
			parent->right = curr->left;
		if (dir == -1)
			parent->left = curr->left;
		free(curr);
		return root;
	}
	NodeT* pWalker = curr;
	if (dir == 1)
	{
		while (pWalker->left != NULL)
			pWalker = pWalker->left;
		if (curr != root)
			parent->right = pWalker;
	}

	else
	{
		while (pWalker->right != NULL)
			pWalker = pWalker->right;
		parent->left = pWalker;
	}
	return root;
}

NodeT* delete_node_wraper(NodeT* root, char *name)
{
	return deletenode(NULL, root, root, name, 0);
}

NodeT* find(char *name, NodeT* root)
{
	if (root == NULL)
		return NULL;

	char current_name[1024];
	strcpy(current_name, root->entry->surname);
	strcat(current_name, root->entry->name);
	
	if (_stricmp(name, current_name) < 0)
		return find(name, root->left);
	if (_stricmp(name, current_name) > 0)
		return find(name, root->right);
	return root;
}

char* print_read(char *to_print)
{
	printf("%s: ", to_print);
	char buffer[1024];
	gets_s(buffer);
	char *to_return = (char*)malloc(sizeof(char) * strlen(buffer));
	strcpy(to_return, buffer);

	return to_return;
}

NodeT* read_entry()
{
	char *surname = print_read("surname");
	char *name = print_read("name");
	char *telephone = print_read("telephone");
	char *address = print_read("address");
	
	EntryT* entry = create_EntryT(name, surname, telephone, address);
	NodeT* node = create_NodeT(entry);
	return node;
}

void print_menu()
{
	printf("\n---------------------------------------\n");
	printf("1. add new entry\n");
	printf("2. delete an entry\n");
	printf("3. search by name and surname\n");
	printf("4. search by telephone number\n");
	printf("5. print all with specified surname\n");
	printf("6. print all data, page by page\n");
	printf("0. exit\n");
	printf("---------------------------------------\n");
}

NodeT* search_by_telephone_number(NodeT* root, char *telephone)
{
	if (root == NULL)
		return NULL;
	if (strcmp(root->entry->telephone, telephone) == 0)
		return root;
	//in order tree traversal
	NodeT* left = search_by_telephone_number(root->left, telephone);
	if (left != NULL)
		return left;
	NodeT* rigth = search_by_telephone_number(root->right, telephone);
	if (rigth != NULL)
		return rigth;

	return NULL;
}

void print_all_with_same_surname(NodeT* node, char *surname)
{
	if (node == NULL)
		return;
	if (strcmp(node->entry->surname, surname) < 0)
	{
		print_all_with_same_surname(node->left, surname);
	}
	if (strcmp(node->entry->surname, surname) > 0)
	{
		print_all_with_same_surname(node->right, surname);
	}

	if (strcmp(node->entry->surname, surname) == 0)
	{
		print_EntryT(node->entry);
		print_all_with_same_surname(node->left, surname);
		print_all_with_same_surname(node->right, surname);
	}
}