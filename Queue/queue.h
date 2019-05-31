#pragma once

#include <stdio.h>
/*#include "../include/enum_rd58.h"*/
#include "enum_rd58.h"
/*******************************************************************************
* NAME: Queue                                                                  *
*                                                                              *
* PURPOSE:                                                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  14.11.18                                                      *
*******************************************************************************/

typedef struct queue_s queue_t;

/******************************************************************************/

queue_t *QueueCreate();
/*******************************************************************************
* Creates queue structure                                                      *
*                                                                              *
* Parameters:                                                                  *
*  none                                                                        *
*                                                                              *
* Returns:                                                                     *
*  pointer to memory address of queue                                          *
*******************************************************************************/

void QueueDestroy(queue_t **q);
/*******************************************************************************
* Frees all memory allocated to queue                                          *
*                                                                              *
* Parameters:                                                                  *
*  q - pointer to memory address of queue, Not NULL                            *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*******************************************************************************/

status QueueEnqueue(queue_t *q, void* data);
/*******************************************************************************
* Inserts data to queue                                                        *
*                                                                              *
* Parameters:                                                                  *
*  q - pointer to memory address of queue, Not NULL                            *
*  data- pointer to memory address containing data                             *
*                                                                              *
* Returns:                                                                     *
*  SUCCESS - if successful                                                     *
*  MEMORY_ALLOCATION_FAIL - if malloc failed                                   *
*******************************************************************************/

void *QueueDequeue(queue_t *q);
/*******************************************************************************
* Removes oldest entry from queue                                              *
*                                                                              *
* Parameters:                                                                  *
*  q - pointer to memory address of queue, Not NULL                            *
*                                                                              *
* Returns:                                                                     *
*  pointer to removed data                                                     *
*******************************************************************************/

void *QueuePeek(queue_t *q);
/*******************************************************************************
* Returns oldest entry data from queue                                         *
*                                                                              *
* Parameters:                                                                  *
*  q - pointer to memory address of queue, Not NULL                            *
*                                                                              *
* Returns:                                                                     *
*  pointer to data                                                             *
*******************************************************************************/

size_t QueueGetSize(const queue_t *q);
/*******************************************************************************
* Returns current size of queue                                                *
*                                                                              *
* Parameters:                                                                  *
*  q - pointer to memory address of queue, Not NULL                            *
*                                                                              *
* Returns:                                                                     *
*  current size of queue                                                       *
*******************************************************************************/

bool QueueIsEmpty(const queue_t *q);
/*******************************************************************************
* Determines if queue is empty                                                 *
*                                                                              *
* Parameters:                                                                  *
*  q - pointer to memory address of queue, Not NULL                            *
*                                                                              *
* Returns:                                                                     *
*  TRUE - if queue is empty                                                    *
*  FALSE - if queue is not empty                                               *
*******************************************************************************/

void QueueAppend(queue_t *dest, queue_t *src);
/*******************************************************************************
* Appends src to end of dest                                                   *
*                                                                              *
* Parameters:                                                                  *
*  dest - pointer to dest, not NULL                                            *
*  src - pointer to src, not NULL                                              *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*                                                                              *
* Warning: src will be undefined after append                                  *
*******************************************************************************/
