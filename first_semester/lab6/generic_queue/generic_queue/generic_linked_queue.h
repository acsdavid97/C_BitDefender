/*
author: Acs David
file name: generic_queue.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_QUEUE_H
#define GENERIC_QUEUE_H

#include "generic_linked_list.h"

// structs
typedef struct GenericLinkedQueueTag {
	GenericLinkedListT* linked_list;
}GenericLinkedQueueT;

// define your functions here
GenericLinkedQueueT* create_GenericLinkedQueueT();
int add_to_linked_queue_at_index(GenericLinkedQueueT* queue, NodeT* node_to_insert, int index);
int remove_from_linked_queue_at_index(GenericLinkedQueueT* queue, int index);
NodeT* get_node_from_linked_queue_at_index(GenericLinkedQueueT* queue, int index);

#endif
