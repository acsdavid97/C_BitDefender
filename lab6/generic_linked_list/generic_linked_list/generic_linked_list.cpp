#define _CRT_SECURE_NO_WARNINGS

/*
author: David
file name: generic_linked_list
file creation date: 2016-12-14 15:10:43
*/

#include "generic_linked_list.h"

GenericDataT* create_GenericDataT(char* surname, char* name, char* address, char* city, char* telephone, char* email)
{
	GenericDataT* generic_data = (GenericDataT*)malloc(sizeof(GenericDataT));
	generic_data->surname = surname;
	generic_data->name = name;
	generic_data->address = address;
	generic_data->city = city;
	generic_data->telephone = telephone;
	generic_data->email = email;

	return generic_data;
}

void print_GenericDataT(GenericDataT* data)
{
	printf("surname: %s; name: %s; address: %s; city %s; telephone: %s; email: %s\n",
		data->surname, data->name, data->address, data->city, data->telephone, data->email);
}

NodeT* create_NodeT(GenericDataT* data)
{
	NodeT* node = (NodeT*)malloc(sizeof(NodeT));
	node->data = data;
	node->next = NULL;

	return node;
}

GenericLinkedListT* create_empty_GenericLinkedListT()
{
	GenericLinkedListT* empty_list = (GenericLinkedListT*)malloc(sizeof(GenericLinkedListT));
	empty_list->head = empty_list->tail = NULL;
	empty_list->length = 0;

	return empty_list;
}

int get_list_length(GenericLinkedListT* list)
{
	return list->length;
}

int add_node_at_end(GenericLinkedListT* list, NodeT* node_to_insert)
{
	//first element to insert
	if (list->head == NULL)
	{
		list->head = list->tail = node_to_insert;
		list->head->next = NULL;
		list->length = 1;
		return 1;
	}

	list->tail->next = node_to_insert;
	list->tail = node_to_insert;
	list->length++;

	return 1;
}

int add_node_at_beginning(GenericLinkedListT* list, NodeT* node_to_insert)
{
	if (list->head == NULL)
	{
		list->head = list->tail = node_to_insert;
		list->head->next = NULL;
		list->length = 1;

		return 1;
	}

	node_to_insert->next = list->head;
	list->head = node_to_insert;
	list->length++;

	return 1;
}

int add_node_at_index(GenericLinkedListT* list, NodeT* node_to_insert, int index)
{
	if (index < 0 || index > list->length)
		return 0;

	if (index == 0)
	{
		return add_node_at_beginning(list, node_to_insert);
	}

	if (index == list->length)
	{
		return add_node_at_end(list, node_to_insert);
	}

	NodeT* pWalker = list->head;
	int i = 1;

	while (pWalker->next != NULL)
	{
		if (i == index)
		{
			node_to_insert->next = pWalker->next;
			pWalker->next = node_to_insert;
			list->length++;

			return 1;
		}

		pWalker = pWalker->next;
		i++;
	}
	
}

void print_linked_list(GenericLinkedListT* list, void (*print_data)(GenericDataT* data))
{
	NodeT* pWalker = list->head;
	while (pWalker != NULL)
	{
		print_data(pWalker->data);
		pWalker = pWalker->next;
	}
}

NodeT* get_node_at_index(GenericLinkedListT* list, int index)
{
	if (index < 0 || index > list->length)
		return NULL;

	NodeT* pWalker = list->head;
	int i = 0;

	while (pWalker != NULL)
	{
		if (i == index)
			return pWalker;
		pWalker = pWalker->next;
		i++;
	}

	return NULL;
}

int remove_node_from_end(GenericLinkedListT* list)
{
	if (list->head == NULL)
		return 0;

	//if head is the only node
	if (list->head->next == NULL)
	{
		free(list->head->data);
		free(list->head);
		list->head = list->tail = NULL;
		list->length = 0;

		return 1;
	}

	//iterating until last element is found
	NodeT* pWalker = list->head;
	while (pWalker->next != list->tail && pWalker->next != NULL)
	{
		pWalker = pWalker->next;
	}

	free(list->tail->data);
	free(list->tail);
	pWalker->next = NULL;
	list->tail = pWalker;
	list->length--;

	return 1;

}

int remove_node_from_beginning(GenericLinkedListT* list)
{
	if (list->head == NULL)
	{
		return 0;
	}

	NodeT* head = list->head;
	list->head = list->head->next;

	free(head->data);
	free(head);
	list->length--;

	return 1;
}


int remove_node_at_index(GenericLinkedListT* list, int index)
{
	if (index < 0 || index >= list->length)
		return 0;

	if (index == 0)
	{
		return remove_node_from_beginning(list);
	}

	if (index == list->length - 1)
	{
		return remove_node_from_end(list);
	}

	if (list->head == NULL)
		return 0;

	//value to be deleted will be stored in pWalker->next, so i should start from 1
	int i = 1;
	NodeT* pWalker = list->head;

	//while condition used only as a failsafe
	while (pWalker->next != NULL)
	{
		if (i == index)
		{
			NodeT* to_delete = pWalker->next;
			pWalker->next = pWalker->next->next;
			free(to_delete->data);
			free(to_delete);
			list->length--;

			return 1;
		}
		pWalker = pWalker->next;
		i++;
	}

	return 0;
}
