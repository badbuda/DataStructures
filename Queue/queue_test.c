/*******************************************************************************
* NAME: Queue                                                                  *
*                                                                              *
* PURPOSE: Queue Tests.                                                        *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  15.11.18                                                      *
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "queue.h"

TestResult QueueCreateEnqueueDequeueTest();
TestResult QueueAppendTest();

int main()
{
	RUN_TEST(QueueCreateEnqueueDequeueTest);
	RUN_TEST(QueueAppendTest);

	return 0;
}
/******************************************************************************/

TestResult QueueCreateEnqueueDequeueTest()
{
	int a = 5;
	int b = 4;
	queue_t *q = QueueCreate();
	REQUIRE(QueueEnqueue(q, &a) == SUCCESS);
	REQUIRE(*(int *)QueuePeek(q) == 5);
	REQUIRE(QueueGetSize(q) == 1);
	REQUIRE(QueueEnqueue(q, &b) == SUCCESS);
	REQUIRE(QueueGetSize(q) == 2);
	REQUIRE(QueueIsEmpty(q) == FALSE);
	REQUIRE(*(int *)QueueDequeue(q) == 5);
	REQUIRE(QueueGetSize(q) == 1);
	REQUIRE(*(int *)QueueDequeue(q) == 4);
	REQUIRE(QueueGetSize(q) == 0);
	REQUIRE(QueueIsEmpty(q) == TRUE);
	REQUIRE(QueueEnqueue(q, &a) == SUCCESS);
	REQUIRE(QueueGetSize(q) == 1);

	QueueDestroy(&q);

	return (TEST_PASS);

}

TestResult QueueAppendTest()
{
	int a = 5;
	int b = 4;
	int c = 10;
	int d = 11;
	int f = 12;
	queue_t *q = QueueCreate();
	queue_t *q2 = QueueCreate();
	REQUIRE(QueueEnqueue(q, &a) == SUCCESS);
	REQUIRE(QueueEnqueue(q, &b) == SUCCESS);
	REQUIRE(QueueEnqueue(q2, &c) == SUCCESS);
	REQUIRE(QueueEnqueue(q2, &d) == SUCCESS);
	REQUIRE(QueueEnqueue(q2, &f) == SUCCESS);
	REQUIRE(*(int *)QueuePeek(q2) == 10);
	QueueAppend(q, q2);
	free(q2);
	q2 = NULL;
	REQUIRE(QueueGetSize(q) == 5);
	REQUIRE(*(int *)QueueDequeue(q) == 5);
	REQUIRE(*(int *)QueueDequeue(q) == 4);
	REQUIRE(*(int *)QueueDequeue(q) == 10);
	REQUIRE(*(int *)QueueDequeue(q) == 11);
	REQUIRE(*(int *)QueueDequeue(q) == 12);
	REQUIRE(QueueIsEmpty(q) == TRUE);

	QueueDestroy(&q);
	return (TEST_PASS);
}
