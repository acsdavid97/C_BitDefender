#define _CRT_SECURE_NO_WARNINGS

/*
author: David
file name: generic_linked_list.cpp
file creation date: 2016-12-14 15:10:43
*/

#include "generic_linked_list.h"

NodeT* create_NodeT(void* data)
{
	NodeT* node = (NodeT*)malloc(sizeof(NodeT));
	if (node == NULL)
	{
		// Memory allocation failed.
		return NULL;
	}
	node->data = data;
	node->next = NULL;

	return node;
}

GenericLinkedListT* create_empty_GenericLinkedListT()
{
	GenericLinkedListT* empty_list = (GenericLinkedListT*)malloc(sizeof(GenericLinkedListT));
	if (empty_list == NULL)
	{
		// Memory allocation failed.
		return NULL;
	}

	empty_list->head = empty_list->tail = NULL;
	empty_list->length = 0;

	return empty_list;
}

int get_list_length(GenericLinkedListT* list)
{
	return list->length;
}

ReturnCodeE add_node_at_end(GenericLinkedListT* list, NodeT* node_to_insert)
{
	//first element to insert
	if (list->head == NULL)
	{
		list->head = list->tail = node_to_insert;
		list->head->next = NULL;
		list->length = 1;
		return SUCCESS;
	}

	list->tail->next = node_to_insert;
	list->tail = node_to_insert;
	list->length++;

	return SUCCESS;
}

ReturnCodeE add_node_at_beginning(GenericLinkedListT* list, NodeT* node_to_insert)
{
	if (list->head == NULL)
	{
		list->head = list->tail = node_to_insert;
		list->head->next = NULL;
		list->length = 1;

		return SUCCESS;
	}

	node_to_insert->next = list->head;
	list->head = node_to_insert;
	list->length++;

	return SUCCESS;
}

ReturnCodeE add_node_at_index(GenericLinkedListT* list, NodeT* node_to_insert, int index)
{
	if (index < 0 || index > list->length)
		return INCORRECT_INDEX;

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

			return SUCCESS;
		}

		pWalker = pWalker->next;
		i++;
	}

	return INCORRECT_INDEX;
}

void print_linked_list(GenericLinkedListT* list, FILE* file, void (*print_data)(const void* data, FILE* file))
{
	NodeT* pWalker = list->head;
	while (pWalker != NULL)
	{
		print_data(pWalker->data, file);
		pWalker = pWalker->next;
	}
	fprintf(file, "\n");
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

ReturnCodeE remove_node_from_end(GenericLinkedListT* list)
{
	if (list->head == NULL)
		return INCORRECT_INDEX;

	//if head is the only node
	if (list->head->next == NULL)
	{
		free(list->head);
		list->head = list->tail = NULL;
		list->length = 0;

		return SUCCESS;
	}

	//iterating until last element is found
	NodeT* pWalker = list->head;
	while (pWalker->next != list->tail && pWalker->next != NULL)
	{
		pWalker = pWalker->next;
	}

	free(list->tail);
	pWalker->next = NULL;
	list->tail = pWalker;
	list->length--;

	return SUCCESS;

}

ReturnCodeE remove_node_from_beginning(GenericLinkedListT* list)
{
	if (list->head == NULL)
	{
		return INCORRECT_INDEX;
	}

	NodeT* head = list->head;
	list->head = list->head->next;

	free(head);
	list->length--;

	return SUCCESS;
}


ReturnCodeE remove_node_at_index(GenericLinkedListT* list, int index)
{
	if (index < 0 || index >= list->length)
		return INCORRECT_INDEX;

	if (index == 0)
	{
		return remove_node_from_beginning(list);
	}

	if (index == list->length - 1)
	{
		return remove_node_from_end(list);
	}

	if (list->head == NULL)
		return INCORRECT_INDEX;

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
			free(to_delete);
			list->length--;

			return SUCCESS;
		}
		pWalker = pWalker->next;
		i++;
	}

	return INCORRECT_INDEX;
}

void* search_element_in_list(GenericLinkedListT* list, void* element, int(*compare)(const void* a, const void *b))
{
	NodeT* pWalker = list->head;
	while (pWalker != NULL)
	{
		if (compare(pWalker->data, element) == 0)
		{
			return pWalker->data;
		}
		pWalker = pWalker->next;
	}

	return NULL;
}

void* search_and_delete_element_in_list(GenericLinkedListT* list, void* element, int(*compare)(const void* a, const void *b))
{
	if (list->head == NULL)
	{
		return NULL;
	}

	if (compare(list->head->data, element) == 0)
	{
		void* element = list->head->data;
		remove_node_from_beginning(list);

		return element;
	}

	NodeT* pWalker = list->head;
	while (pWalker->next != NULL)
	{
		if (compare(pWalker->next->data, element) == 0)
		{
			void* element = pWalker->next->data;
			pWalker->next = pWalker->next->next;
			free(pWalker->next);
			list->length--;

			return element;
		}
		pWalker = pWalker->next;
	}

	return NULL;
}

void sort_elements_in_list(GenericLinkedListT* list, int(*compare)(const void* a, const void *b))
{
	// Bubble sort, since we do not have random access.
	for (NodeT* pWalker1 = list->head; pWalker1->next != NULL; pWalker1 = pWalker1->next)
	{
		for (NodeT* pWalker2 = pWalker1->next; pWalker2 != NULL; pWalker2 = pWalker2->next)
		{
			if (compare(pWalker1->data, pWalker2->data) > 0)
			{
				void* temp = pWalker1->data;
				pWalker1->data = pWalker2->data;
				pWalker2->data = temp;
			}
		}
	}
}

void merge_linked_lists(GenericLinkedListT* list_destination, GenericLinkedListT* list_source)
{
	if (list_destination->head == NULL)
	{
		//if the destination list is empty, copy everything from the source list.
		list_destination->head = list_source->head;
		list_destination->tail = list_source->tail;
		list_destination->length = list_source->length;

		list_source->head = list_source->tail = NULL;
		list_source->length = 0;

		return;
	}
	list_destination->tail->next = list_source->head;
	list_destination->tail = list_source->tail;
	list_destination->length += list_source->length;

	list_source->head = list_source->tail = NULL;
	list_source->length = 0;
}

void delete_linked_list(GenericLinkedListT* list, void(*free_generic_data)(void* generic_data))
{
	NodeT* pWalker = list->head;
	while (pWalker != NULL)
	{
		NodeT* next = pWalker->next;
		free_generic_data(pWalker->data);
		free(pWalker->data);
		free(pWalker);
		pWalker = next;
	}

	list->head = list->tail = NULL;
	list->length = 0;
}
