/*******************************************************************************
* NAME: binary sorted tree Test file                                           *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  13.12.18                                                      *
*******************************************************************************/

#include <stdlib.h> /* size_t */
#include <stdio.h>	/* printf */

#include "test.h"
#include "binary_sorted_tree.h"

#define INT_ARRAY_SIZE 10
#define MIN_NUM 0
#define MAX_NUM 14
#define ADD_ONE 1
#define NOT_IN_TREE 999
/******************************************************************************/

TestResult TestBSTCreateDestroy();
TestResult TestBSTGetSize();
TestResult TestBSTInsertGetNextPrev();
TestResult TestBSTIsEmpty();
TestResult TestBSTBegin();
TestResult TestBSTEnd();
TestResult TestBSTFind();
TestResult TestBSTForEach();
TestResult TestBSTIsSameIterator();
TestResult TestBSTRemove();
/******************************************************************************/

int sorted_int_array[INT_ARRAY_SIZE] = {MIN_NUM, 1, 2, 3, 4, 5, 7, 9, 12, MAX_NUM};
int orig_int_array[INT_ARRAY_SIZE];
int test_int_array[INT_ARRAY_SIZE];

int main()
{
	RUN_TEST(TestBSTCreateDestroy);
	RUN_TEST(TestBSTIsEmpty);
	RUN_TEST(TestBSTGetSize);
	RUN_TEST(TestBSTBegin);
	RUN_TEST(TestBSTEnd);
	RUN_TEST(TestBSTInsertGetNextPrev);
	RUN_TEST(TestBSTFind);
	RUN_TEST(TestBSTForEach);
	RUN_TEST(TestBSTIsSameIterator);
	RUN_TEST(TestBSTRemove);

return (0);
}
/******************************************************************************/

void InitBeforeTest(int *int_array)
{
	int_array[0] = sorted_int_array[5];
	int_array[1] = sorted_int_array[1];
	int_array[2] = sorted_int_array[8];
	int_array[3] = sorted_int_array[3];
	int_array[4] = sorted_int_array[7];
	int_array[5] = sorted_int_array[6];
	int_array[6] = sorted_int_array[4];
	int_array[7] = sorted_int_array[0];
	int_array[8] = sorted_int_array[9];
	int_array[9] = sorted_int_array[2];
}

bst_t *CreateTreeBeforeTest(compare_function_t compare, int *int_array)
{
	int i = 0;
	bst_t *bst = BSTCreate(compare);

	InitBeforeTest(int_array);

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		BSTInsert(bst, &(int_array[i]));
	}

	return (bst);
}

int CompareIntFunction(const void* data, const void* param)
{
  return (*(int*)data - *(int*)param);
}
/******************************************************************************/

