/*******************************************************************************
* NAME: Singly Linked List                                                     *
*                                                                              *
* PURPOSE: Singly Linked List Tests                                            *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  13.11.18                                                      *
*******************************************************************************/
#include <stdio.h>
#include "test.h"
#include "s_linked_list.h"

TestResult SLinkedListCreateTest();
TestResult SLinkedListDestroyTest();
TestResult SLinkedListInsertTest();
TestResult SLinkedListInsertAfterTest();
TestResult SLinkedListRemoveTest();
TestResult SLinkedListRemoveAfterTest();
TestResult SLinkedListFindTest();
TestResult SLinkedListIntersectionTest();
TestResult SLinkedListForEachTest();
TestResult SLinkedListGetSizeTest();
TestResult SLinkedListHasLoopTest();
TestResult SLinkedListFlipTest();
TestResult DisneylandTest();
bool MatchFunctionTest(void* data, void* param);
status ActionFunctionTest(void *data, void *param);
status ActionFunctionDisney(void *data, void *param);

int main()
{
	RUN_TEST(SLinkedListCreateTest);
	RUN_TEST(SLinkedListDestroyTest);
	RUN_TEST(SLinkedListInsertAfterTest);
	RUN_TEST(SLinkedListInsertTest);
	RUN_TEST(SLinkedListRemoveAfterTest);
	RUN_TEST(SLinkedListRemoveTest);
	RUN_TEST(SLinkedListGetSizeTest);
	RUN_TEST(SLinkedListIntersectionTest);
	RUN_TEST(SLinkedListHasLoopTest);
	RUN_TEST(SLinkedListFlipTest);
	RUN_TEST(SLinkedListFindTest);
	RUN_TEST(SLinkedListForEachTest);
	RUN_TEST(DisneylandTest);

	return 0;
}
/******************************************************************************/

TestResult SLinkedListCreateTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	void* data = NULL;
	int array[] = {3,2,1,0};

	data = (void*)array;

	LL1 = SLinkedListCreate(data, NULL);

	REQUIRE(3 == *(int*)(LL1->data));
	REQUIRE(1 == ((int*)LL1->data)[2]);
	REQUIRE(0 == ((int*)LL1->data)[3]);

	REQUIRE(NULL == LL1->next);

	LL2 = SLinkedListCreate(data, LL1);

	REQUIRE(LL1 == LL2->next);

	SLinkedListDestroy(&LL2);

	return (TEST_PASS);
}

