/*******************************************************************************
* NAME: Avl tree                                                               *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  25.12.18                                                      *
*******************************************************************************/
#include <stdlib.h> /* size_t, malloc, free */
#include <assert.h> /* assert */

#include "avl_tree.h"

#define EQUAL 0
#define EMPTY 0
#define NO_HEAVINESS 0
#define MAX(x, y) (x > y ? x : y)
/******************************************************************************/

typedef struct avl_node_s avl_node_t;

struct avl_node_s
{
	void *data;
	avl_node_t *left;
	avl_node_t *right;
	size_t height;
};

struct avl_tree_s
{
	avl_node_t *root;
	compare_function_t compare;
};
/******************************************************************************/

static avl_node_t *NodeCreate(void *data)
{
	avl_node_t *node = malloc(sizeof (avl_node_t));

	if (NULL == node)
	{
		return (NULL);
	}

	node->data = data;
	node->right = NULL;
	node->left = NULL;
	node->height = 1;

	return (node);
}

static void UpdateHeight(avl_node_t *node)
{
	if (node->right)
	{
		if (node->left)
		{
			node->height = 1 + MAX(node->right->height, node->left->height);
		}
		else
		{
			node->height = 1 + node->right->height;
		}
	}
	else if (node->left)
	{
		node->height = 1 + node->left->height;
	}
	else
	{
		node->height = 1;
	}
}

static avl_node_t *RotateTreeRight(avl_node_t *root)
{
	avl_node_t *pivot = root->left;
	root->left = pivot->right;
	pivot->right = root;

	UpdateHeight(root);
	UpdateHeight(pivot);

	return (pivot);
}

static avl_node_t *RotateTreeLeft(avl_node_t *root)
{
	avl_node_t *pivot = root->right;
	root->right = pivot->left;
	pivot->left = root;

	UpdateHeight(root);
	UpdateHeight(pivot);

	return (pivot);
}

static int NodeHeaviness(avl_node_t *node)
{
	if (node->right)
	{
		if (node->left)
		{
			return (node->right->height - node->left->height);
		}
		else
		{
			return (node->right->height);
		}
	}
	else if (node->left)
	{
		return (-(node->left->height));
	}
	else
	{
		return (NO_HEAVINESS);
	}
}

static avl_node_t *TreeBalance(avl_node_t *node)
{
	int diff = NodeHeaviness(node);

	if (diff > 1)
	{
		if (0 > NodeHeaviness(node->right))
		{
			node->right = RotateTreeRight(node->right);
		}

		node = RotateTreeLeft(node);
	}
	else if (diff < -1)
	{
		if (0 < NodeHeaviness(node->left))
		{
			node->left = RotateTreeLeft(node->left);
		}

		node = RotateTreeRight(node);
	}
	else
	{
		UpdateHeight(node);
	}

	return (node);
}
/******************************************************************************/

avl_tree_t *AvlTreeCreate(compare_function_t compare)
{
	avl_tree_t *avl = malloc(sizeof(avl_tree_t));

	if (NULL == avl)
	{
		return (NULL);
	}

	avl->root = NULL;
	avl->compare = compare;

	return (avl);
}
/******************************************************************************/

static void DestroyAndFreeNodes(avl_node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	DestroyAndFreeNodes(node->left);
	node->left = NULL;

	DestroyAndFreeNodes(node->right);
	node->right = NULL;

	free(node);
}

void AvlTreeDestroy(avl_tree_t *avl)
{
	assert(avl);

	DestroyAndFreeNodes(avl->root);

	avl->root = NULL;
	free(avl);
}
/******************************************************************************/

static size_t NodeCount(avl_node_t *node)
{
	if (NULL == node)
	{
		return (0);
	}
	return (1 + NodeCount(node->left) + NodeCount(node->right));
}

size_t AvlTreeCount(const avl_tree_t *avl)
{
	assert(avl);

	return (NodeCount(avl->root));
}
/******************************************************************************/

bool AvlTreeIsEmpty(const avl_tree_t *avl)
{
	assert(avl);

	return(!avl->root);
}
/******************************************************************************/

