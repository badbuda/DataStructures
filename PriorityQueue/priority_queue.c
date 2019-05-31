/*******************************************************************************
* NAME: Priority Queue C File                                                  *
*                                                                              *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  21.11.18                                                      *
*******************************************************************************/
#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, free */
#include <assert.h>	/* assert */

#include "sorted_list.h"
#include "priority_queue.h"

struct p_queue_s
{
	sorted_list_t *sl;
};

/******************************************************************************/

p_queue_t *PQueueCreate(is_higher_priority_t is_higher_priority)
{
	p_queue_t *pq = malloc(sizeof(p_queue_t));

	if (NULL == pq)
	{
		free(pq);
		return (NULL);
	}

	pq->sl = SortedListCreate(is_higher_priority);

	return (pq);
}
/******************************************************************************/

void PQueueDestroy(p_queue_t *pq)
{
	assert(pq->sl);

	SortedListDestroy(pq->sl);
	free(pq);
}
/******************************************************************************/

status PQueueEnqueue(p_queue_t *pq, void* data)
{
	sorted_iterator_t sl_iter;
	assert(pq->sl);

	sl_iter = SortedListInsert(pq->sl, data);

	if (NULL == sl_iter.info)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	return (SUCCESS);
}
/******************************************************************************/

void *PQueueDequeue(p_queue_t *pq)
{
	void *result = NULL;
	assert(pq->sl);

	result = SortedListGetData(SortedListBegin(pq->sl));
	SortedListPopFront(pq->sl);

	return (result);
}
/******************************************************************************/

void *PQueuePeek(p_queue_t *pq)
{
	void *result = NULL;
	assert(pq->sl);

	result = SortedListGetData(SortedListBegin(pq->sl));

	return (result);
}
/******************************************************************************/

size_t PQueueGetSize(const p_queue_t *pq)
{
	assert(pq->sl);

	return (SortedListGetSize(pq->sl));
}
/******************************************************************************/

bool PQueueIsEmpty(const p_queue_t *pq)
{
	assert(pq->sl);

	return (SortedListIsEmpty(pq->sl));
}
/******************************************************************************/

void *PQueueRemove(p_queue_t *pq, match_function_t match, void *param)
{
	sorted_iterator_t sl_iter;
	void* data = NULL;
	assert(pq);

	sl_iter = SortedListFind(pq->sl, match, param);
	data = SortedListGetData(sl_iter);

	if (sl_iter.info != NULL)
	{
		SortedListErase(sl_iter);
		return (data);
	}

	return (NULL);
}
/*******************************************************************************
*******************************************************************************/
