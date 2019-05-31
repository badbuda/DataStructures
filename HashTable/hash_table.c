/*******************************************************************************
* NAME: Hash Table                                                             *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  27.12.18                                                      *
*******************************************************************************/

#include <stdlib.h> /* size_t, malloc, free */
#include <assert.h> /* assert */

#include "hash_table.h"
#include "d_linked_list.h"

struct hash_table_s
{
	size_t number_of_keys;
	hash_function_t hash_function;
	match_function_t match;
	d_linked_list_t **lut;
};
/******************************************************************************/

hash_table_t *HashCreate(hash_function_t hash_function,
                         match_function_t match, size_t number_of_keys)
{
	size_t i = 0, j = 0;
	hash_table_t *hash = malloc(sizeof(hash_table_t) +
                                  (number_of_keys * sizeof(d_linked_list_t *)));
	assert(hash_function);
	assert(match);

	if (NULL != hash)
	{
		hash->number_of_keys = number_of_keys;
		hash->hash_function = hash_function;
		hash->match = match;
		hash->lut = (d_linked_list_t **)(&(hash->lut) + 1);
	}

	for ( ; i < number_of_keys; ++i)
	{
		hash->lut[i] = DLinkedListCreate();

		if (NULL == hash->lut[i])
		{
			for ( ; j < i; ++j)
			{
				DLinkedListDestroy(hash->lut[j]);
			}

			free(hash);
			hash = NULL;
			return (NULL);
		}
	}

	return (hash);
}
/******************************************************************************/

void HashDestroy(hash_table_t *hash)
{
	size_t i = 0;
	assert(hash);

	for ( ; i < hash->number_of_keys; ++i)
	{
		DLinkedListDestroy(hash->lut[i]);
	}

	hash->hash_function = NULL;
	hash->match = NULL;
	hash->lut = NULL;

	free(hash);
	hash = NULL;
}
/******************************************************************************/

size_t HashCount(const hash_table_t *hash)
{
	size_t i = 0, counter = 0;
	assert(hash);

	for ( ; i < hash->number_of_keys; ++i)
	{
		counter += DLinkedListGetSize(hash->lut[i]);
	}

	return (counter);
}
/******************************************************************************/

bool HashIsEmpty(const hash_table_t *hash)
{
	bool is_empty = TRUE;
	size_t i = 0;
	assert(hash);

	for ( ; i < hash->number_of_keys && is_empty; ++i)
	{
		is_empty = DLinkedListIsEmpty(hash->lut[i]);
	}

	return (is_empty);
}
/******************************************************************************/

status HashInsert(hash_table_t *hash, void *data)
{
	size_t index = 0;
	dll_iterator_t iter;
	assert(hash);

	iter.info = NULL;
	index = hash->hash_function(data);
	iter = DLinkedListPushFront(hash->lut[index], data);

	if (NULL == iter.info)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	return (SUCCESS);
}
/******************************************************************************/

void HashRemove(hash_table_t *hash, void *data)
{
	size_t index = 0;
	dll_iterator_t iter;
	assert(hash);

	index = hash->hash_function(data);
	iter = DLinkedListFind(hash->lut[index], hash->match, data);
	DLinkedListErase(iter);
}
/******************************************************************************/

void *HashFind(hash_table_t *hash, void *data_to_match)
{
	size_t index = 0;
	dll_iterator_t iter;
	void *find_data = NULL;

	assert(hash);

	index = hash->hash_function(data_to_match);
	iter = DLinkedListFind(hash->lut[index], hash->match, data_to_match);

	if (NULL != iter.info)
	{
		find_data = DLinkedListGetData(iter);
	}

	return (find_data);
}
/******************************************************************************/

status HashForEach(hash_table_t *hash, action_function_t action, void *param)
{
	status stat = SUCCESS;
	size_t i = 0;

	assert(hash);

	for ( ; (i < hash->number_of_keys) && (SUCCESS == stat); ++i)
	{
		stat = DLinkedListForEach(hash->lut[i], action, param);
	}

	return (stat);
}
/*************************EXTRAS***********************************************/

static size_t NumOfUsedKeys(const hash_table_t *hash)
{
 size_t i = 0;
 size_t counter = 0;

 for ( ; i < hash->number_of_keys; ++i)
 {
	 if (FALSE == DLinkedListIsEmpty(hash->lut[i]))
	 {
		 ++counter;
	 }
 }

 return (counter);
}

static size_t MaxNumOfElementsInKey(const hash_table_t *hash)
{
 size_t i = 0;
 size_t max = 0;

 for ( ; i < hash->number_of_keys; ++i)
 {
	 if (max < DLinkedListGetSize(hash->lut[i]))
	 {
		 max = DLinkedListGetSize(hash->lut[i]);
	 }
 }

 return (max);
}

void HashStatistics(const hash_table_t *hash)
{
	size_t max_num_of_elements = 0;
	size_t num_of_used_keys = 0;
	assert(hash);

	num_of_used_keys = NumOfUsedKeys(hash);
	max_num_of_elements = MaxNumOfElementsInKey(hash);

	printf("\nthe percentege of used keys is:%.2f%%\n",
	                  100.0 * (float)num_of_used_keys / hash->number_of_keys);
	printf("the average number of elements for key is:%.2f\n",
	                   HashCount(hash) / (float)num_of_used_keys);
	printf("the MAX number of elements in one key is:%lu\n",max_num_of_elements);
}
/******************************************************************************/
/******************************************************************************/
