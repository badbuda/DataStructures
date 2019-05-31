/*******************************************************************************
* NAME: binary sorted tree Functions file                                      *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  13.12.18                                                      *
*******************************************************************************/

#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, free */
#include <assert.h>	/* assert */

#include "binary_sorted_tree.h"

#define EQUAL 0
/******************************************************************************/

typedef enum {RIGHT, LEFT} direction;

typedef struct bst_node_s bst_node_t;

struct bst_node_s
{
	void *data;
	bst_node_t *parent;
	bst_node_t *right;
	bst_node_t *left;
};

struct bst_s
{
	bst_node_t *root;
	compare_function_t compare;
};

bst_iterator_t BST_ITERATOR_NOT_FOUND = { NULL };
/******************************************************************************/

static bst_node_t *BstNodeCreate(void *data, bst_node_t *parent)
{
	bst_node_t *bst_node = malloc(sizeof (bst_node_t));

	if (NULL == bst_node)
	{
		return (NULL);
	}

	bst_node->data = data;
	bst_node->parent = parent;
	bst_node->right = NULL;
	bst_node->left = NULL;

	return(bst_node);
}
/******************************************************************************/

bst_t *BSTCreate(compare_function_t compare)
{
	bst_node_t *root = NULL;
	bst_t *bst = malloc(sizeof (bst_t));

	if (NULL == bst)
	{
		return (NULL);
	}

	root = BstNodeCreate(NULL, NULL);

	if (NULL == root)
	{
		return (NULL);
	}

	bst->root = root;
	bst->compare = compare;

	return (bst);
}
/******************************************************************************/

static void RemoveAndFreeNode(bst_node_t *node)
{
		if (node == (node->parent->right))
		{
			node->parent->right = NULL;
		}
		else
		{
			node->parent->left = NULL;
		}

		free(node);
}

void BSTDestroy(bst_t *bst)
{
	bst_node_t *action_node = NULL;
	bst_node_t *parent = NULL;
	bst_node_t *root = NULL;

	assert (bst);

	root = bst->root;
	parent = (bst_node_t*)BSTBegin(bst).info;

	while (root != parent)
	{
		action_node = parent;

		while (NULL != action_node->left || NULL != action_node->right)
		{
			while (NULL != action_node->left)
			{
				action_node = action_node->left;
			}

			while (NULL != action_node->right)
			{
				action_node = action_node->right;
			}
		}

		parent = action_node->parent;

		RemoveAndFreeNode(action_node);
	}

	free(root);
	free(bst);
}
/******************************************************************************/

size_t BSTGetSize(bst_t *bst)
{
	size_t counter = 0;
	bst_iterator_t iterator = BSTBegin(bst);

	while (iterator.info != bst->root)
	{
		++counter;
		iterator = BSTGetNext(iterator);
	}

	return (counter);
}
/******************************************************************************/

bool BSTIsEmpty(bst_t *bst)
{
	return (NULL == bst->root->left);
}
/******************************************************************************/

bst_iterator_t BSTBegin(bst_t *bst)
{
	bst_iterator_t iterator;
	bst_node_t* target = NULL;
	assert(bst);

	target = bst->root;

	while (target->left)
	{
		target = target->left;
	}

	iterator.info = target;

	return (iterator);
}
/******************************************************************************/

bst_iterator_t BSTEnd(bst_t *bst)
{
	bst_iterator_t iterator;
	assert(bst);

	iterator.info = bst->root;

	return (iterator);
}
/******************************************************************************/

bst_iterator_t BSTInsert(bst_t *bst, void *data)
{
	bst_iterator_t iterator;
	bst_node_t* target = NULL;
	bst_node_t* next_target = NULL;
	direction left_or_right = LEFT;

	assert(bst);

	target = bst->root;
	next_target = bst->root->left;

	while (next_target)
	{
		target = next_target;
		assert(EQUAL != bst->compare(data, target->data));

		if (0 < bst->compare(data, target->data))
		{
			next_target = target->right;
			left_or_right = RIGHT;
		}
		else
		{
			next_target = target->left;
			left_or_right = LEFT;
		}
	}

	next_target = BstNodeCreate(data, target);

	if (NULL == next_target)
	{
		return (BST_ITERATOR_NOT_FOUND);
	}

	if (RIGHT == left_or_right)
	{
		target->right = next_target;
	}
	else
	{
		target->left = next_target;
	}

	iterator.info = next_target;

	return (iterator);
}
/******************************************************************************/

