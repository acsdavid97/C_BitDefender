#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: generic_hash_table.cpp
file creation date: 2016-12-18 19:48:52
*/

#include "generic_heap.h"

GenericHeapT* create_empty_GenericHeapT(int(*compare)(const void* a, const void* b))
{
	if (compare == NULL)
	{
		// heap is useless without a compare function.
		return NULL;
	}

	GenericHeapT* empty_heap = (GenericHeapT*)malloc(sizeof(GenericHeapT));
	if (empty_heap == NULL)
	{
		return NULL;
	}

	empty_heap->compare = compare;

	empty_heap->array = create_empty_GenericArrayT();
	if (empty_heap->array == NULL)
	{
		free(empty_heap);
		return NULL;
	}

	return empty_heap;
}

int get_nr_of_elements_in_heap(GenericHeapT* heap)
{
	return heap->array->length;
}

int get_parent_index(int index)
{
	return (index - 1) / 2;
}

int get_left_child_index(int index)
{
	return 2 * index + 1;
}

int get_right_child_index(int index)
{
	return 2 * index + 2;
}

HeapElementT* get_min_child(GenericHeapT* heap, int index, int(*compare)(const void* a, const void* b))
{
	GenericArrayT* array = heap->array;
	int left_child_index = get_left_child_index(index);
	void* left_child = get_element_at_index(array, left_child_index);
	int right_child_index = get_right_child_index(index);
	void* right_child = get_element_at_index(array, right_child_index);

	//no childs exists
	if (left_child == NULL && right_child == NULL)
	{
		return NULL;
	}

	HeapElementT* min_child = (HeapElementT*)malloc(sizeof(HeapElementT));

	//right child does not exist
	if (right_child == NULL)
	{
		min_child->element = left_child;
		min_child->index = left_child_index;
		return min_child;
	}

	//left child does not exist
	if (left_child == NULL)
	{
		min_child->element = right_child;
		min_child->index = right_child_index;
		return min_child;
	}

	//both childs exist, return the smaller one
	if (compare(left_child, right_child) < 0)
	{
		min_child->element = left_child;
		min_child->index = left_child_index;
	}
	else
	{
		min_child->element = right_child;
		min_child->index = right_child_index;
	}
	return min_child;
}

void sink_element_in_heap(GenericHeapT* heap, void* element, int current_index)
{
	HeapElementT* min_child = get_min_child(heap, current_index, heap->compare);

	while (min_child != NULL && heap->compare(element, min_child->element) > 0)
	{
		exchange_elements_at_indexes(heap->array, current_index, min_child->index);
		current_index = min_child->index;
		min_child = get_min_child(heap, current_index, heap->compare);
	}
}

void float_element_in_heap(GenericHeapT* heap, void* element, int current_index)
{
	GenericArrayT* array = heap->array;

	int parent_index = get_parent_index(current_index);
	void* parent_element = get_element_at_index(array, parent_index);
	while (parent_element != NULL && heap->compare(element, parent_element) < 0)
	{
		exchange_elements_at_indexes(array, parent_index, current_index);
		current_index = parent_index;
		parent_index = get_parent_index(current_index);
		parent_element = get_element_at_index(array, parent_index);
	}
}

ReturnCodeE add_element_in_heap(GenericHeapT* heap, void* element)
{
	GenericArrayT* array = heap->array;
	ReturnCodeE return_code = add_element_at_index(array, element, array->length);
	if (return_code != SUCCESS)
	{
		return return_code;
	}

	//float element
	float_element_in_heap(heap, element, array->length - 1);

	return SUCCESS;
}

void* get_heap_top(GenericHeapT* heap)
{
	return get_element_at_index(heap->array, 0);
}

void* delete_heap_top(GenericHeapT* heap)
{
	GenericArrayT* array = heap->array;
	void* top_element = get_element_at_index(array, 0);
	if (top_element == NULL)
	{
		//array is empty
		return NULL;
	}

	void* last_element = get_element_at_index(array, array->length - 1);
	set_element_at_index(array, last_element, 0);
	set_element_at_index(array, NULL, array->length - 1);
	array->length--;

	//sink element
	sink_element_in_heap(heap, last_element, 0);

	return top_element;
}

HeapElementT* search_element_recursively_in_heap(GenericHeapT* heap, void* element, int current_index)
{
	void* current_element = get_element_at_index(heap->array, current_index);
	if (current_element == NULL)
	{
		return NULL;
	}

	if (heap->compare(element, current_element) == 0)
	{
		HeapElementT* element_found = (HeapElementT*)malloc(sizeof(HeapElementT));
		element_found->element = current_element;
		element_found->index = current_index;
		return element_found;
	}

	if (heap->compare(element, current_element) > 0)
	{
		int left_index = get_left_child_index(current_index);
		HeapElementT* left_result = search_element_recursively_in_heap(heap, element, left_index);
		
		if (left_result == NULL)
		{
			int right_index = get_right_child_index(current_index);
			return search_element_recursively_in_heap(heap, element, right_index);
		}

		return left_result;
	}

	return NULL;
}

HeapElementT* search_element_in_heap(GenericHeapT* heap, void* element)
{
	return search_element_recursively_in_heap(heap, element, 0);
}

void* delete_element_from_heap(GenericHeapT* heap, void* element)
{
	HeapElementT* element_found = search_element_in_heap(heap, element);
	if (element_found == NULL)
	{
		return NULL;
	}

	GenericArrayT* array = heap->array;

	void* last_element = get_element_at_index(array, array->length - 1);
	set_element_at_index(array, last_element, element_found->index);
	set_element_at_index(array, NULL, array->length - 1);
	array->length--;

	sink_element_in_heap(heap, last_element, element_found->index);
	float_element_in_heap(heap, last_element, element_found->index);

	return element_found->element;
}

void print_elements_recusively_inorder(GenericHeapT* heap, int current_index, 
	int current_level, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	void* element = get_element_at_index(heap->array, current_index);
	if (element == NULL)
	{
		return;
	}
	print_elements_recusively_inorder(heap, get_left_child_index(current_index),
		current_level + 1, file, print_element);
	for (int i = 0; i < current_level; i++)
	{
		fprintf(file, "  ");
	}
	print_element(element, file);
	fprintf(file, "\n");
	print_elements_recusively_inorder(heap, get_right_child_index(current_index),
		current_level + 1, file, print_element);

}

void print_elements_in_heap(GenericHeapT* heap, FILE* file, void(*print_element)(const void *a, FILE* file))
{
	print_elements_recusively_inorder(heap, 0, 0, file, print_element);
}

ReturnCodeE merge_heaps(GenericHeapT* heap_destination, GenericHeapT* heap_source)
{
	if (heap_destination->compare != heap_source->compare)
	{
		return COMPARE_FUNCTION_MISMATCH;
	}

	for (int i = 0; i < heap_source->array->length; i++)
	{
		void* element = *(heap_source->array->data + i);
		ReturnCodeE return_code = add_element_in_heap(heap_destination, element);
		if (return_code != SUCCESS)
		{
			return return_code;
		}
	}

	free(heap_source->array);
	return SUCCESS;
}

void delete_elements_in_heap(GenericHeapT* heap, void(*free_generic_data)(void* generic_data))
{
	delete_elements_in_array(heap->array, free_GenericDataT);
	free(heap->array);
	heap->array = NULL;
	heap->compare = NULL;
}
