#include <stdlib.h> /* size_t */

#include "../include/enum_rd58.h"

#ifndef __SORTING_ALGORITHMS_H__
#define __SORTING_ALGORITHMS_H__

/*******************************************************************************
* NAME: Sorting Algorithms                                                     *
*                                                                              *
*Created BY Bareket Sar Shalom                                                 *
* DATE UPDATED:  9.12.18                                                       *
*******************************************************************************/

typedef int (*compare_func_t)(void *before, void *after);

void BubbleSort(void *elements, size_t element_size, size_t element_count,
                compare_func_t compare);
/*******************************************************************************
* Sorts the data with bubble sort algorithm                                    *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/
void InsertionSort(void *elements, size_t element_size, size_t element_count,
                   compare_func_t compare);
/*******************************************************************************
* Sorts the data with insertion sort algorithm                                 *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/
void SelectionSort(void *elements, size_t element_size, size_t element_count,
                   compare_func_t compare);
/*******************************************************************************
* Sorts the data with selection sort algorithm                                 *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/

void MergeSort(void *base, size_t nitems, size_t memSize, compare_func_t cmp);

typedef size_t (*key_to_num_t)(void *element, void *param);

status CountingSort(void *elements, size_t element_size,size_t element_count,
                  key_to_num_t key_to_num, void *param, size_t range);
/*******************************************************************************
* Sorts the data with counting sort algorithm                                  *
* Parameters:                                                                  *
*                                                                              *
* key_to_num - function that converts data to index                             *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
* param - parameter for key_to_num function                                    *
* range - the range of element values from zero to range                       *
*******************************************************************************/

status RadixSort(void *elements, size_t element_size, size_t element_count,
               key_to_num_t key_to_num, void *param, size_t byte_count);
/*******************************************************************************
* Sorts the data with radix sort algorithm                                     *
* Parameters:                                                                  *
*                                                                              *
* key_to_num - function that convert data to index                             *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
* param - parameter for key_to_num function                                    *
* byte_count - the number of bytes in the max numbers (four bytes at most)     *
*******************************************************************************/


#endif
