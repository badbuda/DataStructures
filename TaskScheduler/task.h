#pragma once

#include <time.h> /* time_t */
#include <stdlib.h> /* size_t */

#include "../include/task_scheduler.h"
#include "enum_rd58.h"
#include "function_pointers.h"
#include "uid.h"

#ifndef __TASK_H__
#define __TASK_H__

/*******************************************************************************
* NAME: Task                                                                   *
*                                                                              *
* PURPOSE:                                                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  22.11.18                                                      *
*******************************************************************************/

typedef struct task_s task_t;

/******************************************************************************/

task_t *TaskCreate(
	time_t start_time,
	size_t frequency,
	task_function_t task_func,
	destroy_task_function_t destroy_function,
	void *param
);
/*******************************************************************************
* Creates task for task scheduler                                              *
*                                                                              *
* Parameters:                                                                  *
*  task_func  - task function to be executed                                   *
*  param      - task function parameter                                        *
*  start_time - time after which task will be run                              *
*  frequency  - time between executions of repeated task                       *
*  destroy    - function to be called by task if task needs to be destroyed    *
*                                                                              *
* Returns:                                                                     *
*  pointer to new task, NULL if failed                                         *
*******************************************************************************/

void TaskDestroy(task_t *task);
/*******************************************************************************
* Frees all memory allocated to task                                           *
*                                                                              *
* Parameters:                                                                  *
*  task - pointer to task                                                      *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*******************************************************************************/

time_t TaskGetStartTime(task_t *task);
/*******************************************************************************
* Returns time of task scheduled execution                                     *
*                                                                              *
* Parameters:                                                                  *
*  task - pointer to task                                                      *
*                                                                              *
* Returns:                                                                     *
*  time_t - time of scheduled execution                                        *
*******************************************************************************/

size_t TaskGetFrequency(task_t *task);
/*******************************************************************************
* Returns frequency of task execution                                          *
*                                                                              *
* Parameters:                                                                  *
*  task - pointer to task                                                      *
*                                                                              *
* Returns:                                                                     *
*  time_t - time of scheduled execution                                        *
*******************************************************************************/

unique_id_t TaskGetUID(task_t *task);
/*******************************************************************************
* Returns unique id of a task                                                  *
*                                                                              *
* Parameters:                                                                  *
*  task - pointer to task                                                      *
*                                                                              *
* Returns:                                                                     *
*  unique_id_t - unique id                                                     *
*******************************************************************************/

status TaskRun(task_t *task);
/*******************************************************************************
* Runs task and updates schedule time if repeats                               *
*                                                                              *
* Parameters:                                                                  *
*  task - pointer to task                                                      *
*                                                                              *
* Returns:                                                                     *
*  SUCCESS - if successful                                                     *
*  ACTION_FAIL - if failed                                                     *
*  STOP_TASK - if user return STOP_TASK                                        *
*******************************************************************************/

#endif
