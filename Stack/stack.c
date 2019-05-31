/*******************************************************************************
* NAME: Stack                                                                  *
*                                                                              *
* PURPOSE: Stack Data Structure           				       *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  06.11.18                                                      *
*******************************************************************************/
#include <stdio.h> /* size_t */
#include <stdlib.h> /* malloc, calloc */
#include <string.h> /*  memcpy */
#include <assert.h>	/* assert */
#include "stack.h"

struct stack_s
{
	size_t element_size;
	size_t capacity;
	void *array_address;
	void *current_pos;
};
/******************************************************************************/

stack_t *StackCreate(size_t capacity, size_t element_size)
{
	void *array = calloc(capacity, element_size);
	stack_t *stack_var = malloc(sizeof(stack_t));
	assert(array);
	assert(stack_var);

	stack_var -> element_size = element_size;
	stack_var -> capacity = capacity;
	stack_var -> array_address = array;
	stack_var -> current_pos = array;

	return(stack_var);
}

void StackDestroy(stack_t **s)
{
	free((*s) -> array_address);
	free(*s);
}
/******************************************************************************/

size_t StackGetSize(stack_t *s)
{
	size_t diff = (size_t)((char*)(s -> current_pos) - (char*)(s -> array_address));
	return(diff / s -> element_size);
}
/******************************************************************************/

void StackPush(stack_t *s, void *element)
{
	/*assert((char *)(s -> current_pos) < ((char *)(s -> array_address) + s -> capacity));*/

	memcpy(s -> current_pos, element, s -> element_size);

	(s -> current_pos) =(char *)(s -> current_pos) + (s -> element_size);
}

void StackPop(stack_t *s)
{
	assert(s -> current_pos > s -> array_address);

	(s -> current_pos) =(char *)(s -> current_pos) - (s -> element_size);
}

void *StackPeek(stack_t *s)
{
	return (((char *)s -> current_pos) - (s -> element_size));
}
/******************************************************************************/

size_t StackGetElementSize(stack_t *s)
{
	return(s -> element_size);
}

size_t StackGetCapacity(stack_t *s)
{
	return(s -> capacity);
}
/*******************************************************************************
*******************************************************************************/
