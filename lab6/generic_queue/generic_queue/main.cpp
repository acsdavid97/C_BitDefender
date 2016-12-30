#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2016-12-20 11:04:27
*/

//generic linked list implemented
#include "generic_linked_queue.h"
//generic array implemented
#include "generic_array_queue.h"

//NOTE: queue is implemented only as a wrapper for the linkedlist and array ADTs.
int main()
{
	//simple tests
	GenericLinkedQueueT* queue = create_GenericLinkedQueueT();
	add_to_linked_queue_at_index(queue, create_NodeT(create_GenericDataT("asdf", "asdf2", "asdf3", "asdf5", "asdf6", "email")), 0);
	print_GenericDataT(get_node_from_linked_queue_at_index(queue, 0)->data);
	remove_from_linked_queue_at_index(queue, 0);

	GenericArrayQueueT* queue2 = create_GenericArrayQueueT();
	add_to_array_queue_at_index(queue2, create_GenericDataT("fdas", "asdf", "fdas2" "asdfgdsa", "453q", "33332", "acsa@asdfas"), 0);
	print_GenericDataT(get_node_from_array_queue_at_index(queue2, 0));
	remove_from_array_queue_at_index(queue2, 0);

	system("pause");
	return 0;
}