void BSTRemove(bst_iterator_t iter)
{
	bst_node_t *target = NULL;
	bst_node_t *new_son_of_parent = NULL;
	direction left_or_right = LEFT;

	assert(iter.info);

	target = (bst_node_t *)iter.info;
	new_son_of_parent = target->left;

	if (target == target->parent->right)
	{
		left_or_right = RIGHT;
	}

	if (target->right)
	{
		bst_node_t *most_lefty = NULL;
		new_son_of_parent = target->right;

		if (target->left)
		{
			most_lefty = new_son_of_parent;

			while (most_lefty->left)
			{
				most_lefty = most_lefty->left;
			}

			most_lefty->left = target->left;
			most_lefty->left->parent = most_lefty;
		}
	}

	if (new_son_of_parent)
	{
		new_son_of_parent->parent = target->parent;
	}

	if (LEFT ==	left_or_right)
	{
		target->parent->left = new_son_of_parent;
	}
	else
	{
		target->parent->right = new_son_of_parent;
	}

	free(target);
}
/******************************************************************************/

bst_iterator_t BSTFind(bst_t *bst, void *data)
{
	bst_node_t *find_node = NULL;
	bst_node_t *root = NULL;
	bst_iterator_t iterator;

	assert (bst);
	assert (data);

	root = bst->root;
	find_node = (bst_node_t*)BSTBegin(bst).info;

	while (root != find_node)
	{
		if (EQUAL == (bst->compare(find_node->data, data)))
		{
			iterator.info = find_node;
			return (iterator);
		}

		iterator.info = find_node;
		find_node = BSTGetNext(iterator).info;
	}

	return (BST_ITERATOR_NOT_FOUND);
}
/******************************************************************************/

status BSTForEach
	(bst_t *bst, action_function_t action, void *param)
{
	bst_node_t *action_node = NULL;
	bst_node_t *root = NULL;
	bst_iterator_t iterator;
	status stat = SUCCESS;

	assert (bst);

	root = bst->root;
	action_node = (bst_node_t*)BSTBegin(bst).info;

	while (root != action_node)
	{
		if (ACTION_FAIL == (stat = action(action_node->data, param)))
		{
			return (ACTION_FAIL);
		}

		iterator.info = action_node;
		action_node = BSTGetNext(iterator).info;
	}

	return (SUCCESS);
}
/******************************************************************************/

bst_iterator_t BSTGetNext(bst_iterator_t iter)
{
	bst_node_t *target = NULL;
	assert(iter.info);

	target = (bst_node_t *)iter.info;

	if (target->right)
	{
		target = target->right;

		while (target->left)
		{
			target = target->left;
		}
	}
	else if ((NULL != target->parent) && (target->parent->left == target))
	{
		target = target->parent;
	}
	else
	{
		while ((NULL != target->parent) && (target->parent->right == target))
		{
			target = target->parent;
		}

		target = target->parent;
	}

	iter.info = target;

	return (iter);
}
/******************************************************************************/

bst_iterator_t BSTGetPrevious(bst_iterator_t iter)
{
	bst_node_t *target = NULL;
	assert(iter.info);

	target = (bst_node_t *)iter.info;

	if (target->left)
	{
		target = target->left;

		while (target->right)
		{
			target = target->right;
		}
	}
	else if (target->parent->right == target)
	{
		target = target->parent;
	}
	else
	{
		while ((NULL != target->parent) && (target->parent->left == target))
		{
			target = target->parent;
		}

		target = target->parent;
	}

	iter.info = target;
	return (iter);
}
/******************************************************************************/

void *BSTGetData(bst_iterator_t iter)
{
	assert(iter.info);
	return (((bst_node_t *)iter.info)->data);
}
/******************************************************************************/

bool BSTIsSameIterator(
	const bst_iterator_t iter1,
	const bst_iterator_t iter2
)
{
	return (iter1.info == iter2.info);
}
/*******************************************************************************
*******************************************************************************/
