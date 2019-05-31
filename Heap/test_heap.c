/*******************************************************************************
* NAME: Heap Tests                                                             *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  30.12.18                                                      *
*******************************************************************************/

#include <stdlib.h> /* size_t, qsort */
#include <string.h>	/* memcpy */
#include <time.h> /* time */

#include "test.h"
#include "heap.h"
#include "heap_pq.h"

#define INT_ARRAY_SIZE 50
#define INT_ARRAY_RANGE 100
#define INT_ARRAY_SIZE_FOR_REMOVE 8
#define EMPTY 0
#define EQUAL 0
#define NOT_IN_HEAP (INT_ARRAY_RANGE + 1)

#define PQ_ARRAY_SIZE 100
/******************************************************************************/

TestResult BasicTestHeapCreateDestroy();
TestResult BasicTestHeapCreateDestroyForInts();
TestResult BasicTestHeapCount();
TestResult BasicTestHeapIsEmpty();
TestResult TestHeapPush();
TestResult TestHeapPeek();
TestResult TestHeapPop();
TestResult TestHeapRemoveForInts();

TestResult PQueueCreateDestroyTest();
TestResult PQueueEnqueueTest();
TestResult PQueueDequeueTest();
TestResult PQueuePeekTest();
TestResult PQueueGetSizeTest();
TestResult PQueueIsEmptyTest();
TestResult PQueueRemoveTest();
/******************************************************************************/

static int ints_test_array[INT_ARRAY_SIZE];
static int ints_sorted_array[INT_ARRAY_SIZE];
static int ints_original_array[INT_ARRAY_SIZE];

int main()
{
	RUN_TEST(BasicTestHeapCreateDestroy);
	RUN_TEST(BasicTestHeapCount);
	RUN_TEST(BasicTestHeapIsEmpty);
	RUN_TEST(TestHeapPush);
	RUN_TEST(TestHeapPeek);
	RUN_TEST(TestHeapPop);
	RUN_TEST(TestHeapRemoveForInts);

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

static int CompareIntFunction(const void* data, const void* param)
{
	return (*(int*)param - *(int*)data);
}

static bool MatchInts(void *data, void *param)
{
	return (*(int *)data == *(int *)param);
}
/***********************INITIALIZATION*****************************************/

static void InitBeforeTest()
{
	size_t i = 0;
	int number = 0;

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		number = (rand() * time(NULL)) % INT_ARRAY_RANGE;

		ints_test_array[i] = number;
	}

	memcpy(ints_original_array, ints_test_array, sizeof(int) * INT_ARRAY_SIZE);
	memcpy(ints_sorted_array, ints_test_array, sizeof(int) * INT_ARRAY_SIZE);
	qsort(ints_sorted_array, INT_ARRAY_SIZE, sizeof(int), CompareIntFunction);
}

static void FillIntsHeap(heap_t *heap)
{
	size_t i = 0;

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		HeapPush(heap, &ints_test_array[i]);
	}
}
/**************************TESTS***********************************************/

TestResult BasicTestHeapCreateDestroy()
{
	heap_t *heap = HeapCreate(CompareIntFunction);
	HeapDestroy(heap);
	return (TEST_PASS);
}

TestResult BasicTestHeapCount()
{
	heap_t *heap = HeapCreate(CompareIntFunction);

	REQUIRE(EMPTY == HeapCount(heap));

	HeapDestroy(heap);
	return (TEST_PASS);
}

TestResult BasicTestHeapIsEmpty()
{
	heap_t *heap = HeapCreate(CompareIntFunction);

	REQUIRE(TRUE == HeapIsEmpty(heap));

	HeapDestroy(heap);
	return (TEST_PASS);
}

TestResult TestHeapPush()
{
	heap_t *heap = HeapCreate(CompareIntFunction);
	InitBeforeTest();

	HeapPush(heap, &ints_test_array[0]);
	REQUIRE(FALSE == HeapIsEmpty(heap));
	REQUIRE(1 == HeapCount(heap));

	HeapDestroy(heap);

	heap = HeapCreate(CompareIntFunction);
	FillIntsHeap(heap);

	REQUIRE(FALSE == HeapIsEmpty(heap));
	REQUIRE(INT_ARRAY_SIZE == HeapCount(heap));

	HeapDestroy(heap);
	return (TEST_PASS);
}

TestResult TestHeapPeek()
{
	int test_int_small = 1;
	int test_int_big = 10;
	heap_t *heap = HeapCreate(CompareIntFunction);

	HeapPush(heap, &test_int_small);
	REQUIRE(test_int_small = *(int *)HeapPeek(heap));
	HeapPush(heap, &test_int_big);
	REQUIRE(test_int_big = *(int *)HeapPeek(heap));

	HeapDestroy(heap);

	heap = HeapCreate(CompareIntFunction);

	HeapPush(heap, &test_int_big);
	REQUIRE(test_int_big = *(int *)HeapPeek(heap));
	HeapPush(heap, &test_int_small);
	REQUIRE(test_int_big = *(int *)HeapPeek(heap));

	HeapDestroy(heap);
	return (TEST_PASS);
}

