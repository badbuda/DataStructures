/*******************************************************************************
* NAME: Sorted Linked List Test file                                           *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  21.11.18                                                      *
*******************************************************************************/
#include <stdio.h>  /* size_t */
#include "test.h"
#include "sorted_list.h"
#include "d_linked_list.h"

#define ARRAY_SIZE 100

sorted_list_t *UtilityCreateSL(int n, int *array);
bool is_before(void *current_data, void *new_data);
TestResult SortedListCreateDestroyTest();
TestResult SortedListInsertTest();
TestResult SortedListGetDataTest();
TestResult SortedListGetSizeTest();
TestResult SortedListIsEmptyTest();
TestResult SortedListBeginEndTest();
TestResult SortedListEraseTest();
TestResult SortedListPopTest();
TestResult SortedListFindTest();
TestResult SortedListForEachTest();
TestResult SortedListGetNextGetPreviousTest();
TestResult SortedListIsSameIteratorTest();
TestResult SortedListMergeTest();

int array[ARRAY_SIZE];

int main()
{
	RUN_TEST(SortedListCreateDestroyTest);
	RUN_TEST(SortedListInsertTest);
	RUN_TEST(SortedListGetDataTest);
	RUN_TEST(SortedListGetSizeTest);
	RUN_TEST(SortedListIsEmptyTest);
	RUN_TEST(SortedListBeginEndTest);
	RUN_TEST(SortedListEraseTest);
	RUN_TEST(SortedListPopTest);
	RUN_TEST(SortedListFindTest);
	RUN_TEST(SortedListForEachTest);
	RUN_TEST(SortedListGetNextGetPreviousTest);
	RUN_TEST(SortedListIsSameIteratorTest);
	RUN_TEST(SortedListMergeTest);

	return (0);
}
/******************************************************************************/

sorted_list_t *UtilityCreateSL(int n, int *array)
{
	int i = 0;
	sorted_list_t *sl = SortedListCreate(is_before);

	for ( ; i<ARRAY_SIZE ; ++i)
	{
		array[i] = i*10;
	}

	for (i = 0 ; i<n ; ++i)
	{
		SortedListInsert(sl, &array[i]);
	}

	return (sl);
}
/******************************************************************************/

bool is_before(void *current_data, void *new_data)
{
	if (*(int*)new_data <= *(int*)current_data)
	{
		return (TRUE);
	}

	return (FALSE);
}
/******************************************************************************/

