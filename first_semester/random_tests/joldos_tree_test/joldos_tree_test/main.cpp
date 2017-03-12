#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main
file creation date: 2017-01-06 15:43:20
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node_type
{
	char id; /* node name */
			 /* ... other useful info */
	struct node_type *left, *right;
} NodeT;

void fatalError(const char *msg)
{
	printf(msg);
	printf("\n");
	exit(1);
}

void preorder(NodeT *p, int level)
{
	if (p != NULL)
	{
		for (int i = 0; i <= level; i++) printf(" "); /* for nice listing */
		printf("%2.2d\n", p->id);
		preorder(p->left, level + 1);
		preorder(p->right, level + 1);
	}
}

void inorder(NodeT *p, int level)
{
	if (p != NULL)
	{
		inorder(p->left, level + 1);
		for (int i = 0; i <= level; i++) printf(" "); /* for nice listing */
		printf("%2.2d\n", p->id);
		inorder(p->right, level + 1);
	}
}

void postorder(NodeT *p, int level)
{
	if (p != NULL)
	{
		postorder(p->left, level + 1);
		postorder(p->right, level + 1);
		for (int i = 0; i <= level; i++) printf(" "); /* for nice listing */
		printf("%2.2d\n", p->id);
	}
}

void interchangeodes(NodeT* p)
{
	if (p == NULL)
	{
		printf("cant interchange NULL pointer");
		return;
	}

	NodeT* aux = p->right;
	p->right = p->left;
	p->left = aux;
}

NodeT *createBinTree(int branch, NodeT *parent)
{
	NodeT *p;
	int id;
	/* read node id */
	if (branch == 0)
		printf("Root identifier [0 to end] =");
	else if (branch == 1)
		printf("Left child of %d [0 to end] =",
			parent->id);
	else
		printf("Right child of %d [0 to end] =",
			parent->id);
	scanf("%d", &id);
	if (id == 0)
		return NULL;
	else
	{
		/* build node pointed to by p */
		p = (NodeT *)malloc(sizeof(NodeT));
		if (p == NULL)
			fatalError("Out of space in createBinTree");
		/* fill in node */
		p->id = id;
		p->left = createBinTree(1, p);
		p->right = createBinTree(2, p);
	}
	return p;
}

NodeT* find(NodeT* p, int value)
{
	if (p != NULL)
	{
		if (p->id == value)
			return p;
		return find(p->left, value);
		return find(p->right, value);
	}
	return NULL;
}

void show(NodeT *root, int level)
{
	while ('\n' != getc(stdin));
	printf("\nPreorder listing\n");
	preorder(root, 0);
	printf("Press Enter to continue.");
	while ('\n' != getc(stdin));
	printf("\nInorder listing\n");
	inorder(root, 0);
	printf("Press Enter to continue.");
	while ('\n' != getc(stdin));
	printf("\nPostorder listing\n");
	postorder(root, 0);
	printf("Press Enter to continue.");
	while ('\n' != getc(stdin));
}

int detheight(NodeT* nod)
{
	if (nod == NULL)
		return 0;
	int l = detheight(nod->left);
	int r = detheight(nod->right);
	if (l>r)
		return l + 1;
	return r + 1;
}

int nmb_of_leaves(NodeT* nod)
{
	int s = 0;
	if (nod->left == NULL&&nod->right == NULL)
		return 1;
	if (nod->left != NULL)
		s += nmb_of_leaves(nod->left);
	if (nod->right != NULL)
		s += nmb_of_leaves(nod->right);
	return s;
}

int main()
{
	NodeT *root;
	root = createBinTree(0, NULL);
	show(root, 0);
	printf("identifier of node to interchange:");
	int i = 0;
	scanf("%d", &i);
	interchangeodes(find(root, i));
	show(root, 0);
	printf("height:%d\n", detheight(root));
	printf("nr of leaves:%d\n", nmb_of_leaves(root));
	return 0;
}
