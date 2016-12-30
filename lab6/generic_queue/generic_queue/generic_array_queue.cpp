#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: generic_array_queue
file creation date: 2016-12-20 20:43:14
*/

#include "generic_array_queue.h"

GenericArrayQueueT* create_GenericArrayQueueT()
{
	GenericArrayQueueT* queue = (GenericArrayQueueT*)malloc(sizeof(GenericArrayQueueT));
	queue->array_list = create_empty_GenericArrayT();

	return queue;
}
int add_to_array_queue_at_index(GenericArrayQueueT* queue, GenericDataT* element_to_insert, int index)
{
	return add_element_at_index(queue->array_list, element_to_insert, index);
}

int remove_from_array_queue_at_index(GenericArrayQueueT* queue, int index)
{
	return delete_element_at_index(queue->array_list, index);
}

GenericDataT* get_node_from_array_queue_at_index(GenericArrayQueueT* queue, int index)
{
	return get_element_at_index(queue->array_list, index);
}
