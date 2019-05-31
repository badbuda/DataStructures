/*******************************************************************************
* NAME: AVL tree Test file                                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  15.12.18                                                      *
*******************************************************************************/

#include <stdlib.h> /* size_t, qsort, rand */
#include <string.h>	/* memcpy */
#include <time.h> /* time */
#include <math.h>

#include "test.h"
#include "avl_tree.h"

#define INT_ARRAY_SIZE 5000 /* change to 5000 */
#define ARRAY_SIZE_FOR_BALANCE 6
#define EMPTY 0

#define ADD_ONE 1
#define IN_TREE 1
#define NOT_IN_TREE (INT_ARRAY_SIZE + 1)
/******************************************************************************/

TestResult TestAvlTreeCreateDestroy();
TestResult TestAvlTreeInsert();
TestResult TestAvlTreeCount();
TestResult TestAvlTreeIsEmpty();
TestResult TestAvlTreeFind();
TestResult TestAvlTreeForEach();
TestResult TestAvlTreeRemove();
TestResult TestAvlTreeHeightBeforeBalance();
TestResult TestAvlTreeHeightAndBalance();
/******************************************************************************/

int sorted_int_array[INT_ARRAY_SIZE];
int orig_int_array[INT_ARRAY_SIZE];
int test_int_array[INT_ARRAY_SIZE];

int main()
{
	RUN_TEST(TestAvlTreeCreateDestroy);
	RUN_TEST(TestAvlTreeInsert);
	RUN_TEST(TestAvlTreeCount);
	RUN_TEST(TestAvlTreeIsEmpty);
	RUN_TEST(TestAvlTreeFind);
	RUN_TEST(TestAvlTreeForEach);
	RUN_TEST(TestAvlTreeRemove);
	RUN_TEST(TestAvlTreeHeightBeforeBalance);
	RUN_TEST(TestAvlTreeHeightAndBalance);

return (0);
}
/******************************************************************************/

int CompareIntFunction(const void* data, const void* param)
{
	return (*(int*)data - *(int*)param);
}

bool IsInArray(int *int_array, int num, size_t ln_array)
{
	size_t i = 0;

	for ( ; i < ln_array; ++i)
	{
		if (int_array[i] == num)
		{
			return (TRUE);
		}
	}

	return (FALSE);
}

void InitBeforeTest()
{
	size_t i = 0;
	int number = 0;

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		number = (rand() * time(NULL)) % INT_ARRAY_SIZE - (INT_ARRAY_SIZE / 2);

		if (FALSE == IsInArray(test_int_array, number, i))
		{
			test_int_array[i] = number;
		}
		else
		{
			--i;
		}
	}

	memcpy(sorted_int_array, test_int_array, sizeof(int) * INT_ARRAY_SIZE);
	qsort(sorted_int_array, INT_ARRAY_SIZE, sizeof(int), CompareIntFunction);
}


avl_tree_t *CreateTreeBeforeTest(compare_function_t compare)
{
	int i = 0;
	avl_tree_t *avl = AvlTreeCreate(compare);
	InitBeforeTest();

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		AvlTreeInsert(avl, &(test_int_array[i]));
	}

	return (avl);
}
/******************************************************************************/

