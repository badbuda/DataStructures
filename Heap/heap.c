/*******************************************************************************
* NAME: Heap                                                                   *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  30.12.18                                                      *
*******************************************************************************/
#include <stdlib.h> /* size_t, malloc, free */
#include <assert.h> /* assert */

#include "heap.h"
#include "d_vector.h"

#define EMPTY 0
#define ROOT 0

struct heap_s
{
	compare_function_t compare;
	d_vector_t *vector;
};
/******************Utility Functions*******************************************/

static void Swap(void **item, void **data)
{
	void *tmp_ptr = *item;
	*item = *data;
	*data = tmp_ptr;
}

static void HeapifyUp(heap_t *heap, size_t element_index)
{
	void **new_data = NULL;
	void **compared_data = NULL;
	size_t compared_data_index = 0;

	if (ROOT == element_index)
	{
		return;
	}

	compared_data_index = (element_index - 1) / 2;
	new_data = DVectorGetItem(heap->vector, element_index);
	compared_data = DVectorGetItem(heap->vector, compared_data_index);

	if (0 < heap->compare(*compared_data, *new_data))
	{
		Swap(compared_data, new_data);
		HeapifyUp(heap, compared_data_index);
	}

	return;
}

static void HeapifyDown(heap_t *heap, size_t element_index)
{
	void **elemet_data = DVectorGetItem(heap->vector, element_index);
	void **left_child_data = DVectorGetItem(heap->vector, 2 * element_index + 1);
	void **right_child_data = NULL;
	void **biggest_child = left_child_data;
	size_t next_index = 2 * element_index + 1;

	if (NULL == left_child_data)
	{
		return;
	}

	right_child_data = DVectorGetItem(heap->vector, 2 * element_index + 2);

	if (NULL != right_child_data)
	{
		if ((0 < heap->compare(*left_child_data, *right_child_data)))
		{
			biggest_child = right_child_data;
			++next_index;
		}
	}

	if (0 > heap->compare(*biggest_child, *elemet_data))
	{
		Swap(biggest_child, elemet_data);
		HeapifyDown(heap, next_index);
	}
}
/******************************************************************************/

heap_t *HeapCreate(compare_function_t compare)
{
	heap_t *heap = NULL;
  assert(compare);

	heap = malloc(sizeof(heap_t));

	if (NULL != heap)
	{
		heap->compare = compare;

		if(NULL == (heap->vector = DVectorCreate(1, sizeof(void *))))
		{
			free(heap);
			return (NULL);
		}
	}

	return (heap);
}
/******************************************************************************/

void HeapDestroy(heap_t *heap)
{
	assert(heap);

	DVectorDestroy(heap->vector);
	heap->vector = NULL;
	free(heap);
}
/******************************************************************************/

size_t HeapCount(const heap_t *heap)
{
	assert(heap);

	return (DVectorGetSize(heap->vector));
}
/******************************************************************************/

bool HeapIsEmpty(const heap_t *heap)
{
	assert(heap);

	return (EMPTY == DVectorGetSize(heap->vector));
}
/******************************************************************************/

status HeapPush(heap_t *heap, void *data)
{
	assert(heap);
	assert(data);

	if(MEMORY_ALLOCATION_FAIL == DVectorPush(heap->vector, &data))
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	HeapifyUp(heap, HeapCount(heap) - 1);

	return (SUCCESS);
}
/******************************************************************************/

void *HeapPeek(heap_t *heap)
{
	void *root = NULL;
	assert(heap);
	assert(FALSE == HeapIsEmpty(heap));

	root = *(void **)DVectorGetItem(heap->vector, ROOT);

	return (root);
}
/******************************************************************************/

void HeapPop(heap_t *heap)
{
	void **data_to_pop = NULL;
	void **data_to_swap = NULL;
	assert(heap);
	assert(FALSE == HeapIsEmpty(heap));

	data_to_pop = DVectorGetItem(heap->vector, ROOT);
	data_to_swap = DVectorGetItem(heap->vector, HeapCount(heap) - 1);

	Swap(data_to_pop, data_to_swap);

	DVectorPop(heap->vector);

	 HeapifyDown(heap, ROOT);
}
/******************************************************************************/

void *HeapRemove (heap_t *heap, match_function_t match, void *data_to_match)
{
	size_t i = 0, heap_count = 0;
	void **compared_data_ptr = NULL;
	void *removed_data = NULL;

	assert(heap);
	assert(match);
	assert(data_to_match);

	heap_count =  HeapCount(heap);

	for ( ; i < heap_count; ++i)
	{
		compared_data_ptr = DVectorGetItem(heap->vector, i);

		if (TRUE == match(*compared_data_ptr, data_to_match))
		{
			void **data_to_swap_ptr = DVectorGetItem(heap->vector, heap_count - 1);
			void *swap_data = *data_to_swap_ptr;
			removed_data = *compared_data_ptr;

			Swap(compared_data_ptr, data_to_swap_ptr);
			DVectorPop(heap->vector);

			if (0 < heap->compare(swap_data, removed_data))
			{
				HeapifyDown(heap, i);
			}
			else if (0 > heap->compare(swap_data, removed_data))
			{
				HeapifyUp(heap, i);
			}

			break;
		}
	}

	return (removed_data);
}
/******************************************************************************/
/******************************************************************************/
