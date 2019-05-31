/*******************************************************************************
*Created BY Bareket Sar Shalom                                                 *
*Date: 9.12.18                                                                *
*******************************************************************************/
#include <stdlib.h>/*******************malloc free*****************************/
#include <stdio.h>/*******************malloc free*****************************/
#include <assert.h>/***********************assert******************************/
#include <string.h> /* memcpy */
#include "sorting.h"
/******************************************************************************/
#define RIGHT_CHILD(i) ((i * 2) + 2)
#define LEFT_CHILD(i) ((i * 2) + 1)
#define PARENT(i) ((i - 1) / 2)
#define BYTE_RANGE 256
#define THRESHOLD 10
#define CURR (void*)(char*)array + (element_size * i)
#define LEFT_SIDE (char*)array + (element_size * LEFT_CHILD(i))
/******************************************************************************/
void Swap (void *element1, void *element2, size_t elements_size)
{
	char *temp = malloc(elements_size);

	if (NULL == temp)
	{
		return;
	}

	memcpy(temp, element2, elements_size);
	memcpy(element2, element1, elements_size);
	memcpy(element1, temp, elements_size);

	free(temp);
}
void SwitchData(void *param1, void *param2, size_t element_size)
{
    char *char_param1 = (char *)param1;
    char *char_param2 = (char *)param2;

    if (char_param1 != char_param2)
    {
        for(; element_size > 0; --element_size, ++char_param1, ++char_param2)
        {
            *char_param1 ^= *char_param2;
            *char_param2 ^= *char_param1;
            *char_param1 ^= *char_param2;
        }
    }
}
/******************************************************************************/
void BubbleSort(void *elements, size_t element_size, size_t element_count,
                compare_func_t compare)
{
	size_t i = 0;
	bool continue_swapping = TRUE;

	while (continue_swapping)
	{
		continue_swapping = FALSE;

		for (i = 0; i < element_count - 1; ++i)
		{
			if (compare((char*)elements + i * element_size,
								(char *)elements + ((i + 1) * element_size)) > 0)
			{
				Swap ((void*)((char*)elements + (i * element_size)),
						 (void*)((char*)elements + ((i + 1) * element_size)), element_size);

					continue_swapping = TRUE;
			}
		}
	}
}
/******************************************************************************/
void InsertionSort(void *elements, size_t element_size, size_t element_count,
                   compare_func_t compare)
{
	size_t i, j;

	for (i = 1; i < element_count; ++i)
	{

	j = i;
		while (j > 0 && compare((char*)elements + j * element_size,
							(char *)elements + ((j - 1) * element_size)) < 0)
		{
			Swap ((void*)((char*)elements + (j * element_size)),
				 (void*)((char*)elements + ((j - 1) * element_size)), element_size);
			--j;
		}
	}
}
/******************************************************************************/
void SelectionSort(void *elements, size_t element_size, size_t element_count,
                   compare_func_t compare)
{
	size_t i, j;
	void *min;

	for (i = 0; i < element_count - 1; ++i)
	{
		min = (char*)elements + i * element_size;

		for (j = i + 1; j < element_count; ++j)
		{
			if (compare(min,(char *)elements + (j * element_size)) > 0)
			{
				min = (char *)elements + (j * element_size);
			}
		}

		Swap ((void*)((char*)elements + (i * element_size)), (void*)(min),
																																element_size);
	}
}
/******************************************************************************/
void HeapifyDown(void *array, size_t max, size_t i, size_t element_size,
																													compare_func_t compare)
{
	void *left_side = NULL;
	void *right_side = NULL;
	void *current = NULL;
	int largest = i;
	left_side = (char*)array + (element_size * LEFT_CHILD(i));
	right_side = (char*)array + (element_size * RIGHT_CHILD(i));
	current = (char*)array + (element_size * i);

	if (max <= LEFT_CHILD(i))
	{
			return ;
	}

	if (compare(current, left_side) < 0)
	{
		largest = LEFT_CHILD(i);
	}

	if (RIGHT_CHILD(i) < max)
	{
		if (compare(((char*)array + (element_size * largest)), right_side) < 0)
		{
			largest = RIGHT_CHILD(i);
		}
	}

	if (largest != i)
	{
		SwitchData((void*)(current), (void*)((char*)array + (element_size * largest)), element_size);
		HeapifyDown(array, max, largest, element_size, compare);
	}
}

void HeapSort(void *elements, size_t element_size, size_t element_count,
                   compare_func_t compare)
{
	int i = 0, j;

	for (i = (element_count / 2) - 1; i >= 0; --i)
	{
		HeapifyDown(elements, element_count, i, element_size, compare);
	}

	for (j = element_count; j > 0 ; --j)
	{
		void *current = (char*)elements + (element_size * (j - 1));
		SwitchData((void*)elements, current, element_size);
		element_count--;
		HeapifyDown(elements, element_count, 0, element_size, compare);
	}
}
/******************************************************************************/
static int Partition(void *array, size_t low, size_t high, size_t element_size,
	compare_func_t compare, size_t element_count)
{
	void* pivot = (char*)array + (element_size * (high -1));
	int j = low , i = (low - 1);

	for (; j < high; j++)
	{
		if (compare((char*)array + (element_size * j), pivot) < 0)
		{
			i++;
			Swap((void*)((char*)array + (element_size * i)), (void*)((char*)array + (element_size * j)),element_size);
		}
	}

	Swap((void*)((char*)array + (element_size * (i + 1))), (void*)((char*)array + (element_size * (high - 1))),element_size);

	return (i + 1);
}

