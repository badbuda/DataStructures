/*******************************************************************************
* NAME: SafeStack header                                                       * 
*                                                                              *
* PURPOSE: A rapper to the original data structure						       *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  06.11.18                                                      *
*******************************************************************************/
#include <stdio.h>

#ifndef __DS_STACKSAFE_H__
#define __DS_STACKSAFE_H__

typedef struct stack_s safe_stack_t;

safe_stack_t *SafeStackCreate(size_t capacity, size_t element_size);
/*creates a stack data structure*/

void SafeStackDestroy(safe_stack_t **s);
/*destroyed the data structure*/

size_t SafeStackGetSize(safe_stack_t *s);
/*return the size of the stack*/

int SafeStackPush(safe_stack_t *s, void *element);
/*add element to stack*/

int SafeStackPop(safe_stack_t *s);
/*remove element to stack*/

void *SafeStackPeek(safe_stack_t *s);
/*return top element of the stack*/

size_t SafeStackGetElementSize(safe_stack_t *s);
/*return size of element of the stack*/

size_t SafeStackGetCapacity(safe_stack_t *s);
/*return capacity of the stack*/

#endif
