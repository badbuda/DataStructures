/*******************************************************************************
* NAME: Task Scheduler                                                         *
*                                                                              *
* PURPOSE: Manage TaskSchedulers.			                       *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  26.11.18                                                      *
*******************************************************************************/
#include <stdio.h> 	/* sizeof */
#include <time.h> 	/* time_t, time() */
#include <stdlib.h> /* size_t, malloc, free */
#include <assert.h>	/* assert */
#include <unistd.h> /* sleep */

#include "uid.h"
#include "task.h"
#include "task_scheduler.h"
#include "priority_queue.h"

struct task_scheduler_s
{
	p_queue_t *pq;
	bool running_flag;
};

/******************************************************************************/
bool IsHigher(void *current_data, void *new_data)
{
	return (TaskGetStartTime((task_t*)new_data) <=
			TaskGetStartTime((task_t*)current_data));
}
/******************************************************************************/

task_scheduler_t *TaskSchedulerCreate()
{
	task_scheduler_t *scheduler = malloc(sizeof(task_scheduler_t));

	if (NULL == scheduler)
	{
		free(scheduler);
		return (NULL);
	}

	scheduler->pq = PQueueCreate(IsHigher);

	if (NULL == scheduler->pq)
	{
		return (NULL);
	}

	return (scheduler);
}
/******************************************************************************/

void TaskSchedulerDestroy(task_scheduler_t *scheduler)
{
	assert(scheduler);

	while (FALSE == PQueueIsEmpty(scheduler->pq))
	{
		TaskDestroy(PQueueDequeue(scheduler->pq));
	}

	PQueueDestroy(scheduler->pq);
	free(scheduler);
}
/******************************************************************************/

unique_id_t TaskSchedulerAddTask( task_scheduler_t *scheduler,
							      time_t start_time,
							      size_t frequency,
							      task_function_t task_func,
							      destroy_task_function_t destroy_function,
							      void *param)
{
	task_t *task = NULL;

	assert(scheduler);

	task = TaskCreate(start_time, frequency, task_func,
							  destroy_function, param);

	if (NULL == task)
	{
		return (UIDGetBad());
	}

	if (MEMORY_ALLOCATION_FAIL == PQueueEnqueue(scheduler->pq, (void*)task))
	{
		return (UIDGetBad());
	}

	return (TaskGetUID(task));
}
/******************************************************************************/

bool MatchUID(void *task, void *param)
{
	assert(task);
	assert(param);

	return (UIDIsSame(TaskGetUID(task),*(unique_id_t*)param));
}

void TaskSchedulerRemoveTask(task_scheduler_t *scheduler, unique_id_t id)
{
	task_t *task;

	assert(scheduler);

	task = PQueueRemove(scheduler->pq, MatchUID, &id);

	if (NULL != task)
	{
		TaskDestroy(task);
	}
}
/******************************************************************************/

bool TaskSchedulerIsEmpty(task_scheduler_t *scheduler)
{
	assert(scheduler);

	return (PQueueIsEmpty(scheduler->pq));
}
/******************************************************************************/

size_t TaskSchedulerCount(task_scheduler_t *scheduler)
{
	assert(scheduler);

	return (PQueueGetSize(scheduler->pq));
}
/******************************************************************************/

status TaskSchedulerStart(task_scheduler_t *scheduler)
{
	status task_status = SUCCESS;
	task_t *current_task = NULL;

	assert(scheduler);

	scheduler->running_flag = TRUE;

	while(scheduler->running_flag && (FALSE == TaskSchedulerIsEmpty(scheduler)))
	{
		current_task = (task_t*)PQueueDequeue(scheduler->pq);

		if (TaskGetStartTime(current_task) > time(NULL))
		{
			sleep(TaskGetStartTime(current_task) - time(NULL));
		}

		task_status = TaskRun(current_task);

		if (TASK_FAIL == task_status)
		{
			TaskDestroy(current_task);
			return (TASK_FAIL);
		}
		else if (STOP_TASK == task_status)
		{
			TaskDestroy(current_task);
		}
		else if (MEMORY_ALLOCATION_FAIL == PQueueEnqueue(scheduler->pq,
														(void*)current_task))
		{
			TaskDestroy(current_task);
			return (MEMORY_ALLOCATION_FAIL);
		}
	}

	return (SUCCESS);
}
/******************************************************************************/

void TaskSchedulerStop(task_scheduler_t *scheduler)
{
	assert(scheduler);
	scheduler->running_flag = FALSE;
}
/*******************************************************************************
*******************************************************************************/
