/*******************************************************************************
* NAME:    stack_test                                                          *
*                                                                              *
* PURPOSE: tests for the safe_stack        				       *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  06.11.18                                                      *
*******************************************************************************/
#include <stdio.h>
#include "test.h"
#include "stack.h"
#include "safe_stack.h"

TestResult StackTest(void);

int main()
{
	RUN_TEST (StackTest);

	return 0;
}

TestResult StackTest(void)
{
	int i = 0;
	int a = 4;
	int b = 5;

	stack_t *s = SafeStackCreate(100, sizeof(int));

/*	void *array_address = (char *)SafeStackPeek(s) + sizeof(int);	*/

	REQUIRE(100 == SafeStackGetCapacity(s));
	REQUIRE(sizeof(int) == SafeStackGetElementSize(s));
	REQUIRE(0 == SafeStackGetSize(s));
	REQUIRE(SafeStackPeek(s) == NULL);

	SafeStackPush(s, &a);
	REQUIRE(100 == SafeStackGetCapacity(s));
	REQUIRE(sizeof(int) == SafeStackGetElementSize(s));
	REQUIRE(1 == SafeStackGetSize(s));
	REQUIRE(*(int *)SafeStackPeek(s) == a);

	SafeStackPush(s, &b);
	REQUIRE(100 == SafeStackGetCapacity(s));
	REQUIRE(sizeof(int) == SafeStackGetElementSize(s));
	REQUIRE(2 == SafeStackGetSize(s));
	REQUIRE(*(int *)SafeStackPeek(s) == b);

	SafeStackPop(s);
	REQUIRE(100 == SafeStackGetCapacity(s));
	REQUIRE(sizeof(int) == SafeStackGetElementSize(s));
	REQUIRE(1 == SafeStackGetSize(s));
	REQUIRE(*(int *)SafeStackPeek(s) == a);

	SafeStackPop(s);
	REQUIRE(100 == SafeStackGetCapacity(s));
	REQUIRE(sizeof(int) == SafeStackGetElementSize(s));
	REQUIRE(0 == SafeStackGetSize(s));

	for(i = 0; i < 105 ; ++i)
	{
		SafeStackPush(s, &a);
	}

	REQUIRE(100 == SafeStackGetSize(s));
	REQUIRE(*(int *)SafeStackPeek(s) == a);

	for(i = 0; i < 105 ; ++i)
	{
		SafeStackPop(s);
	}

	REQUIRE(0 == SafeStackGetSize(s));
	REQUIRE(SafeStackPeek(s) == NULL);

	StackDestroy(&s);

/*	printf("%lu, %lu, %lu\n", StackGetElementSize(s), StackGetCapacity(s), StackGetSize(s));*/
/*	printf("%p\n", StackPeek(s));*/

	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
