/*******************************************************************************
* NAME: Task C file.                                                           *
*                                                                              *
* PURPOSE: Create Tasks for the TaskSchedualer.                                *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  26.11.18                                                      *
*******************************************************************************/
#include <stdio.h> 	/* sizeof */
#include <time.h>   /* time_t */
#include <stdlib.h> /* size_t, malloc, free */
#include <assert.h>	/* assert */

#include "../include/uid.h"
#include "../include/task.h"

struct task_s
{
	unique_id_t uid;
	time_t start_time;
	size_t frequency;
	task_function_t task_func;
	destroy_task_function_t destroy_function;
	void *param;	
} ;

/******************************************************************************/

task_t *TaskCreate(	time_t start_time,
					size_t frequency,
					task_function_t task_func,
					destroy_task_function_t destroy_function,
					void *param)
{
	task_t *task = malloc(sizeof(task_t));
	
	if (NULL == task)
	{
		free(task);
		return (NULL);
	}
	
	task->start_time = start_time;
	task->frequency = frequency;
	task->task_func = task_func;
	task->destroy_function = destroy_function;
	task->param = param;
	task->uid = UIDCreate();
	
	if (UIDIsBad(task->uid))
	{
		return (NULL);
	}
	
	return (task);
}
/******************************************************************************/

void TaskDestroy(task_t *task)
{
	assert(task);
	task->destroy_function(task->param);
	free(task);
}
/******************************************************************************/

time_t TaskGetStartTime(task_t *task)
{
	assert(task);
	return (task->start_time);
}
/******************************************************************************/

size_t TaskGetFrequency(task_t *task)
{
	assert(task);
	return (task->frequency);
}
/******************************************************************************/

unique_id_t TaskGetUID(task_t *task)
{
	assert(task);
	return (task->uid);
}
/******************************************************************************/

status TaskRun(task_t *task)
{
	status task_status = SUCCESS;

	assert(task);
	
	task_status = task->task_func(task->param);
	
	if (ACTION_FAIL == task_status)
	{
		return (TASK_FAIL);
	}
	else if (STOP_TASK == task_status || task->frequency <= 0)
	{
		return (STOP_TASK);
	}
	else
	{
		task->start_time += task->frequency;
		return (SUCCESS);
	}
}
/*******************************************************************************
*******************************************************************************/
