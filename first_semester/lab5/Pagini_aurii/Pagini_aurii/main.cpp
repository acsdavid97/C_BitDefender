#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-12-13 09:29:57
*/

#include "pagini_aurii.h"

int main()
{
	NodeT* root = NULL;
	while (true)
	{
		print_menu();
		char input = getchar();
		char c = '0';
		while (c = getchar() != '\n' && c != EOF); // discard unwanted characters
		switch (input)
		{
		case '1':
		{
			NodeT* node = read_entry();
			root = insert(node, root);
			break;
		}
		case '2':
		{
			char *to_delete = print_read("name to delete");
			delete_node_wraper(root, to_delete);
			free(to_delete);
			break;
		}	
		case '3':
		{
			char *to_find = print_read("search by name and surname(do not separate by space)");
			NodeT* result = find(to_find, root);
			if (result != NULL)
				print_EntryT(result->entry);
			else
				printf("not found\n");
			break;
		}
		case '4':
		{
			char *tel_to_find = print_read("telephone number to find");
			NodeT* result = search_by_telephone_number(root, tel_to_find);
			if (result != NULL)
				print_EntryT(result->entry);
			else
				printf("not found\n");
			break;
		}
		case '5':
		{
			char *surname = print_read("surname");
			print_all_with_same_surname(root, surname);
			break;
		}
		case '6':
			printtree(root);
			break;
		case '0':
			exit(0);
			break;
		default:
			printf("not a valid input\n");
			break;
		}
	}
	
	system("pause");
	return 0;
}