/*******************************************************************************
* NAME: Circular Buffer                                                        *
*                                                                              *
* PURPOSE: Circular Buffer                                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  10.11.18                                                      *
*******************************************************************************/

#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, free */
#include <string.h> /*  memcpy */
#include <assert.h>	/* assert */
#include <unistd.h> /* ssize_t */
#include <errno.h>  /* errno */
#include "c_buffer.h"

struct c_buffer_s
{
	size_t capacity;
	size_t size;
	size_t write_head;
	size_t read_head;
	char* buffer;
}; 

/******************************************************************************/

c_buffer_t *CBufferCreate(size_t capacity)
{
	c_buffer_t *c_buffer_var = malloc (sizeof(c_buffer_t) + capacity + 1);
	
	if(NULL == c_buffer_var)
	{
		return(NULL);
	}
	
	c_buffer_var -> capacity = capacity;
	c_buffer_var -> size = 0;
	c_buffer_var -> write_head = 0;
	c_buffer_var -> read_head  = 0;
	c_buffer_var -> buffer = (char*)(&c_buffer_var->buffer) + 
							 sizeof(c_buffer_var -> buffer);

	return (c_buffer_var);
}

void CBufferDestroy(c_buffer_t **cb)
{
	assert(*cb);
	
	free(*cb);
	*cb = NULL;
}

void CBufferClear(c_buffer_t *cb)
{
	assert(cb);
	
	cb -> size 		 = 0;
	cb -> write_head = 0;
	cb -> read_head  = 0;
}
/******************************************************************************/

ssize_t CBufferWrite(c_buffer_t *cb, const void *src, size_t nbyte)
{
	size_t cb_size = CBufferGetSize(cb);
	size_t delta_of_cap_and_write_head = cb->capacity + 1 - cb->write_head;
	
	assert(cb);
	assert(src);
	
	if(nbyte >= (cb->capacity) - cb_size)
	{
		if((cb->capacity) == cb_size)
		{
			errno = ENOBUFS;
			return(-1);
		}
		
		nbyte = (cb->capacity) - cb_size;
	}
	
	if(cb->read_head > cb->write_head || nbyte <= delta_of_cap_and_write_head)
	{
		memcpy(cb->buffer + cb->write_head, src, nbyte);
		cb->write_head += nbyte;
		
		if(cb->capacity + 1 == cb->write_head)
		{
			cb->write_head = 0;
		}
		
	}
	else
	{
		memcpy(cb->buffer + cb->write_head, src, delta_of_cap_and_write_head);
		src = (char*)src + delta_of_cap_and_write_head;
		memcpy(cb->buffer, src, (nbyte - delta_of_cap_and_write_head));
		
		cb->write_head = (nbyte - delta_of_cap_and_write_head);
	}

	cb->size += nbyte;
	return(nbyte);
}
/******************************************************************************/

ssize_t CBufferRead(c_buffer_t *cb, void *dest, size_t nbyte)
{
	size_t cb_size = CBufferGetSize(cb);
	size_t delta_of_cap_and_read_head = cb->capacity + 1 - cb->read_head;
	
	assert(cb);
	assert(dest);
	
	if(nbyte >= cb_size)
	{
		if(0 == cb_size)
		{
			errno = ENODATA;
			return(-1);
		}
		
		nbyte = cb_size;
	}
	
	if(nbyte < delta_of_cap_and_read_head)
	{
		memcpy(dest, cb->buffer + cb->read_head, nbyte);
    	cb->read_head += nbyte;
    	
		if(cb->capacity + 1 == cb->read_head)
		{
			cb->read_head = 0;
		}
	}
	else
	{
		memcpy(dest, cb->buffer + cb->read_head, delta_of_cap_and_read_head);
		dest = (char*)dest + delta_of_cap_and_read_head;
		memcpy(dest, cb->buffer, (nbyte - delta_of_cap_and_read_head));
		
    	cb->read_head = (nbyte - delta_of_cap_and_read_head);
	}
	
	cb->size -= nbyte;
	return(nbyte);
}

/******************************************************************************/

size_t CBufferGetSize(const c_buffer_t *cb)
{
	assert(cb);
	return(cb->size);
}

size_t CBufferGetCapacity(const c_buffer_t *cb)
{
	assert(cb);
	return(cb -> capacity);
}
/******************************************************************************/
/******************************************************************************/
