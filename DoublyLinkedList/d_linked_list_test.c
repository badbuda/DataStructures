/*******************************************************************************
* NAME: Doubly Linked List Test file.                                          *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  18.11.18                                                      *
*******************************************************************************/

#include <stdio.h>  /* size_t */
#include "test.h"
#include "d_linked_list.h"

TestResult DLinkedListCreateTest();
TestResult DLinkedListIsEmptyTest();
TestResult DLinkedListBeginTest();
TestResult DLinkedListEndTest();
TestResult DLinkedListGetSizeTest();
TestResult DLinkedListInsertTest();
TestResult DLinkedListInsertAfterTest();
TestResult DLinkedListEraseTest();
TestResult DLinkedListPushFrontTest();
TestResult DLinkedListPushBackTest();
TestResult DLinkedListPopFrontTest();
TestResult DLinkedListPopBackTest();
TestResult DLinkedListFindTest();
bool MatchFunctionTest(void* data, void* param);
TestResult DLinkedListForEachTest();
status ActionFunctionTest(void *data, void *param);
TestResult DLinkedListSpliceTest();
TestResult DLinkedListGetNextTest();
TestResult DLinkedListGetPreviousTest();
TestResult DLinkedListGetDataTest();
TestResult DLinkedListIsSameIteratorTest();

int main()
{
	RUN_TEST(DLinkedListCreateTest);
	RUN_TEST(DLinkedListIsEmptyTest);
	RUN_TEST(DLinkedListBeginTest);
	RUN_TEST(DLinkedListEndTest);
	RUN_TEST(DLinkedListGetSizeTest);
	RUN_TEST(DLinkedListInsertTest);
	RUN_TEST(DLinkedListInsertAfterTest);
	RUN_TEST(DLinkedListEraseTest);
	RUN_TEST(DLinkedListPushFrontTest);
	RUN_TEST(DLinkedListPushBackTest);
	RUN_TEST(DLinkedListPopFrontTest);
	RUN_TEST(DLinkedListPopBackTest);
	RUN_TEST(DLinkedListFindTest);
	RUN_TEST(DLinkedListForEachTest);
	RUN_TEST(DLinkedListSpliceTest);
	RUN_TEST(DLinkedListGetNextTest);
	RUN_TEST(DLinkedListGetPreviousTest);
	RUN_TEST(DLinkedListGetDataTest);
	RUN_TEST(DLinkedListIsSameIteratorTest);

	return 0;
}
/******************************************************************************/

