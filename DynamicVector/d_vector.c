#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, realloc, free */
#include <string.h> /*  memcpy */
#include <assert.h>	/* assert */
#include "d_vector.h"
/*******************************************************************************
* NAME: Dynamic Vector                                                         *
*                                                                              *
* PURPOSE: Dynamic Vector (ADT)                                                *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  07.11.18                                                      *
*******************************************************************************/

struct d_vector_s
{
	size_t element_size;
	size_t capacity;
	void *array;
	size_t head;
};
/******************************************************************************/

d_vector_t *DVectorCreate(size_t capacity, size_t element_size)
{
	d_vector_t *d_vector_var = malloc(sizeof(d_vector_t));

	if(NULL == d_vector_var)
	{
		return(NULL);
	}

	d_vector_var -> array = malloc(capacity * element_size);

	if(NULL == d_vector_var -> array)
	{
		return(NULL);
	}

	d_vector_var -> element_size = element_size;
	d_vector_var -> capacity = capacity;
	d_vector_var -> head = 0;

	return(d_vector_var);
}

void DVectorDestroy(d_vector_t *dv)
{
	free((dv)->array);
	(dv)->array = NULL;

	free(dv);
}

/******************************************************************************/

status DVectorReserve(d_vector_t *dv, size_t requested_capacity)
{
	assert(dv);

	if(requested_capacity > (dv -> capacity))
	{
		if(NULL == (dv-> array = realloc(dv->array,
								(requested_capacity) * (dv->element_size))))
		{
			return(MEMORY_ALLOCATION_FAIL);
		}

		dv -> capacity = requested_capacity;
	}

	return(SUCCESS);
}
/******************************************************************************/

status DVectorPush(d_vector_t *dv, void *element)
{
	assert(dv);

	if(DVectorGetSize(dv) == dv-> capacity)
	{
		void* ptr_realloc;
		dv-> capacity *= 2;

		if(NULL == (ptr_realloc = realloc(dv->array,
										(dv->capacity) * (dv->element_size))))
		{
			dv-> capacity /= 2;
			return(MEMORY_ALLOCATION_FAIL);
		}

		dv -> array = ptr_realloc;
	}

	memcpy((char*)dv->array + (dv->head) * (dv->element_size),
								element, dv-> element_size);
	++(dv->head);

	return(SUCCESS);
}

status DVectorPop(d_vector_t *dv)
{
	assert(dv);
	assert((DVectorGetSize(dv)) > 0);

	--(dv->head);

	if(DVectorGetSize(dv) == (dv->capacity) / 4 && (DVectorGetSize(dv) != 0))
	{
		void* ptr_realloc;

		if(NULL == (ptr_realloc = realloc(dv->array,
										(dv->capacity) * (dv->element_size))))
		{
			++(dv->head);
			return(MEMORY_ALLOCATION_FAIL);
		}

		dv-> capacity /= 2;
		dv -> array = ptr_realloc;
	}

	return(SUCCESS);
}
/******************************************************************************/

void *DVectorGetItem(const d_vector_t *dv , size_t index)
{
	assert(dv);

	if(index >= DVectorGetSize(dv))
	{
		return(NULL);
	}

	return ((char *)dv -> array + (index * (dv -> element_size)));
}
/******************************************************************************/

size_t DVectorGetSize(const d_vector_t *dv)
{
	assert(dv);
	return(dv-> head);
}

size_t DVectorGetElementSize(const d_vector_t *dv)
{
	assert(dv);
	return(dv -> element_size);
}

size_t DVectorGetCapacity(const d_vector_t *dv)
{
	assert(dv);
	return(dv -> capacity);
}
/*******************************************************************************
*******************************************************************************/