void QuickSortWrapper(void *array, size_t low, size_t high, size_t element_size,
																													compare_func_t compare, size_t element_count)
{
	if (low < high)
	{
		int Part = Partition(array, low, high,  element_size, compare, element_count);
		QuickSortWrapper(array, low, Part, element_size, compare, element_count);
		QuickSortWrapper(array, Part + 1, high, element_size, compare, element_count);
	}
}

void QuickSort(void *elements, size_t element_size, size_t element_count,
        																								compare_func_t compare)
	{
		QuickSortWrapper(elements, 0, element_count, element_size, compare, element_count);
	}
/******************************************************************************/
void Merge(void *left, void *right, void *middle, void *elements,
	 					size_t element_count, size_t element_size, compare_func_t compare)
{
	void *left_p = left;
	void *middle_p = middle;
	char *element_p = elements;

	while ((middle_p > left_p) && (middle_p <= right))
	{
		if (0 >= compare(left_p,middle_p))
		{
		  memcpy(elements, left, element_size);
		  left_p = (char*)left_p + element_size;
		  element_p = (char*)element_p + element_size;
		}
		else
		{
		  memcpy(elements, middle, element_size);
		  middle_p = (char*)middle_p + element_size;
		  element_p = (char*)element_p + element_size;
		}
	}

	while (middle > left_p)
	{
		memcpy(element_p, left_p, element_size);
		left_p = (char*)left_p + element_size;
		element_p = (char*)element_p + element_size;
	}

	while (middle_p <= right)
	{
		memcpy(elements, middle, element_size);
		middle_p = (char*)middle_p + element_size;
		element_p = (char*)element_p + element_size;
	}

	memcpy(left, elements, (element_count * element_size));

}
/******************************************************************************/
void SplitArray(void *left, void *right, size_t element_count,
	 			size_t element_size, void *elements, compare_func_t compare)
{
	void *middle = (char*)left + ((element_count / 2) * element_size);
	size_t size = element_count / 2;

	if (right > left)
	{
		SplitArray(left, (char*)middle - element_size, size, element_size, elements, compare);
		SplitArray(middle , right, element_count - size, element_size, elements, compare);
		Merge(left, right, middle, elements, element_count, element_size, compare);
	}
}
/******************************************************************************/
void MergeSort(void *elements, size_t element_size, size_t element_count, compare_func_t compare)
{
	void *new_elements = malloc(element_count * element_size);
	void *right = (char*)elements + ((element_count - 1)* element_size);

	SplitArray(elements, right, element_count, element_size, new_elements, compare);

	free(new_elements);
}
/******************************************************************************/
status CountingSort(void *elements, size_t element_size,size_t element_count,
                  key_to_num_t key_to_num, void *param, size_t range)
{
	size_t i;
	char *element_pointer = elements;
	size_t *lut_array = NULL;
	char *sorted_array = NULL;
	size_t element_position;

	lut_array = calloc (range + 1, sizeof(size_t));

	if (NULL == lut_array)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}
	sorted_array = calloc (element_count, element_size);

	if (NULL == sorted_array)
	{
		free(lut_array);
		return (MEMORY_ALLOCATION_FAIL);
	}

	for (i = 1; i <= element_count ; ++i)
	{
 		++lut_array[key_to_num(element_pointer, param)];
		element_pointer = element_pointer + element_size;
	}

	for (i = 1; i <= range ; ++i)
	{
		lut_array [i] = lut_array[i] + lut_array[i-1];
	}

	element_pointer = element_pointer - element_size;

	for (i = element_count; i > 0 ; --i)
	{
		element_position = lut_array[key_to_num(element_pointer, param)] - 1;
		--lut_array[key_to_num(element_pointer, param)];
		memcpy(sorted_array + element_position * element_size,
		element_pointer, element_size);

		element_pointer -= element_size;
	}

	memcpy(elements, sorted_array, (element_count)*(element_size));
	free(lut_array);
	free(sorted_array);

	return (SUCCESS);
}
/******************************************************************************/
typedef struct param_and_key_s
{
	void *param;
	int index;
	key_to_num_t key_to_num;
}param_and_key_t;

size_t key_to_num_wrapper (void *element, void *param_and_key)
{
	void* param = ((param_and_key_t*)param_and_key)->param;
	int i = ((param_and_key_t*)param_and_key)->index;
	key_to_num_t key_to_num = ((param_and_key_t*)param_and_key)->key_to_num;

	return ((key_to_num(element,param) >>  (i*8)) & 0xFF);
}
/******************************************************************************/
status RadixSort(void *elements, size_t element_size, size_t element_count,
				          	key_to_num_t key_to_num, void *param, size_t byte_count)
{
	size_t i;
	param_and_key_t *param_and_key = NULL;

	param_and_key = malloc(sizeof(param_and_key_t));

	if (NULL == param_and_key)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	param_and_key->param = param;
	param_and_key->key_to_num = key_to_num;

	for (i = 0; i < byte_count; ++i)
	{
		param_and_key->index = i;

		CountingSort(elements, element_size, element_count, key_to_num_wrapper,
				param_and_key, BYTE_RANGE);
	}

	free(param_and_key);

	return (SUCCESS);
}
/******************************************************************************/