static avl_node_t *InsertNode(avl_node_t *node, void *data,
	                            compare_function_t compare)
{
	if (NULL == node)
	{
		return (NodeCreate(data));
	}

	assert(0 != compare(node->data, data));

	if (0 < compare(node->data, data))
	{
		node->left = InsertNode(node->left, data, compare);

		if (NULL == node->left) /* MEMORY_ALLOCATION_FAIL */
		{
			return (NULL);
		}
	}
	else
	{
		node->right = InsertNode(node->right, data, compare);

		if (NULL == node->right) /* MEMORY_ALLOCATION_FAIL */
		{
			return (NULL);
		}
	}

	return (TreeBalance(node));
}

status AvlTreeInsert(avl_tree_t *avl, void *data)
{
	avl_node_t *original_root = NULL;
	assert(avl);

	original_root = avl->root;

	avl->root = InsertNode(avl->root, data, avl->compare);

	if (NULL == avl->root)
	{
		avl->root = original_root;
		return (MEMORY_ALLOCATION_FAIL);
	}

	return (SUCCESS);
}
/******************************************************************************/

static avl_node_t *FindMostLeftyNode(avl_node_t *node)
{
	if (NULL == node->left)
	{
		return (node);
	}

	return (FindMostLeftyNode(node->left));
}

static avl_node_t *NodeRemove(avl_node_t *node_to_remove)
{
	avl_node_t *the_eldest_child = node_to_remove->left;

	if (NULL != node_to_remove->right)
	{
		the_eldest_child = node_to_remove->right;
		FindMostLeftyNode(the_eldest_child)->left = node_to_remove->left;
	}

	node_to_remove->left = NULL;
	node_to_remove->right = NULL;
	free (node_to_remove);

	return (the_eldest_child);
}

static avl_node_t *FindNodeToRemove(avl_node_t *node,
	                  compare_function_t compare, const void *data_to_match)
{
	if (NULL == node)
	{
		return (NULL);
	}

	if (0 < compare(node->data, data_to_match))
	{
		node->left = FindNodeToRemove(node->left, compare, data_to_match);
	}
	else if (0 > compare(node->data, data_to_match))
	{
		node->right  = FindNodeToRemove(node->right, compare, data_to_match);
	}
	else
	{
		return (NodeRemove(node));
	}

	return (TreeBalance(node));
}

void AvlTreeRemove(avl_tree_t *avl, const void *data)
{
	assert(avl);

	avl->root = FindNodeToRemove(avl->root, avl->compare, data);
}
/******************************************************************************/

static void *LocateData(avl_node_t *node, compare_function_t compare,
	                    const void *data_to_match)
{
	void *data;

	if (NULL == node)
	{
		return (NULL);
	}

	if (0 == compare(node->data, data_to_match))
	{
		return (node->data);
	}

	if (0 < compare(node->data, data_to_match))
	{
		data = LocateData(node->left, compare, data_to_match);
	}
	else
	{
		data = LocateData(node->right, compare, data_to_match);
	}

	return (data);
}

void *AvlTreeFind(avl_tree_t *avl, const void *data_to_match)
{
	assert(avl);

	return (LocateData(avl->root, avl->compare, data_to_match));
}
/******************************************************************************/

static status ForEachNode(avl_node_t *node, action_function_t action, void *param)
{
	status stat = SUCCESS;

	if (NULL == node)
	{
		return (SUCCESS);
	}

	stat = ForEachNode(node->left, action, param);

	if (SUCCESS != stat)
	{
		return (stat);
	}

	stat = action(node->data, param);

	if (SUCCESS != stat)
	{
		return (stat);
	}

	stat = ForEachNode(node->right, action, param);
	return (stat);
}

status AvlTreeForEach(avl_tree_t *avl, action_function_t action, void *param)
{
	assert(avl);

	return (ForEachNode(avl->root, action, param));
}
/******************************************************************************/

size_t AvlTreeHeight(const avl_tree_t *avl)
{
	assert(avl);

	if (NULL == avl->root)
	{
		return (EMPTY);
	}

	return (avl->root->height);
}
/******************************************************************************/

d_linked_list_t *AvlTreeMultiFind(
	avl_tree_t *avl, match_function_t match, void *param);
/******************************************************************************/

void AvlTreeMultiRemove(avl_tree_t *avl, match_function_t match, void *param);
/******************************************************************************/
/******************************************************************************/