TestResult SLinkedListDestroyTest()
{
	sll_node_t* LL1 = NULL;
	void* data = NULL;

	LL1 = SLinkedListCreate(data, NULL);

	SLinkedListDestroy(&LL1);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SLinkedListInsertAfterTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;

	void* data = NULL;
	int array[] = {0,1,2,3};

	data = (void*)array;

	LL1 = SLinkedListCreate(data, NULL);
	LL2 = SLinkedListCreate(data, LL1);
	LL3 = SLinkedListCreate(data, NULL);
	SLinkedListInsertAfter(LL2, LL3);
	REQUIRE(LL3 == LL2->next);
	REQUIRE(LL1 == LL3->next);
	REQUIRE(NULL == LL1->next);
	SLinkedListDestroy(&LL2);

	LL1 = SLinkedListCreate(data, NULL);
	LL2 = SLinkedListCreate(data, LL1);
	LL3 = SLinkedListCreate(data, NULL);
	SLinkedListInsertAfter(LL1, LL3);
	REQUIRE(LL3 == LL1->next);
	REQUIRE(LL1 == LL2->next);
	REQUIRE(NULL == LL3->next);
	SLinkedListDestroy(&LL2);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult SLinkedListInsertTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;

	void* data1 = NULL;
	void* data2 = NULL;
	int array1[] = {0,1,2,3};
	int array2[] = {3,2,1,0};

	data1 = (void*)array1;
	data2 = (void*)array2;

	LL1 = SLinkedListCreate(data1, NULL);
	LL2 = SLinkedListCreate(data1, LL1);
	LL3 = SLinkedListCreate(data2, NULL);
	head = SLinkedListInsert(LL2, LL3);
	REQUIRE(LL2 == head);
	REQUIRE(LL3 == head->next);
	REQUIRE(NULL == LL1->next);
	REQUIRE(3 == *(int*)(head->data));
	REQUIRE(1 == ((int*)head->data)[2]);
	REQUIRE(0 == ((int*)head->data)[3]);
	SLinkedListDestroy(&LL2);

	LL1 = SLinkedListCreate(data1, NULL);
	LL2 = SLinkedListCreate(data1, LL1);
	LL3 = SLinkedListCreate(data2, NULL);
	head = LL2;
	SLinkedListInsert(LL1, LL3);
	REQUIRE(LL2 == head);
	REQUIRE(LL3 == head->next->next);
	REQUIRE(NULL == LL3->next);
	REQUIRE(3 == *(int*)(head->next->data));
	REQUIRE(1 == ((int*)head->next->data)[2]);
	REQUIRE(0 == ((int*)head->next->data)[3]);
	SLinkedListDestroy(&LL2);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult SLinkedListRemoveAfterTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;

	void* data1 = NULL;
	void* data2 = NULL;
	void* data3 = NULL;
	int array1[] = {1};
	int array2[] = {2};
	int array3[] = {3};

	data1 = (void*)array1;
	data2 = (void*)array2;
	data3 = (void*)array3;

	LL3 = SLinkedListCreate(data3, NULL);
	LL2 = SLinkedListCreate(data2, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;
	REQUIRE(2 == ((int*)head->next->data)[0]);
	REQUIRE(LL2 == SLinkedListRemoveAfter(LL1));
	REQUIRE(3 == ((int*)head->next->data)[0]);
	REQUIRE(NULL == LL2->next);
	SLinkedListDestroy(&LL2);
	SLinkedListDestroy(&LL1);

	LL3 = SLinkedListCreate(data3, NULL);
	LL2 = SLinkedListCreate(data2, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;
	REQUIRE(LL3 == (head->next->next));
	SLinkedListRemoveAfter(LL2);
	REQUIRE(NULL == (head->next->next));

	SLinkedListDestroy(&LL3);
	SLinkedListDestroy(&LL1);
	return (TEST_PASS);
}

TestResult SLinkedListRemoveTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;

	void* data1 = NULL;
	void* data2 = NULL;
	void* data3 = NULL;
	int array1[] = {1};
	int array2[] = {2};
	int array3[] = {3};

	data1 = (void*)array1;
	data2 = (void*)array2;
	data3 = (void*)array3;

	LL3 = SLinkedListCreate(data3, NULL);
	LL2 = SLinkedListCreate(data2, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;
	REQUIRE(2 == ((int*)head->next->data)[0]);
	REQUIRE(1 == *(int*)SLinkedListRemove(LL1)->data);
	REQUIRE(2 == ((int*)head->data)[0]);
	REQUIRE(3 == ((int*)head->next->data)[0]);
	REQUIRE(NULL == LL2->next);
	SLinkedListDestroy(&LL2);
	SLinkedListDestroy(&LL1);

	LL3 = SLinkedListCreate(data3, NULL);
	LL2 = SLinkedListCreate(data2, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;
	REQUIRE(2 == ((int*)head->next->data)[0]);
	SLinkedListRemove(LL2);
	REQUIRE(3 == ((int*)head->next->data)[0]);
	REQUIRE(NULL == LL3->next);

	SLinkedListDestroy(&LL3);
	SLinkedListDestroy(&LL1);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult SLinkedListGetSizeTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;

	void* data = NULL;

	LL3 = SLinkedListCreate(data, NULL);
	LL2 = SLinkedListCreate(data, LL3);
	LL1 = SLinkedListCreate(data, LL2);
	head = LL1;
	REQUIRE(3 == SLinkedListGetSize(head));

	SLinkedListDestroy(&LL1);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SLinkedListIntersectionTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head1 = NULL;

	sll_node_t* LL11 = NULL;
	sll_node_t* LL22 = NULL;
	sll_node_t* head2 = NULL;

	void* data = NULL;

	LL3 = SLinkedListCreate(data, NULL);
	LL2 = SLinkedListCreate(data, LL3);
	LL1 = SLinkedListCreate(data, LL2);
	head1 = LL1;

	LL22 = SLinkedListCreate(data, LL3);
	LL11 = SLinkedListCreate(data, LL22);
	head2 = LL11;

	REQUIRE(LL3 == SLinkedListIntersection(head1,head2));

	LL22->next = NULL;
	REQUIRE(NULL == SLinkedListIntersection(head1,head2));

	SLinkedListDestroy(&head1);
	SLinkedListDestroy(&head2);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult SLinkedListHasLoopTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;
	void* data = NULL;

	LL3 = SLinkedListCreate(data, NULL);
	LL2 = SLinkedListCreate(data, LL3);
	LL1 = SLinkedListCreate(data, LL2);
	head = LL1;
	REQUIRE(FALSE == SLinkedListHasLoop(head));

	LL3->next = LL1;
	REQUIRE(TRUE == SLinkedListHasLoop(head));
	LL3->next = NULL;

	LL2->next = NULL;
	REQUIRE(FALSE == SLinkedListHasLoop(head));
	LL2->next = LL1;
	REQUIRE(TRUE == SLinkedListHasLoop(head));
	LL2->next = LL3;

	SLinkedListDestroy(&head);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult SLinkedListFlipTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;

	void* data = NULL;

	LL3 = SLinkedListCreate(data, NULL);
	LL2 = SLinkedListCreate(data, LL3);
	LL1 = SLinkedListCreate(data, LL2);
	head = LL1;

	head = SLinkedListFlip(head);
	REQUIRE(LL3 == head);
	REQUIRE(LL2 == (head->next));
	REQUIRE(LL1 == (head->next->next));
	REQUIRE(NULL == (head->next->next->next));

	SLinkedListDestroy(&head);

	LL1 = SLinkedListCreate(data, NULL);
	head = LL1;
	head = SLinkedListFlip(head);
	REQUIRE(LL1 == head);
	REQUIRE(NULL == (head->next));

	SLinkedListDestroy(&head);
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

TestResult SLinkedListFindTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;
	match_function_t match = MatchFunctionTest;

	int array1[] = {1};
	int array2[] = {2};
	int array3[] = {3};
	int array4[] = {3};

	void* data1 = (void*)array1;
	void* data2 = (void*)array2;
	void* data3 = (void*)array3;
	void* param = (void*)array4;

	LL3 = SLinkedListCreate(data3, NULL);
	LL2 = SLinkedListCreate(data2, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;

	REQUIRE(LL3 == SLinkedListFind(head, match, param));

	array4[0] = 4;
	REQUIRE(NULL == SLinkedListFind(head, match, param));

	SLinkedListDestroy(&head);
	return (TEST_PASS);
}
/*****************************************************************************/

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

TestResult SLinkedListForEachTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* head = NULL;
	action_function_t action = ActionFunctionTest;

	int array1[] = {1};
	int array2[] = {2};
	int array3[] = {3};
	int array4[] = {4};

	void* data1 = (void*)array1;
	void* data2 = (void*)array2;
	void* data3 = (void*)array3;
	void* param = (void*)array4;

	LL3 = SLinkedListCreate(data3, NULL);
	LL2 = SLinkedListCreate(data2, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;

	REQUIRE(SUCCESS == SLinkedListForEach(head, action, param));
	REQUIRE(5 == *array1);
	REQUIRE(6 == *array2);
	REQUIRE(7 == *array3);

	SLinkedListDestroy(&head);
	return (TEST_PASS);
}
/******************************************************************************/

status ActionFunctionDisney(void *data, void *param)
{
	*(int*)param ^= *(int*)data;

	return (SUCCESS);
}

TestResult DisneylandTest()
{
	sll_node_t* LL1 = NULL;
	sll_node_t* LL2 = NULL;
	sll_node_t* LL3 = NULL;
	sll_node_t* LL4 = NULL;
	sll_node_t* LL5 = NULL;
	sll_node_t* LL6 = NULL;
	sll_node_t* LL7 = NULL;
	sll_node_t* head = NULL;
	action_function_t action = ActionFunctionDisney;

	int array_param[] = {0};
	int array1[] = {1};
	int array2[] = {2};
	int array3[] = {3};
	int array4[] = {4};

	void* param = (void*)array_param;
	void* data1 = (void*)array1;
	void* data2 = (void*)array2;
	void* data3 = (void*)array3;
	void* data4 = (void*)array4;

	LL7 = SLinkedListCreate(data3, NULL);
	LL6 = SLinkedListCreate(data3, LL7);
	LL5 = SLinkedListCreate(data4, LL6);
	LL4 = SLinkedListCreate(data4, LL5);
	LL3 = SLinkedListCreate(data1, LL4);
	LL2 = SLinkedListCreate(data2, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;
	REQUIRE(SUCCESS == SLinkedListForEach(head, action, param));
	REQUIRE(2 == *(int*)param);
	SLinkedListDestroy(&head);

	array_param[0] = 0;
	LL6 = SLinkedListCreate(data3, NULL);
	LL5 = SLinkedListCreate(data2, LL6);
	LL4 = SLinkedListCreate(data3, LL5);
	LL3 = SLinkedListCreate(data2, LL4);
	LL2 = SLinkedListCreate(data1, LL3);
	LL1 = SLinkedListCreate(data1, LL2);
	head = LL1;
	REQUIRE(SUCCESS == SLinkedListForEach(head, action, param));
	REQUIRE(0 == *(int*)param);
	SLinkedListDestroy(&head);

	return (TEST_PASS);
}
/*******************************************************************************
*******************************************************************************/
