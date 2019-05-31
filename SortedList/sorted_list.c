/*******************************************************************************
* NAME: Sorted Linked List C file                                              *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  21.11.18                                                      *
*******************************************************************************/
#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, free */
#include <assert.h>	/* assert */

#include "sorted_list.h"
#include "d_linked_list.h"

struct sorted_list_s
{
	d_linked_list_t *dll;
	is_before_t is_before;
};
/******************************************************************************/

sorted_list_t *SortedListCreate(is_before_t is_before)
{
	sorted_list_t *sl = malloc(sizeof(sorted_list_t));

	if (NULL == sl)
	{
		return (NULL);
	}

	sl->dll = DLinkedListCreate();
	sl->is_before = is_before;

	return (sl);
}
/******************************************************************************/

void SortedListDestroy(sorted_list_t *sl)
{
	assert(sl);

	DLinkedListDestroy(sl->dll);
	free(sl);
}
/******************************************************************************/

size_t SortedListGetSize(const sorted_list_t *sl)
{
	assert(sl);

	return (DLinkedListGetSize(sl->dll));
}
/******************************************************************************/

bool SortedListIsEmpty(const sorted_list_t *sl)
{
	assert(sl);

	return (DLinkedListIsEmpty(sl->dll));
}
/******************************************************************************/

sorted_iterator_t SortedListBegin(const sorted_list_t *sl)
{
	sorted_iterator_t sl_iter;
	dll_iterator_t dll_iter;

	assert(sl);

	dll_iter = DLinkedListBegin(sl->dll);
	sl_iter.info = dll_iter.info;

	return (sl_iter);
}
/******************************************************************************/

sorted_iterator_t SortedListEnd(const sorted_list_t *sl)
{
	sorted_iterator_t sl_iter;
	dll_iterator_t dll_iter;

	assert(sl);

	dll_iter = DLinkedListEnd(sl->dll);
	sl_iter.info = dll_iter.info;
	return (sl_iter);
}
/******************************************************************************/

sorted_iterator_t SortedListInsert(sorted_list_t *sl, void *data)
{
	sorted_iterator_t sl_iter;
	dll_iterator_t dll_iter;

	assert(sl);

		dll_iter = DLinkedListFind(sl->dll, sl->is_before, data);

		if (NULL == dll_iter.info)
		{
			dll_iter = DLinkedListPushBack(sl->dll, data);
		}
		else
		{
			dll_iter = DLinkedListInsert(dll_iter, data);
		}

	sl_iter.info = dll_iter.info;

	return (sl_iter);
}
/******************************************************************************/

void SortedListErase(sorted_iterator_t iter)
{
	dll_iterator_t dll_iter;
	assert(iter.info);

	dll_iter.info = iter.info;
	DLinkedListErase(dll_iter);

}
/******************************************************************************/

void SortedListPopFront(sorted_list_t *sl)
{
	assert(sl);
	DLinkedListPopFront(sl->dll);
}
/******************************************************************************/

void SortedListPopBack(sorted_list_t *sl)
{
	assert(sl);
	DLinkedListPopBack(sl->dll);
}
/******************************************************************************/

sorted_iterator_t SortedListFind
	(sorted_list_t *sl, match_function_t match, void *param)
{
	sorted_iterator_t sl_iter;

	assert(sl);

	sl_iter.info = DLinkedListFind(sl->dll, match, param).info;

	return (sl_iter);
}
/******************************************************************************/

status SortedListForEach
	(sorted_list_t *sl, action_function_t action, void *param)
{
	assert(sl);
	return (DLinkedListForEach(sl->dll, action, param));
}
/******************************************************************************/

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
	dll_iterator_t dll_iter_dest;
	dll_iterator_t dll_iter_dest_end;
	dll_iterator_t dll_iter_src_from;
	dll_iterator_t dll_iter_src_to;
	dll_iterator_t dll_iter_src_end;

	assert(dest);
	assert(src);

	dll_iter_dest = DLinkedListBegin(dest->dll);
	dll_iter_dest_end = DLinkedListEnd(dest->dll);
	dll_iter_src_from = DLinkedListBegin(src->dll);
	dll_iter_src_to = DLinkedListBegin(src->dll);
	dll_iter_src_end = DLinkedListEnd(src->dll);

	while(!DLinkedListIsEmpty(src->dll) &&
		  (dll_iter_dest.info != dll_iter_dest_end.info))
	{
		while ((dll_iter_src_to.info != dll_iter_src_end.info) &&
			   (TRUE == dest->is_before(DLinkedListGetData(dll_iter_dest),
			   						DLinkedListGetData(dll_iter_src_to))))
		{
			dll_iter_src_to = DLinkedListGetNext(dll_iter_src_to);
		}

		DLinkedListSplice(dll_iter_dest, dll_iter_src_from, dll_iter_src_to);
		dll_iter_src_from = dll_iter_src_to;
		dll_iter_dest = DLinkedListGetNext(dll_iter_dest);
	}

	if (!DLinkedListIsEmpty(src->dll))
	{
		DLinkedListSplice(dll_iter_dest, DLinkedListBegin(src->dll),
						  DLinkedListEnd(src->dll));
	}
}
/******************************************************************************/

sorted_iterator_t SortedListGetNext(sorted_iterator_t iter)
{
	dll_iterator_t dll_iter;
	assert(iter.info);

	dll_iter.info = iter.info;
	dll_iter = DLinkedListGetNext(dll_iter);
	iter.info = dll_iter.info;

	return (iter);

}
/******************************************************************************/

sorted_iterator_t SortedListGetPrevious(sorted_iterator_t iter)
{
	dll_iterator_t dll_iter;
	assert(iter.info);

	dll_iter.info = iter.info;
	dll_iter = DLinkedListGetPrevious(dll_iter);
	iter.info = dll_iter.info;

	return (iter);
}
/******************************************************************************/

void *SortedListGetData(sorted_iterator_t iter)
{
	dll_iterator_t dll_iter;
	assert(iter.info);

	dll_iter.info = iter.info;

	return (DLinkedListGetData(dll_iter));
}
/******************************************************************************/

bool SortedListIsSameIterator(
	const sorted_iterator_t iter1, const sorted_iterator_t iter2)
{
	assert(iter1.info);
	assert(iter2.info);

	if (iter1.info == iter2.info)
	{
		return (TRUE);
	}

	return (FALSE);
}
/*******************************************************************************
*******************************************************************************/
