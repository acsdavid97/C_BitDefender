#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: generic_linked_queue
file creation date: 2016-12-20 14:19:10
*/

#include "generic_linked_queue.h"

GenericLinkedQueueT* create_GenericLinkedQueueT()
{
	GenericLinkedQueueT* queue = (GenericLinkedQueueT*)malloc(sizeof(GenericLinkedQueueT));
	queue->linked_list = create_empty_GenericLinkedListT();

	return queue;
}

int add_to_linked_queue_at_index(GenericLinkedQueueT* queue, NodeT* node_to_insert, int index)
{
	return add_node_at_index(queue->linked_list, node_to_insert, index);
}

int remove_from_linked_queue_at_index(GenericLinkedQueueT* queue, int index)
{
	return remove_node_at_index(queue->linked_list, index);
}

NodeT* get_node_from_linked_queue_at_index(GenericLinkedQueueT* queue, int index)
{
	return get_node_at_index(queue->linked_list, index);
}
