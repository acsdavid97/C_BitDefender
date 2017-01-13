#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: trie_module.cpp
file creation date: 2017-01-11 14:52:20
*/

#include "trie_module.h"

int get_nr_of_words(TrieNodeT* subtrie_root)
{
	int sum = 0;
	if (subtrie_root == NULL)
	{
		return 0;
	}
	if (subtrie_root->word_end == true)
	{
		sum += 1;
	}

	sum += get_nr_of_words(subtrie_root->child) + get_nr_of_words(subtrie_root->brother);
	return sum;
}


void add_word_in_trie(TrieNodeT* subtrie_root, char* word_to_add)
{
	if (subtrie_root->child == NULL || subtrie_root->child->element > *word_to_add)
	{
		TrieNodeT* old_child = subtrie_root->child;
		subtrie_root->child = create_TrieNodeT(*word_to_add);
		subtrie_root->child->brother = old_child;
	}

	subtrie_root = subtrie_root->child;
	while (*word_to_add != '\0')
	{
		if (subtrie_root->element == *word_to_add)
		{
			word_to_add++;
			if (subtrie_root->child == NULL)
			{
				subtrie_root->child = create_TrieNodeT(*word_to_add);
			}
			subtrie_root = subtrie_root->child;
			continue;
		}
		while (subtrie_root->brother != NULL && subtrie_root->brother->element < *word_to_add)
		{
			subtrie_root = subtrie_root->brother;
		}

		TrieNodeT* old_brother = subtrie_root->brother;
		subtrie_root->brother = create_TrieNodeT(*word_to_add);
		subtrie_root = subtrie_root->brother;
		subtrie_root->brother = old_brother;
	}
	subtrie_root->word_end = true;
}

int search_word_in_trie(TrieNodeT* subtrie_root, char* word_to_search)
{
	subtrie_root = subtrie_root->child;
	int i = 0;
	while (i <= strlen(word_to_search))
	{
		if (word_to_search[i] == '\0' && subtrie_root->word_end == true)
		{
			//found it
			return 1;
		}
		while (subtrie_root != NULL && subtrie_root->element < word_to_search[i])
		{
			subtrie_root = subtrie_root->brother;
		}
		if (subtrie_root != NULL && subtrie_root->element == word_to_search[i])
		{
			subtrie_root = subtrie_root->child;
			i++;
			continue;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

TrieNodeT* create_TrieNodeT(char element)
{
	TrieNodeT* trieNodeT = (TrieNodeT*)malloc(sizeof(TrieNodeT));
	trieNodeT->brother = trieNodeT->child = NULL;
	trieNodeT->element = element;
	trieNodeT->word_end = false;

	return trieNodeT;
}
