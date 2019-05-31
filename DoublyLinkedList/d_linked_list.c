/*******************************************************************************
* NAME: Doubly Linked List C file.                                             *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  18.11.18                                                      *
*******************************************************************************/

#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, free */
#include <assert.h>	/* assert */
#include "d_linked_list.h"

#define NUM_DUMMIES 2

typedef struct dll_node_s dll_node_t;

struct dll_node_s
{
	void *data;
	dll_node_t *next;
	dll_node_t *prev;
};

struct d_linked_list_s
{
	dll_node_t *head;
	dll_node_t *tail;
};
/******************************************************************************/

dll_node_t *UtilityDllNodeCreate(void *data)
{
	dll_node_t *dll_node = malloc(sizeof(dll_node_t));

	if (NULL == dll_node)
	{
		return (NULL);
	}

	dll_node->data = data;

	return(dll_node);
}
/******************************************************************************/
d_linked_list_t *DLinkedListCreate()
{
	dll_node_t *dummy_head = NULL;
	dll_node_t *dummy_tail = NULL;
	d_linked_list_t *dll = malloc(sizeof(d_linked_list_t));

	if (NULL == dll)
	{
		return (NULL);
	}

	dummy_head = UtilityDllNodeCreate(NULL);
	dummy_tail = UtilityDllNodeCreate(NULL);

	dll->head = dummy_head;
	dll->tail = dummy_tail;
	dummy_head->next = dummy_tail;
	dummy_head->prev = NULL;
	dummy_tail->next = NULL;
	dummy_tail->prev = dummy_head;

	return (dll);
}
/******************************************************************************/

void DLinkedListDestroy(d_linked_list_t *dll)
{
	dll_node_t *next = NULL;
	assert(dll);

	while ((dll)->head)
	{
		next = (dll)->head->next;
		free ((dll)->head);
		(dll)->head = next;
	}

	free(dll);
}
/******************************************************************************/

bool DLinkedListIsEmpty(d_linked_list_t *dll)
{
	assert(dll);

	if (dll->tail == dll->head->next)
	{
		return (TRUE);
	}

	return (FALSE);
}
/******************************************************************************/

size_t DLinkedListGetSize(d_linked_list_t *dll)
{
	size_t counter = 0;
	dll_node_t *node = NULL;

	assert(dll);

	node = dll->head;

	while (node)
	{
		node = node->next;
		++counter;
	}

	return (counter - NUM_DUMMIES);
}
/******************************************************************************/

dll_iterator_t DLinkedListBegin(d_linked_list_t *dll)
{
	dll_iterator_t iter;

	assert(dll);

	iter.info = (void*)(dll->head->next);
	return (iter);
}
/******************************************************************************/

dll_iterator_t DLinkedListEnd(d_linked_list_t *dll)
{
	dll_iterator_t iter;

	assert(dll);

	iter.info = (void*)(dll->tail);
	return (iter);
}
/******************************************************************************/

dll_iterator_t DLinkedListInsert(dll_iterator_t iter, void *data)
{
	dll_node_t *new_node = NULL;
	dll_node_t *info = NULL;

	assert(iter.info);

	info = (dll_node_t*)iter.info;
	new_node = UtilityDllNodeCreate(data);

	new_node->prev = info->prev;
	new_node->next = info;
	info->prev = new_node;
	new_node->prev->next = new_node;

	iter.info = new_node;
	return (iter);
}
/******************************************************************************/

dll_iterator_t DLinkedListInsertAfter(dll_iterator_t iter, void *data)
{
	dll_node_t *new_node = NULL;
	dll_node_t *info = NULL;

	assert(iter.info);

	info = (dll_node_t*)iter.info;
	new_node = UtilityDllNodeCreate(data);

	new_node->prev = info;
	new_node->next = info->next;
	info->next = new_node;
	new_node->next->prev = new_node;

	iter.info = new_node;
	return (iter);
}
/******************************************************************************/

void DLinkedListErase(dll_iterator_t iter)
{
	dll_node_t *info = NULL;
	assert(iter.info);

	info = (dll_node_t*)iter.info;

	info->prev->next = info->next;
	info->next->prev = info->prev;
	info->prev = NULL;
	info->next = NULL;
	free(info);
}
/******************************************************************************/

