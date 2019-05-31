/*******************************************************************************
* NAME: Circular Buffer                                                        *
*                                                                              *
* PURPOSE: Circular Buffer                                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  10.11.18                                                      *
*******************************************************************************/

/************************************API**************************************** 
* user can read and write up to (capacity) bytes.			       *
* user can read up to the current size of the buffer. 			       *
* if return value is smaller than the number of bytes requested this is not an *
* error, this may happen for example because fewer bytes are actually 	       *
* available right now.						               *
* user can write up to the current available memory of the buffer.	       *
* if return value is less than the number of bytes requested this is not an    *
* error. this may happen for example because there is insufficient space on    * 
* the underlying physical medium. 					       *
*******************************************************************************/

#include <stdio.h> 	/* size_t */
#include <unistd.h>	/* ssize_t */ 

#ifndef _DS_CBuffer_H__
#define _DS_CBuffer_H__

typedef struct c_buffer_s c_buffer_t;

/******************************************************************************/

c_buffer_t *CBufferCreate(size_t capacity);
/*******************************************************************************
* Creates dynamic buffer and returns pointer to memory address of new buffer   *
*                                                                              *
* Parameters:                                                                  *
*  capacity - number of elements in buffer                                     *
*                                                                              *
* Returns:                                                                     *
*  pointer to memory address of new buffer                                     *
*******************************************************************************/

void CBufferDestroy(c_buffer_t **cb);
/*******************************************************************************
* Frees dynamic buffer memory                                                  *
*                                                                              *
* Parameters:                                                                  *
*  cb - pointer to memory address of buffer, not NULL                          *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*******************************************************************************/

void CBufferClear(c_buffer_t *cb);
/*******************************************************************************
* Clears the buffer                                                            *
*                                                                              *
* Parameters:                                                                  *
*  cb - pointer to memory address of buffer, not NULL                          *
*                                                                              *
*******************************************************************************/

ssize_t CBufferWrite(c_buffer_t *cb, const void *src, size_t nbyte);
/*******************************************************************************
* Write element to top of dynamic buffer.                                      *
*                                                                              *
* Parameters:                                                                  *
*  cb - pointer to memory address of buffer, not NULL                          *
*  source - this is the source to read from  , not NULL                        *
*   nbyte − This is the number of bytes to read                                *
*                                                                              *
* Returns:                                                                     *
*  number of bytes written to buffer in size_t                                 *
*  if attemp to write to full buffer, returns -1 and sets errno ENOBUFS 105     *
*                                                                              *
*******************************************************************************/

ssize_t CBufferRead(c_buffer_t *cb, void *dest, size_t nbyte);
/*******************************************************************************
* Reads element from top of dynamic buffer.                                    *
*                                                                              *
* Parameters:                                                                  *
*  cb - pointer to memory address of buffer, not NULL                          *
*  dest - this is the destination to write to,  not NULL                       *
*   nbytr − This is the number of bytes to read                                *
*                                                                              *
* Returns:                                                                     *
*  how many bytes was read in size_t                                           *
*  if attemp to read from empty buffer, returns -1 and sets errno ENODATA 61   *
*                                                                              *
*******************************************************************************/

size_t CBufferGetSize(const c_buffer_t *cb);
/*******************************************************************************
* Returns current size of dynamic buffer as size_t (bytes)                     *
*                                                                              *
* Parameters:                                                                  *
*  cb - pointer to memory address of buffer, not NULL                          *
*                                                                              *
* Returns:                                                                     *
*  current size of unread info in the buffer as size_t                         *
*******************************************************************************/

size_t CBufferGetCapacity(const c_buffer_t *cb);
/*******************************************************************************
* Returns dynamic buffer capacity as size_t                                    *
*                                                                              *
* Parameters:                                                                  *
*  cb - pointer to memory address of buffer, not NULL                          *
*                                                                              *
* Returns:                                                                     *
*  current buffer capacity as size_t                                           *
*******************************************************************************/

#endif
/******************************************************************************/
/******************************************************************************/
