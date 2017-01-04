#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2016-12-14 15:10:47
*/

#include "../Library/generic_linked_list.h"
#include "generic_data.h"

int main()
{
	GenericLinkedListT* list = create_empty_GenericLinkedListT();
	GenericDataT* data = create_GenericDataT(23);
	NodeT* node = create_NodeT(data);
	add_node_at_end(list, node);
	data = create_GenericDataT(54);
	node = create_NodeT(data);
	add_node_at_end(list, node);

	data = create_GenericDataT(1234);
	node = create_NodeT(data);
	add_node_at_end(list, node);

	remove_node_at_index(list, 2);

	print_GenericDataT(get_node_at_index(list, 1)->data, stdout);
	remove_node_from_end(list);

	system("pause");
	return 0;
}