TestResult SortedListCreateDestroyTest()
{
	sorted_list_t *sl = SortedListCreate(is_before);
	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListInsertTest()
{
	int data1 = 20;
	int data2 = 30;
	sorted_iterator_t sl_iter;
	sorted_list_t *sl = SortedListCreate(is_before);
	sl_iter = SortedListInsert(sl, (void *)&data1);
	REQUIRE(NULL != sl_iter.info);
	sl_iter = SortedListInsert(sl, (void *)&data2);
	REQUIRE(NULL != sl_iter.info);

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListGetDataTest()
{
	int data1 = 20;
	sorted_iterator_t sl_iter;
	sorted_list_t *sl = SortedListCreate(is_before);
	sl_iter = SortedListInsert(sl, (void *)&data1);

	REQUIRE(20 == *(int*)SortedListGetData(sl_iter));


	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListGetSizeTest()
{
	sorted_list_t *sl = SortedListCreate(is_before);
	REQUIRE(0 == SortedListGetSize(sl));
	SortedListDestroy(sl);

	sl = UtilityCreateSL(1, array);
	REQUIRE(1 == SortedListGetSize(sl));

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListIsEmptyTest()
{
	sorted_list_t *sl = SortedListCreate(is_before);
	REQUIRE(TRUE == SortedListIsEmpty(sl));
	SortedListDestroy(sl);

	sl = UtilityCreateSL(1, array);
	REQUIRE(FALSE == SortedListIsEmpty(sl));

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListBeginEndTest()
{
	sorted_iterator_t sl_iter;
	sorted_list_t *sl = UtilityCreateSL(4, array);

	sl_iter = SortedListBegin(sl);
	REQUIRE(0 == *(int*)SortedListGetData(sl_iter));
	sl_iter = SortedListEnd(sl);
	REQUIRE(30 == *(int*)SortedListGetData(SortedListGetPrevious(sl_iter)));

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListEraseTest()
{
	int data1 = 20;
	int data2 = 30;
	sorted_iterator_t sl_iter1;
	sorted_iterator_t sl_iter2;
	sorted_list_t *sl = SortedListCreate(is_before);
	sl_iter1 = SortedListInsert(sl, (void *)&data1);
	sl_iter2 = SortedListInsert(sl, (void *)&data2);

	SortedListErase(sl_iter1);
	REQUIRE(1 == SortedListGetSize(sl));
	SortedListErase(sl_iter2);
	REQUIRE(0 == SortedListGetSize(sl));

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListPopTest()
{
	sorted_iterator_t sl_iter;
	sorted_list_t *sl = UtilityCreateSL(3, array);

	SortedListPopFront(sl);
	SortedListPopBack(sl);

	REQUIRE(1 == SortedListGetSize(sl));
	sl_iter = SortedListBegin(sl);
	REQUIRE(10 == *(int*)SortedListGetData(sl_iter));
	sl_iter = SortedListEnd(sl);
	REQUIRE(10 == *(int*)SortedListGetData(SortedListGetPrevious(sl_iter)));

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

bool MatchFunctionTest(void* data, void* param)
{
	if (*(int*)data == *(int*)param)
	{
		return (TRUE);
	}

	return (FALSE);
}

TestResult SortedListFindTest()
{
	sorted_list_t *sl = UtilityCreateSL(3, array);
	int param = 20;

	REQUIRE(NULL != SortedListFind(sl, MatchFunctionTest, &param).info);

	++param;
	REQUIRE(NULL == SortedListFind(sl, MatchFunctionTest, &param).info);

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

status ActionFunctionTest(void *data, void *param)
{
	int previos = *(int*)data;

	*(int*)data += *(int*)param;

	if(*(int*)data == (previos + *(int*)param))
	{
		return (SUCCESS);
	}

	return (ACTION_FAIL);
}

TestResult SortedListForEachTest()
{
	sorted_list_t *sl = UtilityCreateSL(3, array);
	int param = 1;
	int result1 = 1;
	int result2 = 11;
	int result3 = 21;

	REQUIRE(SUCCESS == SortedListForEach(sl, ActionFunctionTest, &param));

	REQUIRE(NULL != SortedListFind(sl, MatchFunctionTest, &result1).info);
	REQUIRE(NULL != SortedListFind(sl, MatchFunctionTest, &result2).info);
	REQUIRE(NULL != SortedListFind(sl, MatchFunctionTest, &result3).info);

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListGetNextGetPreviousTest()
{
	sorted_iterator_t sl_iter1;
	sorted_iterator_t sl_iter2;
	sorted_list_t *sl = UtilityCreateSL(2, array);

	sl_iter1 = SortedListBegin(sl);
	sl_iter2 = SortedListGetPrevious(SortedListEnd(sl));

	REQUIRE(sl_iter2.info == SortedListGetNext(sl_iter1).info);
	REQUIRE(sl_iter1.info == SortedListGetPrevious(sl_iter2).info);

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListIsSameIteratorTest()
{
	sorted_iterator_t sl_iter1;
	sorted_iterator_t sl_iter2;
	sorted_list_t *sl = UtilityCreateSL(2, array);

	sl_iter1 = SortedListBegin(sl);
	sl_iter2 = SortedListBegin(sl);
	REQUIRE(TRUE == SortedListIsSameIterator(sl_iter1, sl_iter2));

	sl_iter2 = SortedListGetPrevious(SortedListEnd(sl));
	REQUIRE(FALSE == SortedListIsSameIterator(sl_iter1, sl_iter2));

	SortedListDestroy(sl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SortedListMergeTest()
{
	sorted_list_t *sl1 = UtilityCreateSL(10, array);
	sorted_list_t *sl2 = UtilityCreateSL(2, array);

	SortedListMerge(sl1, sl2);
	REQUIRE(0 == *(int *)SortedListGetData(
								   SortedListGetNext(SortedListBegin(sl1))));
	REQUIRE(12 == SortedListGetSize(sl1));
	REQUIRE(TRUE == SortedListIsEmpty(sl2));
	SortedListDestroy(sl1);
	SortedListDestroy(sl2);

	sl1 = UtilityCreateSL(2, array);
	sl2 = UtilityCreateSL(10, array);

	SortedListMerge(sl1, sl2);
	REQUIRE(0 == *(int *)SortedListGetData(
									SortedListGetNext(SortedListBegin(sl1))));
	REQUIRE(90 == *(int *)SortedListGetData(
									SortedListGetPrevious(SortedListEnd(sl1))));
	REQUIRE(12 == SortedListGetSize(sl1));
	REQUIRE(TRUE == SortedListIsEmpty(sl2));

	SortedListDestroy(sl1);
	SortedListDestroy(sl2);
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