dll_iterator_t DLinkedListPushFront(d_linked_list_t *dll, void *data)
{
	dll_node_t *new_node = NULL;
	dll_iterator_t iter;
	assert(dll);

	new_node = UtilityDllNodeCreate(data);

	new_node->prev = dll->head;
	new_node->next = dll->head->next;
	dll->head->next = new_node;
	new_node->next->prev = new_node;

	iter.info = new_node;
	return (iter);
}
/******************************************************************************/

dll_iterator_t DLinkedListPushBack(d_linked_list_t *dll, void *data)
{
	dll_node_t *new_node = NULL;
	dll_iterator_t iter;

	assert(dll);

	new_node = UtilityDllNodeCreate(data);

	new_node->next = dll->tail;
	new_node->prev = dll->tail->prev;
	dll->tail->prev = new_node;
	new_node->prev->next = new_node;

	iter.info = new_node;
	return (iter);
}
/******************************************************************************/

void DLinkedListPopFront(d_linked_list_t *dll)
{
	dll_node_t *removed = NULL;

	assert(dll);
	assert(FALSE == DLinkedListIsEmpty(dll));

	removed = dll->head->next;

	dll->head->next = removed->next;
	removed->next->prev = dll->head;
	removed->prev = NULL;
	removed->next = NULL;
	free(removed);
}
/******************************************************************************/

void DLinkedListPopBack(d_linked_list_t *dll)
{
	dll_node_t *removed = NULL;

	assert(dll);
	assert(FALSE == DLinkedListIsEmpty(dll));

	removed = dll->tail->prev;

	dll->tail->prev = removed->prev;
	removed->prev->next = dll->tail;
	removed->prev = NULL;
	removed->next = NULL;
	free(removed);
}
/******************************************************************************/

dll_iterator_t DLinkedListFind
	(d_linked_list_t *dll, match_function_t match, void *param)
{
	dll_node_t *node = NULL;
	dll_iterator_t iter;

	assert(dll);
	assert(match);

	node = dll->head->next;

	while (node != dll->tail)
	{
		if (TRUE == match(node->data, param))
		{
			iter.info = node;
			return (iter);
		}

		node = node->next;
	}

	iter.info = NULL;
	return (iter);

}
/******************************************************************************/

status DLinkedListForEach
	(d_linked_list_t *dll, action_function_t action, void *param)
{
	dll_node_t *iter = NULL;

	assert(dll);
	assert(action);

	iter = dll->head->next;

	while (iter != dll->tail)
	{
		if (ACTION_FAIL == action(iter->data, param))
		{
			return (ACTION_FAIL);
		}

		iter = iter->next;
	}

	return (SUCCESS);
}
/******************************************************************************/

void DLinkedListSplice(
	dll_iterator_t where, dll_iterator_t from, dll_iterator_t to)
{
	dll_node_t *prev = NULL;
	dll_node_t *where_node = NULL;
	dll_node_t *from_node = NULL;
	dll_node_t *to_node = NULL;
	dll_node_t *from_prev_node = NULL;

	assert(where.info && from.info && to.info);

	where_node = (dll_node_t*)where.info;
	from_node = (dll_node_t*)from.info;
	to_node = (dll_node_t*)to.info;
	from_prev_node = (dll_node_t*)DLinkedListGetPrevious(from).info;

	prev = where_node->prev;

	from_node->prev = prev;
	prev->next = from_node;
	to_node->prev->next = where_node;
	where_node->prev = to_node->prev;

	from_prev_node->next = to_node;
	to_node->prev = from_prev_node;
}
/******************************************************************************/

dll_iterator_t DLinkedListGetNext(dll_iterator_t iter)
{
	dll_node_t *info = NULL;
	assert(iter.info);

	info = (dll_node_t*)iter.info;
	info = info->next;
	iter.info = info;

	return (iter);
}
/******************************************************************************/

dll_iterator_t DLinkedListGetPrevious(dll_iterator_t iter)
{
	dll_node_t *info = NULL;
	assert(iter.info);

	info = (dll_node_t*)iter.info;
	info = info->prev;
	iter.info = info;

	return (iter);
}
/******************************************************************************/

void *DLinkedListGetData(dll_iterator_t iter)
{
	dll_node_t *info = NULL;
	assert(iter.info);

	info = (dll_node_t*)iter.info;
	return (info->data);
}
/******************************************************************************/

bool DLinkedListIsSameIterator(dll_iterator_t iter1, dll_iterator_t iter2)
{
	assert(iter1.info);
	assert(iter2.info);

	if (iter1.info == iter2.info)
	{
		return (TRUE);
	}

	return (FALSE);
}
/******************************************************************************/
/******************************************************************************/
