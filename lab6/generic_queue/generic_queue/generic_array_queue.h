/*
author: Acs David
file name: generic_array_queue
file creation date: 2016-11-20 23:26:24
*/

#ifndef GENERIC_ARRAY_QUEUE_H
#define GENERIC_ARRAY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "generic_array.h"

// structs
typedef struct GenericArrayQueueTag {
	GenericArrayT* array_list;
}GenericArrayQueueT;

// define your functions here
GenericArrayQueueT* create_GenericArrayQueueT();
int add_to_array_queue_at_index(GenericArrayQueueT* queue, GenericDataT* element_to_insert, int index);
int remove_from_array_queue_at_index(GenericArrayQueueT* queue, int index);
GenericDataT* get_node_from_array_queue_at_index(GenericArrayQueueT* queue, int index);

#endif