TestResult DLinkedListCreateTest()
{
	d_linked_list_t *dll = DLinkedListCreate();
	REQUIRE(NULL != dll);

	DLinkedListDestroy(dll);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListIsEmptyTest()
{
	d_linked_list_t *dll = DLinkedListCreate();
	REQUIRE(TRUE == DLinkedListIsEmpty(dll));
	DLinkedListDestroy(dll);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListGetSizeTest()
{
	d_linked_list_t *dll = DLinkedListCreate();
	REQUIRE(0 == DLinkedListGetSize(dll));
	DLinkedListDestroy(dll);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListBeginTest()
{
	int data = 20;
	d_linked_list_t *dll = DLinkedListCreate();
	DLinkedListPushFront(dll, (void *)&data);

	REQUIRE(20 == *(int *) DLinkedListGetData(DLinkedListBegin(dll)));

	DLinkedListDestroy(dll);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListEndTest()
{
	int data = 20;
	d_linked_list_t *dll = DLinkedListCreate();
	DLinkedListPushBack(dll, (void *)&data);

	REQUIRE(20 == *(int *) DLinkedListGetData(
							DLinkedListGetPrevious(DLinkedListEnd(dll))));
	DLinkedListDestroy(dll);

	return (TEST_PASS);

}
/******************************************************************************/

TestResult DLinkedListInsertTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter = DLinkedListPushBack(dll, (void *)&data);

	iter = DLinkedListInsert(iter, (void *)&data2);
	REQUIRE(30 == *(int *) DLinkedListGetData(iter));
	REQUIRE(20 == *(int *) DLinkedListGetData(DLinkedListGetNext(iter)));

	DLinkedListDestroy(dll);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListInsertAfterTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter = DLinkedListPushBack(dll, (void *)&data);

	iter = DLinkedListInsertAfter(iter, (void *)&data2);
	REQUIRE(30 == *(int *) DLinkedListGetData(iter));
	REQUIRE(20 == *(int *) DLinkedListGetData(DLinkedListGetPrevious(iter)));

	DLinkedListDestroy(dll);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListEraseTest()
{
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter = DLinkedListPushBack(dll, NULL);

	REQUIRE(1 == DLinkedListGetSize(dll));
	REQUIRE(FALSE == DLinkedListIsEmpty(dll));
	DLinkedListErase(iter);
	REQUIRE(0 == DLinkedListGetSize(dll));
	REQUIRE(TRUE == DLinkedListIsEmpty(dll));

	DLinkedListDestroy(dll);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListPushFrontTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter = DLinkedListPushBack(dll, (void *)&data);

	iter = DLinkedListPushFront(dll, (void *)&data2);
	REQUIRE(30 == *(int *) DLinkedListGetData(iter));
	REQUIRE(20 == *(int *) DLinkedListGetData(DLinkedListGetNext(iter)));

	DLinkedListDestroy(dll);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListPushBackTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter1 = DLinkedListPushBack(dll, (void *)&data);
	dll_iterator_t iter2 = DLinkedListPushBack(dll, (void *)&data2);

	REQUIRE(20 == *(int *) DLinkedListGetData(iter1));
	REQUIRE(30 == *(int *) DLinkedListGetData(iter2));

	DLinkedListDestroy(dll);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListPopFrontTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	DLinkedListPushBack(dll, (void *)&data);
	DLinkedListPushBack(dll, (void *)&data2);

	REQUIRE(2 == DLinkedListGetSize(dll));
	DLinkedListPopFront(dll);
	REQUIRE(1 == DLinkedListGetSize(dll));
	DLinkedListPopFront(dll);
	REQUIRE(0 == DLinkedListGetSize(dll));

	DLinkedListDestroy(dll);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListPopBackTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	DLinkedListPushBack(dll, (void *)&data);
	DLinkedListPushBack(dll, (void *)&data2);

	REQUIRE(2 == DLinkedListGetSize(dll));
	DLinkedListPopBack(dll);
	REQUIRE(1 == DLinkedListGetSize(dll));
	DLinkedListPopBack(dll);
	REQUIRE(0 == DLinkedListGetSize(dll));

	DLinkedListDestroy(dll);

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

TestResult DLinkedListFindTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	d_linked_list_t *dll2 = DLinkedListCreate();

	DLinkedListPushBack(dll, (void *)&data2);
	DLinkedListPushBack(dll, (void *)&data2);
	DLinkedListPushBack(dll, (void *)&data2);
	DLinkedListPushBack(dll, (void *)&data);
	DLinkedListPushBack(dll, (void *)&data2);

	REQUIRE(NULL != DLinkedListFind(dll, MatchFunctionTest, (void*)&data).info);
	DLinkedListDestroy(dll);

	DLinkedListPushBack(dll2, (void *)&data2);
	DLinkedListPushBack(dll2, (void *)&data2);
	DLinkedListPushBack(dll2, (void *)&data2);
	DLinkedListPushBack(dll2, (void *)&data2);

	REQUIRE(NULL== DLinkedListFind(dll2, MatchFunctionTest, (void*)&data).info);

	DLinkedListDestroy(dll2);

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

TestResult DLinkedListForEachTest()
{
	int data = 20;
	int data2 = 30;
	int param = 1;
	int result = 21;
	int result2 = 31;
	d_linked_list_t *dll = DLinkedListCreate();

	DLinkedListPushBack(dll, (void *)&data2);
	DLinkedListPushBack(dll, (void *)&data);


	REQUIRE(SUCCESS == DLinkedListForEach(dll, ActionFunctionTest,
													(void*)&param));
	REQUIRE(NULL != DLinkedListFind(dll, MatchFunctionTest, (void*)&result).info);
	REQUIRE(NULL != DLinkedListFind(dll, MatchFunctionTest, (void*)&result2).info);

	DLinkedListDestroy(dll);
	return (TEST_PASS);

}
/******************************************************************************/

TestResult DLinkedListSpliceTest()
{
	int data = 20;
	int data2 = 30;
	d_linked_list_t *dll = DLinkedListCreate();
	d_linked_list_t *dll2 = DLinkedListCreate();

	dll_iterator_t from, to;
	dll_iterator_t where = DLinkedListPushBack(dll, (void *)&data);
	DLinkedListPushBack(dll, (void *)&data);
	DLinkedListPushFront(dll, (void *)&data);

	from = DLinkedListPushBack(dll2, (void *)&data2);
	DLinkedListPushBack(dll2, (void *)&data2);
	DLinkedListPushBack(dll2, (void *)&data2);
	to = DLinkedListEnd(dll2);

	DLinkedListSplice(where, from, to);

	REQUIRE(6 == DLinkedListGetSize(dll));
	REQUIRE(0 == DLinkedListGetSize(dll2));
	REQUIRE(30 == *(int*)DLinkedListGetData(DLinkedListGetPrevious(where)));

	DLinkedListDestroy(dll);
	DLinkedListDestroy(dll2);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListGetNextTest()
{
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter1 = DLinkedListPushBack(dll, NULL);
	dll_iterator_t iter2 = DLinkedListPushBack(dll, NULL);

	REQUIRE(iter2.info == DLinkedListGetNext(iter1).info);

	DLinkedListDestroy(dll);
	return (TEST_PASS);

}
/******************************************************************************/

TestResult DLinkedListGetPreviousTest()
{
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter1 = DLinkedListPushBack(dll, NULL);
	dll_iterator_t iter2 = DLinkedListPushBack(dll, NULL);

	REQUIRE(iter1.info == DLinkedListGetPrevious(iter2).info);

	DLinkedListDestroy(dll);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListGetDataTest()
{
	int data = 20;
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter = DLinkedListPushBack(dll, (void *)&data);

	REQUIRE(20 == *(int*)DLinkedListGetData(iter));

	DLinkedListDestroy(dll);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult DLinkedListIsSameIteratorTest()
{
	d_linked_list_t *dll = DLinkedListCreate();
	dll_iterator_t iter1 = DLinkedListPushBack(dll, NULL);
	dll_iterator_t iter2 = DLinkedListPushBack(dll, NULL);

	REQUIRE(FALSE == DLinkedListIsSameIterator(iter1, iter2));

	DLinkedListDestroy(dll);
	return (TEST_PASS);
	return (TEST_PASS);
}
/*******************************************************************************
*******************************************************************************/