TestResult TestHeapPop()
{
	size_t i = 0;

	heap_t *heap = HeapCreate(CompareIntFunction);
	InitBeforeTest();

	HeapPush(heap, &ints_test_array[0]);
	HeapPush(heap, &ints_test_array[1]);

	REQUIRE(2 == HeapCount(heap));
	HeapPop(heap);
	REQUIRE(1 == HeapCount(heap));
	HeapPop(heap);
	REQUIRE(EMPTY == HeapCount(heap));

	HeapDestroy(heap);

	heap = HeapCreate(CompareIntFunction);
	FillIntsHeap(heap);

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		REQUIRE(ints_sorted_array[i] == *(int *)HeapPeek(heap));
		HeapPop(heap);
	}

	REQUIRE(EMPTY == HeapCount(heap));

	HeapDestroy(heap);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestHeapRemoveForInts()
{
	size_t i = 0;
	int *removed_data = NULL;
	int data = 0;
	int not_in_heap = NOT_IN_HEAP;
	int data_array_test[INT_ARRAY_SIZE_FOR_REMOVE] = {8, 1, 5, 9, 3, 3, 2, 8};
	int data_array_original[INT_ARRAY_SIZE_FOR_REMOVE] = {8, 1, 5, 9, 3, 3, 2, 8};

	heap_t *heap = HeapCreate(CompareIntFunction);

	for ( ; i < INT_ARRAY_SIZE_FOR_REMOVE; ++i)
	{
		HeapPush(heap, &data_array_test[i]);
	}

	for (i = 0 ; i < INT_ARRAY_SIZE_FOR_REMOVE; ++i)
	{
		data = data_array_original[i];
		removed_data = HeapRemove(heap, MatchInts, &data_array_original[i]);
		REQUIRE(data == *removed_data);
	}

	HeapDestroy(heap);

	heap = HeapCreate(CompareIntFunction);
	InitBeforeTest();
	FillIntsHeap(heap);

	for (i = 0 ; i < INT_ARRAY_SIZE; ++i)
	{
		data = ints_original_array[i];
		removed_data = HeapRemove(heap, MatchInts, &ints_original_array[i]);
		REQUIRE(data == *removed_data);
	}

	removed_data = HeapRemove(heap, MatchInts, &not_in_heap);
	REQUIRE(NULL == removed_data);


	HeapDestroy(heap);
	return (TEST_PASS);
}
/***********************Heap PQ************************************************/

static int pq_array[PQ_ARRAY_SIZE];

static int is_higher(const void *current_data, const void *new_data)
{
	return (*(int*)current_data - *(int*)new_data);
}

static bool FindData(void *data, void *param)
{
	return (*(int *)data == *(int *)param);
}

static heap_p_queue_t *UtilityCreateSL(int n, int *array)
{
	int i = 0;
	heap_p_queue_t *pq = HeapPQueueCreate(is_higher);

	for ( ; i < PQ_ARRAY_SIZE ; ++i)
	{
		array[i] = i * 10;
	}

	for (i = 0 ; i < n ; ++i)
	{
		HeapPQueueEnqueue(pq, &array[i]);
	}

	return (pq);
}
/******************************************************************************/

TestResult PQueueCreateDestroyTest()
{
	heap_p_queue_t *pq = HeapPQueueCreate(is_higher);
	HeapPQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueueEnqueueTest()
{
	int data1 = 20;
	int data2 = 30;

	heap_p_queue_t *pq = HeapPQueueCreate(is_higher);
	REQUIRE(SUCCESS == HeapPQueueEnqueue(pq, (void *)&data1));
	REQUIRE(SUCCESS == HeapPQueueEnqueue(pq, (void *)&data2));

	HeapPQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueueDequeueTest()
{
	void* return_data;
	heap_p_queue_t *pq = UtilityCreateSL(2, pq_array);

	return_data = HeapPQueueDequeue(pq);
	REQUIRE(pq_array[0] == *(int *)return_data);
	return_data = HeapPQueueDequeue(pq);
	REQUIRE(pq_array[1] == *(int *)return_data);

	HeapPQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueueGetSizeTest()
{
	heap_p_queue_t  *pq = HeapPQueueCreate(is_higher);
	REQUIRE(0 == HeapPQueueGetSize(pq));
	HeapPQueueDestroy(pq);

	pq = UtilityCreateSL(1, pq_array);
	REQUIRE(1 == HeapPQueueGetSize(pq));

	HeapPQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueuePeekTest()
{
	void* return_data;
	heap_p_queue_t  *pq = UtilityCreateSL(3, pq_array);

	return_data = HeapPQueuePeek(pq);
	REQUIRE(pq_array[0] == *(int *)return_data);

	HeapPQueueDestroy(pq);
	return (TEST_PASS);

}
/******************************************************************************/

TestResult PQueueIsEmptyTest()
{
	heap_p_queue_t  *pq = HeapPQueueCreate(is_higher);
	REQUIRE(TRUE == HeapPQueueIsEmpty(pq));
	HeapPQueueDestroy(pq);

	pq = UtilityCreateSL(1, pq_array);
	REQUIRE(FALSE == HeapPQueueIsEmpty(pq));

	HeapPQueueDestroy(pq);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult PQueueRemoveTest()
{
	int j = PQ_ARRAY_SIZE - 1, param = 40;
	heap_p_queue_t *pq = HeapPQueueCreate(is_higher);

	for ( ; j >= 0 ; --j)
	{
		HeapPQueueEnqueue(pq, &pq_array[j]);
	}

	HeapPQueueRemove(pq, FindData, &param);
	REQUIRE((PQ_ARRAY_SIZE - 1) == (int)HeapPQueueGetSize(pq));

	HeapPQueueDequeue(pq);
	HeapPQueueDequeue(pq);
	REQUIRE(pq_array[2] == *(int *)HeapPQueuePeek(pq));

	HeapPQueueDestroy(pq);
	pq = NULL;

	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/


























/******************************************************************************/
/******************************************************************************/
