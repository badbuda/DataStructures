/*******************************************************************************
* NAME: Queue                                                                  *
*                                                                              *
* PURPOSE:                                                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  15.11.18                                                      *
*******************************************************************************/
#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, free */
#include <assert.h>	/* assert */
#include "queue.h"
#include "s_linked_list.h"

struct queue_s
{
	sll_node_t *head;
	sll_node_t *tail;
};
/******************************************************************************/

queue_t *QueueCreate()
{
	sll_node_t *dummy;
	queue_t *q = malloc(sizeof(queue_t));

	if (NULL == q)
	{
		return(NULL);
	}

	dummy = SLinkedListCreate(NULL, NULL);

	if (NULL == dummy)
	{
		return (NULL);
	}

	q->tail = dummy;
	q->head = dummy;

	return (q);
}
/******************************************************************************/

void QueueDestroy(queue_t **q)
{
	assert(*q);

	SLinkedListDestroy(&((*q)->head));
	free(*q);
	*q = NULL;
}
/******************************************************************************/

status QueueEnqueue(queue_t *q, void* data)
{
	sll_node_t *new_tail = NULL;
	sll_node_t *node = NULL;

	assert(q);

	node = SLinkedListCreate(data, NULL);

	if (NULL == node)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	new_tail = node;

	SLinkedListInsert(q->tail, node);
	q->tail = new_tail;

	return (SUCCESS);
}
/******************************************************************************/

void *QueueDequeue(queue_t *q)
{
	void *head_data = NULL;
	sll_node_t *old_head = NULL;

	assert(q);
	assert(0 != QueueGetSize);

	head_data = q->head->data;
	old_head  = q->head;
	q->head = q->head->next;
	free(old_head);
	old_head = NULL;

	return (head_data);
}
/******************************************************************************/

void *QueuePeek(queue_t *q)
{
	assert(q);

	return(q->head->data);
}
/******************************************************************************/

size_t QueueGetSize(const queue_t *q)
{
	assert(q);

	return (SLinkedListGetSize(q->head) - 1);
}
/******************************************************************************/

bool QueueIsEmpty(const queue_t *q)
{
	assert(q);

	if (NULL == q->head->next)
	{
		return (TRUE);
	}

	return (FALSE);
}
/******************************************************************************/
void QueueAppend(queue_t *dest, queue_t *src)
{
	assert(dest);
	assert(src);

	dest->tail->data = src->head->data;
	dest->tail->next = src->head->next;
	free(src->head);
	src->head = NULL;
	dest->tail = src->tail;
	src->tail = NULL;
}
/******************************************************************************/
/******************************************************************************/
