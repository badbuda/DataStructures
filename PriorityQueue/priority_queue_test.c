/*******************************************************************************
* NAME: Priority Queue List Test file                                          *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  21.11.18                                                      *
*******************************************************************************/
#include <stdio.h>  /* size_t */
#include "test.h"
#include "sorted_list.h"
#include "priority_queue.h"

#define ARRAY_SIZE 100

p_queue_t *UtilityCreateSL(int n, int *array);
bool is_higher(void *current_data, void *new_data);
TestResult PQueueCreateDestroyTest();
TestResult PQueueEnqueueTest();
TestResult PQueueDequeueTest();
TestResult PQueuePeekTest();
TestResult PQueueGetSizeTest();
TestResult PQueueIsEmptyTest();
bool FindData(void *data, void *param);
TestResult PQueueRemoveTest();

static int array[ARRAY_SIZE];

int main()
{
	RUN_TEST(PQueueCreateDestroyTest);
	RUN_TEST(PQueueEnqueueTest);
	RUN_TEST(PQueueDequeueTest);
	RUN_TEST(PQueuePeekTest);
	RUN_TEST(PQueueGetSizeTest);
	RUN_TEST(PQueueIsEmptyTest);
	RUN_TEST(PQueueRemoveTest);

	return (0);
}
/******************************************************************************/

p_queue_t *UtilityCreateSL(int n, int *array)
{
	int i = 0;
	p_queue_t *pq = PQueueCreate(is_higher);

	for ( ; i<ARRAY_SIZE ; ++i)
	{
		array[i] = i*10;
	}

	for (i = 0 ; i<n ; ++i)
	{
		PQueueEnqueue(pq, &array[i]);
	}

	return (pq);
}
/******************************************************************************/

bool is_higher(void *current_data, void *new_data)
{
	return (*(int*)new_data <= *(int*)current_data);
}
/******************************************************************************/

TestResult PQueueCreateDestroyTest()
{
	p_queue_t *pq = PQueueCreate(is_higher);
	PQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueueEnqueueTest()
{
	int data1 = 20;
	int data2 = 30;

	p_queue_t *pq = PQueueCreate(is_higher);
	REQUIRE(SUCCESS == PQueueEnqueue(pq, (void *)&data1));
	REQUIRE(SUCCESS == PQueueEnqueue(pq, (void *)&data2));

	PQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueueDequeueTest()
{
	void* return_data;
	p_queue_t *pq = UtilityCreateSL(2, array);

	return_data = PQueueDequeue(pq);
	REQUIRE(array[0] == *(int *)return_data);
	return_data = PQueueDequeue(pq);
	REQUIRE(array[1] == *(int *)return_data);

	PQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueueGetSizeTest()
{
	p_queue_t  *pq = PQueueCreate(is_higher);
	REQUIRE(0 == PQueueGetSize(pq));
	PQueueDestroy(pq);

	pq = UtilityCreateSL(1, array);
	REQUIRE(1 == PQueueGetSize(pq));

	PQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueuePeekTest()
{
	void* return_data;
	p_queue_t  *pq = UtilityCreateSL(3, array);

	return_data = PQueuePeek(pq);
	REQUIRE(array[0] == *(int *)return_data);

	PQueueDestroy(pq);
	return (TEST_PASS);

}
/******************************************************************************/

TestResult PQueueIsEmptyTest()
{
	p_queue_t  *pq = PQueueCreate(is_higher);
	REQUIRE(TRUE == PQueueIsEmpty(pq));
	PQueueDestroy(pq);

	pq = UtilityCreateSL(1, array);
	REQUIRE(FALSE == PQueueIsEmpty(pq));

	PQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/
bool FindData(void *data, void *param)
{
	return (*(int *)data == *(int *)param);
}

TestResult PQueueRemoveTest()
{
	int j = ARRAY_SIZE - 1, param = 40;
	p_queue_t *pq = PQueueCreate(is_higher);

	for ( ; j >= 0 ; --j)
	{
		PQueueEnqueue(pq, &array[j]);
	}

	PQueueRemove(pq, FindData, &param);
	REQUIRE((ARRAY_SIZE - 1) == (int)PQueueGetSize(pq));

	PQueueDequeue(pq);
	PQueueDequeue(pq);
	REQUIRE(array[2] == *(int *)PQueuePeek(pq));

	PQueueDestroy(pq);
	pq = NULL;

	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