TestResult TestAvlTreeCreateDestroy()
{
	avl_tree_t *avl = AvlTreeCreate(CompareIntFunction);
	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestAvlTreeInsert()
{
	avl_tree_t *avl = CreateTreeBeforeTest(CompareIntFunction);

	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestAvlTreeCount()
{
	avl_tree_t *avl = AvlTreeCreate(CompareIntFunction);

	REQUIRE((size_t)EMPTY == AvlTreeCount(avl));
	AvlTreeDestroy(avl);

	avl = CreateTreeBeforeTest(CompareIntFunction);
	REQUIRE(INT_ARRAY_SIZE == AvlTreeCount(avl));

	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestAvlTreeIsEmpty()
{
	avl_tree_t *avl = AvlTreeCreate(CompareIntFunction);

	REQUIRE(TRUE == AvlTreeIsEmpty(avl));
	AvlTreeDestroy(avl);

	avl = CreateTreeBeforeTest(CompareIntFunction);
	REQUIRE(FALSE == AvlTreeIsEmpty(avl));

	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestAvlTreeFind()
{
	int exist_data = IN_TREE;
	int non_exist_data = NOT_IN_TREE;
	int i = 0;
	avl_tree_t *avl = CreateTreeBeforeTest(CompareIntFunction);

	REQUIRE((int)IN_TREE == *(int *)AvlTreeFind(avl, &exist_data));
	REQUIRE(NULL == AvlTreeFind(avl, &non_exist_data));

	for (i = -(INT_ARRAY_SIZE / 2) ; i < (INT_ARRAY_SIZE / 2); ++i)
	{
		REQUIRE((int)i == *(int *)AvlTreeFind(avl, &i));
	}

	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

status AddOneActionFunction(void *data, void *param)
{
	*(int*)data += *(int*)param;

		return (SUCCESS);
}

TestResult TestAvlTreeForEach()
{
	int add_one_param = ADD_ONE;
	int i = 0;
	avl_tree_t *avl = CreateTreeBeforeTest(CompareIntFunction);

	memcpy(orig_int_array, test_int_array, sizeof(int) * INT_ARRAY_SIZE);
	AvlTreeForEach(avl, AddOneActionFunction, &add_one_param);

	for (; i < INT_ARRAY_SIZE; ++i)
	{
		REQUIRE(test_int_array[i] == orig_int_array[i] + 1);
	}

	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestAvlTreeRemove()
{
	int exist_data = IN_TREE;
	int non_exist_data = NOT_IN_TREE;
	int i = 0;
	avl_tree_t *avl = CreateTreeBeforeTest(CompareIntFunction);

	AvlTreeRemove(avl, &non_exist_data);
	REQUIRE(INT_ARRAY_SIZE == AvlTreeCount(avl));

	AvlTreeRemove(avl, &exist_data);
	REQUIRE(INT_ARRAY_SIZE - 1 == AvlTreeCount(avl));
	REQUIRE(NULL == AvlTreeFind(avl, &exist_data));

	for (i = -(INT_ARRAY_SIZE / 2) ; i < exist_data; ++i)
	{
		REQUIRE((int)i == *(int *)AvlTreeFind(avl, &i));
	}

	for (i = exist_data + 1 ; i < (INT_ARRAY_SIZE / 2); ++i)
	{
		REQUIRE((int)i == *(int *)AvlTreeFind(avl, &i));
	}

	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestAvlTreeHeightBeforeBalance()
{
	avl_tree_t *avl = CreateTreeBeforeTest(CompareIntFunction);

	REQUIRE((size_t)INT_ARRAY_SIZE >= AvlTreeHeight(avl));

	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestAvlTreeHeightAndBalance()
{
	int i = 0;
	int int_array[ARRAY_SIZE_FOR_BALANCE] = {10, 1, 20, 30, 17, 18};
	avl_tree_t *avl = AvlTreeCreate(CompareIntFunction);
	InitBeforeTest();

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		AvlTreeInsert(avl, &(sorted_int_array[i]));
	}

	REQUIRE((size_t)((log(INT_ARRAY_SIZE) / log(2) + 1)) == AvlTreeHeight(avl));
	AvlTreeDestroy(avl);

	avl = AvlTreeCreate(CompareIntFunction);

	for (i = 0 ; i < ARRAY_SIZE_FOR_BALANCE; ++i)
	{
		AvlTreeInsert(avl, &(int_array[i]));
	}

	REQUIRE((size_t)((log(ARRAY_SIZE_FOR_BALANCE) / log(2) + 1)) ==
		                             AvlTreeHeight(avl));
	AvlTreeDestroy(avl);
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
