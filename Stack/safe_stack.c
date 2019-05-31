/*******************************************************************************
* NAME: Safe_Stack                                                             *
*                                                                              *
* PURPOSE:                                               								       *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  06.11.18                                                      *
*******************************************************************************/
#include <stdio.h> /* size_t */
#include "safe_stack.h"
#include "stack.h"

/******************************************************************************/

safe_stack_t *SafeStackCreate(size_t capacity, size_t element_size)
{
	return(StackCreate(capacity, element_size));
}

void SafeStackDestroy(safe_stack_t **s)
{
	StackDestroy(s);
}
/******************************************************************************/

size_t SafeStackGetSize(safe_stack_t *s)
{
	return(StackGetSize(s));
}
/******************************************************************************/

int SafeStackPush(safe_stack_t *s, void *element)
{
	if(StackGetSize(s) == StackGetCapacity(s))
	{
		return(0);
	}
	else
	{
		StackPush(s, element);
	}

	return(1);
}

int SafeStackPop(safe_stack_t *s)
{
	if(0 == StackGetSize(s))
	{
		return(0);
	}
	else
	{
		StackPop(s);
	}

	return(1);
}

void *SafeStackPeek(safe_stack_t *s)
{
	if(0 == StackGetSize(s))
	{
		return(NULL);
	}

	return (StackPeek(s));
}
/******************************************************************************/

size_t SafeStackGetElementSize(safe_stack_t *s)
{
	return(StackGetElementSize(s));
}

size_t SafeStackGetCapacity(safe_stack_t *s)
{
	return(StackGetCapacity(s));
}
/*******************************************************************************
*******************************************************************************/
