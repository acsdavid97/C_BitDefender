#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2016-12-14 15:10:47
*/

#include "generic_linked_list.h"

int main()
{
	GenericLinkedListT* list = create_empty_GenericLinkedListT();
	GenericDataT* data = create_GenericDataT("asdf", "name", "adddress", "city", "telephone", "email");
	NodeT* node = create_NodeT(data);
	add_node_at_end(list, node);
	data = create_GenericDataT("asdf2", "name", "adddress", "city", "telephone", "email");
	node = create_NodeT(data);
	add_node_at_end(list, node);

	data = create_GenericDataT("asdf3", "name", "adddress", "city", "telephone", "email");
	node = create_NodeT(data);
	add_node_at_end(list, node);

	remove_node_at_index(list, 2);

	print_GenericDataT(get_node_at_index(list, 1)->data);
	remove_node_from_end(list);
	print_linked_list(list, print_GenericDataT);

	system("pause");
	return 0;
}