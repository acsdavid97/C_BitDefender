#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2017-01-11 14:45:38
*/

#include "trie_module.h"

int main()
{
	TrieNodeT* root = create_TrieNodeT('\0');

	FILE* input_file = fopen("strings.txt", "r");

	char buffer[1024];
	while (fgets(buffer, 1024, input_file))
	{
		add_word_in_trie(root, buffer);
	}
	printf("nr of words:%d\n", get_nr_of_words(root));
	printf("is 'hehe' in trie: %d\n", search_word_in_trie(root, "hehe\n"));
	printf("is 'forth.' in trie: %d\n", search_word_in_trie(root, "forth.\n"));
	printf("is 'and' in trie: %d\n", search_word_in_trie(root, "and\n"));

	fclose(input_file);
	system("pause");
	return 0;
}