/*******************************************************************************
* NAME: Heap Priority Queue                                                    *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  30.12.18                                                      *
*******************************************************************************/

#include <stdlib.h> /* size_t, malloc, free */
#include <assert.h> /* assert */

#include "heap.h"
#include "heap_pq.h"

struct heap_p_queue_s
{
	heap_t *heap;
};
/******************************************************************************/

heap_p_queue_t *HeapPQueueCreate(compare_function_t priority)
{
  heap_p_queue_t *pq = malloc(sizeof(heap_p_queue_t));

	if (NULL == pq)
	{
		free(pq);
		return (NULL);
	}

	pq->heap = HeapCreate(priority);

	return (pq);
}
/******************************************************************************/

void HeapPQueueDestroy(heap_p_queue_t *pq)
{
  assert(pq->heap);

  HeapDestroy(pq->heap);
  free(pq);
}
/******************************************************************************/

status HeapPQueueEnqueue(heap_p_queue_t *pq, void* data)
{
	assert(pq->heap);

	return (HeapPush(pq->heap, data));
}
/******************************************************************************/

void *HeapPQueueDequeue(heap_p_queue_t *pq)
{
	void *removed_data = NULL;
	assert(pq->heap);

	removed_data = HeapPeek(pq->heap);
	HeapPop(pq->heap);

	return (removed_data);
}
/******************************************************************************/

void *HeapPQueuePeek(heap_p_queue_t *pq)
{
	assert(pq->heap);
	return (HeapPeek(pq->heap));
}
/******************************************************************************/

size_t HeapPQueueGetSize(const heap_p_queue_t *pq)
{
	assert(pq->heap);
	return (HeapCount(pq->heap));
}
/******************************************************************************/

bool HeapPQueueIsEmpty(const heap_p_queue_t *pq)
{
  assert(pq->heap);
  return (HeapIsEmpty(pq->heap));
}
/******************************************************************************/

void *HeapPQueueRemove(heap_p_queue_t *pq, match_function_t match, void *param)
{
  assert(pq->heap);
  return (HeapRemove(pq->heap, match, param));
}
/******************************************************************************/
/******************************************************************************/
