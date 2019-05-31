/*******************************************************************************
* NAME: Stack header                                                           *
*                                                                              *
* PURPOSE:                              					                			       *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  05.11.18                                                      *
*******************************************************************************/

#ifndef __DS_STACK_H__
#define __DS_STACK_H__

typedef struct stack_s stack_t;

stack_t *StackCreate(size_t capacity, size_t element_size);
/*creates a stack data structure*/

void StackDestroy(stack_t **s);
/*destroyed the data structure*/

size_t StackGetSize(stack_t *s);
/*return the size of the stack*/

void StackPush(stack_t *s, void *element);
/*add element to stack*/

void StackPop(stack_t *s);
/*remove first element from stack*/

void *StackPeek(stack_t *s);
/*return top element of the stack*/

size_t StackGetElementSize(stack_t *s);
/*return size of element of the stack*/

size_t StackGetCapacity(stack_t *s);
/*return capacity of the stack*/

#endif