TestResult TestBSTCreateDestroy()
{
	bst_t *bst = BSTCreate(CompareIntFunction);
	BSTDestroy(bst);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestBSTInsertGetNextPrev()
{
	int i = 0;
	bst_iterator_t iterator;
	bst_t *bst = BSTCreate(CompareIntFunction);

	InitBeforeTest(test_int_array);

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		BSTInsert(bst, &(test_int_array[i]));
	}

	iterator = BSTBegin(bst);
	/*REQUIRE(NULL == BSTGetPrevious(iterator).info);*/

	for (i = 1 ; i < INT_ARRAY_SIZE ; ++i)
	{
		iterator = BSTGetNext(iterator);
		REQUIRE(sorted_int_array[i] == *(int *)BSTGetData(iterator));
	}

	iterator = BSTGetNext(iterator);
	REQUIRE(iterator.info == BSTEnd(bst).info);

	iterator = BSTEnd(bst);
	/*REQUIRE(NULL == BSTGetNext(iterator).info);*/

	for (i = INT_ARRAY_SIZE - 1 ; i >= 0; --i)
	{
		iterator = BSTGetPrevious(iterator);
		REQUIRE(sorted_int_array[i] == *(int *)BSTGetData(iterator));
	}

	BSTDestroy(bst);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestBSTIsEmpty()
{
	bst_t *bst = BSTCreate(NULL);

	REQUIRE(TRUE == BSTIsEmpty(bst));

	BSTInsert(bst, &(test_int_array[0]));
	REQUIRE(FALSE == BSTIsEmpty(bst));

	BSTDestroy(bst);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestBSTBegin()
{
	bst_t *bst = CreateTreeBeforeTest(CompareIntFunction, test_int_array);
	bst_iterator_t iterator = BSTBegin(bst);

	REQUIRE(MIN_NUM == *(int *)BSTGetData(iterator));

	BSTDestroy(bst);
	return (TEST_PASS);
}

TestResult TestBSTEnd()
{
	bst_iterator_t iterator;
	bst_t *bst = CreateTreeBeforeTest(CompareIntFunction, test_int_array);

	iterator = BSTEnd(bst);
	iterator = BSTGetPrevious(iterator);

	REQUIRE(MAX_NUM == *(int *)BSTGetData(iterator));

	BSTDestroy(bst);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestBSTGetSize()
{
	bst_t *bst = CreateTreeBeforeTest(CompareIntFunction, test_int_array);
	bst_t *empty_bst = BSTCreate(CompareIntFunction);

	REQUIRE(INT_ARRAY_SIZE == BSTGetSize(bst));
	REQUIRE(0 == BSTGetSize(empty_bst));

	BSTDestroy(bst);
	BSTDestroy(empty_bst);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestBSTFind()
{
	int param = NOT_IN_TREE;
	bst_iterator_t iterator;
	bst_t *bst = CreateTreeBeforeTest(CompareIntFunction, test_int_array);

	iterator = BSTFind(bst, &test_int_array[5]);
	REQUIRE(test_int_array[5] == *(int *)BSTGetData(iterator));
	REQUIRE(BST_ITERATOR_NOT_FOUND.info == BSTFind(bst, &param).info);

	BSTDestroy(bst);
	return (TEST_PASS);
}
/******************************************************************************/

status AddOneActionFunction(void *data, void *param)
{
	*(int*)data += *(int*)param;

		return (SUCCESS);
}

TestResult TestBSTForEach()
{
	int i = 0;
	int add_one_param = ADD_ONE;
	bst_t *bst = CreateTreeBeforeTest(CompareIntFunction, test_int_array);
	bst_t *empty_bst = BSTCreate(CompareIntFunction);

	REQUIRE(SUCCESS == BSTForEach(empty_bst, AddOneActionFunction, &add_one_param));
	REQUIRE(SUCCESS == BSTForEach(bst, AddOneActionFunction, &add_one_param));

	InitBeforeTest(orig_int_array);

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		REQUIRE(ADD_ONE + orig_int_array[i] == test_int_array[i]);
	}

	BSTDestroy(bst);
	BSTDestroy(empty_bst);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestBSTIsSameIterator()
{
	bst_iterator_t iterator1;
	bst_iterator_t iterator2;
	bst_t *bst = CreateTreeBeforeTest(CompareIntFunction, test_int_array);

	iterator1 = BSTBegin(bst);
	iterator2 = BSTBegin(bst);

	REQUIRE(TRUE == BSTIsSameIterator(iterator1, iterator2));

	iterator2 = BSTGetNext(iterator2);
	REQUIRE(FALSE == BSTIsSameIterator(iterator1, iterator2));

	BSTDestroy(bst);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestBSTRemove()
{
	size_t i = 0;
	bst_iterator_t iterator;
	bst_t *bst = CreateTreeBeforeTest(CompareIntFunction, test_int_array);

	for (i = 0; i < INT_ARRAY_SIZE; i++)
	{
		iterator = BSTBegin(bst);
		REQUIRE(sorted_int_array[i] == *(int *)BSTGetData(iterator));
		REQUIRE((INT_ARRAY_SIZE - i) == BSTGetSize(bst));

		BSTRemove(iterator);
	}

	BSTDestroy(bst);
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
