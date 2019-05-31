/*******************************************************************************
* Name: sorting_algorithms3  Test file                                         *
*                                                                              *
*Created BY Bareket Sar Shalom                                                 *
* Last Update: 26.12.18                                                        *
*******************************************************************************/
#include <stdio.h>	/* printf */
#include <stdlib.h> /* size_t, qsort, rand */
#include <string.h> /* memcpy */
#include <time.h> /* time */

#include "../include/test.h"
#include "sorting.h"

#define INT_ARRAY_SIZE 3
#define CHAR_ARRAY_SIZE 26
#define ASCII_A  'A'
#define ASCII_Z  'Z'
#define BITS_IN_BYTES 8
#define WORD_BYTES (__WORDSIZE / BITS_IN_BYTES)

#define DURATION_TEST_SIZE 100000000
/******************************************************************************/

TestResult TestHeapSort();
TestResult TestHeapSort2();
TestResult TestHeapSortDuration();
TestResult TestQuickSort();
/******************************************************************************/

int int_array[INT_ARRAY_SIZE];
char char_array[CHAR_ARRAY_SIZE];

int sorted_int_array[INT_ARRAY_SIZE];
char sorted_char_array[CHAR_ARRAY_SIZE];



int main()
{
	/*RUN_TEST(TestHeapSort2);
	RUN_TEST(TestHeapSort);
	RUN_TEST(TestHeapSortDuration);*/

	RUN_TEST(TestQuickSort);

return (0);
}
/******************************************************************************/

int CompareIntFunction(const void* before, const void* after)
{
	/*printf("%d , %d\n", *(int*)before , *(int*)after);*/
	return (*(int*)before - *(int*)after);
}

int CompareCharFunction(const void* before, const void* after)
{
	/*printf("%d , %d\n", *(int*)before , *(int*)after);*/
	return (*(char*)before - *(char*)after);
}
/******************************************************************************/

void BeforeTest()
{
  int i = 0;

  for( ; i < INT_ARRAY_SIZE; ++i)
  {
    /*int_array[i] = (INT_ARRAY_SIZE - 1) - i;*/
		int_array[i] = (rand() * time(NULL)) % INT_ARRAY_SIZE;
  }

  for(i = 0 ; i < CHAR_ARRAY_SIZE; ++i)
  {
    char_array[i] = ASCII_Z - i;
  }

	memcpy(sorted_int_array, int_array, sizeof(int) * INT_ARRAY_SIZE);
	memcpy(sorted_char_array, char_array, sizeof(char) * CHAR_ARRAY_SIZE);

	qsort(sorted_int_array, INT_ARRAY_SIZE, sizeof(int), CompareIntFunction);
	qsort(sorted_char_array, CHAR_ARRAY_SIZE, sizeof(char), CompareCharFunction);
}

void RandomInitBeforeTest(int *array1, int *array2)
{
	size_t i = 0;
	int number = 0;

	for ( ; i < DURATION_TEST_SIZE; ++i)
	{
		number = (rand() * time(NULL)) % DURATION_TEST_SIZE - (DURATION_TEST_SIZE / 2);

		array1[i] = number;
	}

	memcpy(array2, array1, sizeof(int) * DURATION_TEST_SIZE);
}

/******************************************************************************/

TestResult TestHeapSort()
{
  int i = 0;
  BeforeTest();

  HeapSort(int_array, sizeof(int), INT_ARRAY_SIZE, CompareIntFunction);
	HeapSort(char_array, sizeof(char), CHAR_ARRAY_SIZE, CompareCharFunction);

#ifdef PRINT

	printf("\n");
	for(i = 0 ; i < INT_ARRAY_SIZE; ++i)
	{
		printf("%d,",sorted_int_array[i]);
	}
	printf("\n");
	for(i = 0 ; i < INT_ARRAY_SIZE; ++i)
	{
		printf("%d,",int_array[i]);
	}
	printf("\n");

#endif

	for(i = 0 ; i < INT_ARRAY_SIZE; ++i)
	{
		REQUIRE(sorted_int_array[i] == int_array[i]);
	}

	for(i = 0 ; i < CHAR_ARRAY_SIZE; ++i)
  {
		REQUIRE(sorted_char_array[i] == char_array[i]);
	}

	return (TEST_PASS);
}

TestResult TestHeapSort2()
{
  int i = 0;
  int a[10] = {9,8,1,5,9,5,8,6,7,3};

printf("\n");
	for( ; i < INT_ARRAY_SIZE; ++i)
	{
		/*printf("%d,", a[i]);*/
		/*REQUIRE(i == int_array[i]);*/
	}
  HeapSort(a, sizeof(int), INT_ARRAY_SIZE, CompareIntFunction);
	/*HeapSort(char_array, sizeof(char), CHAR_ARRAY_SIZE, CompareCharFunction);*/
printf("\n");
	for(i = 0 ; i < INT_ARRAY_SIZE; ++i)
  {
    /*printf("%d,", a[i]);*/
    /*REQUIRE(i == int_array[i]);*/
  }

	/*printf("\ni = %d, %c\n", i, char_array[i]);*/
  /*for(i = 0 ; i < CHAR_ARRAY_SIZE; ++i)
  {

	REQUIRE((ASCII_A + i) == char_array[i]);
	}*/

	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestHeapSortDuration()
{
	int i = 0, start_time = 0, end_time = 0;
	int *duration_test_array = malloc(DURATION_TEST_SIZE * sizeof(int));
	int *q_duration_test_array = malloc(DURATION_TEST_SIZE * sizeof(int));

	RandomInitBeforeTest(duration_test_array, q_duration_test_array);
	start_time = time(NULL);
	HeapSort(duration_test_array, sizeof(int), DURATION_TEST_SIZE, CompareIntFunction);
	end_time = time(NULL);
	printf("\nHeapSort: %d s\n", end_time - start_time);

	start_time = time(NULL);
	qsort(q_duration_test_array, DURATION_TEST_SIZE, sizeof(int), CompareIntFunction);
	end_time = time(NULL);

	printf("qsort:     %d s\n", end_time - start_time);

	for (; i < DURATION_TEST_SIZE; ++i)
	{
		REQUIRE(duration_test_array[i] == q_duration_test_array[i]);
	}
	return (TEST_PASS);
}


/******************************************************************************/

TestResult TestQuickSort()
{
	int i = 0, start_time = 0, end_time = 0;
	int *duration_test_array = malloc(DURATION_TEST_SIZE * sizeof(int));
	int *q_duration_test_array = malloc(DURATION_TEST_SIZE * sizeof(int));

	RandomInitBeforeTest(duration_test_array, q_duration_test_array);
	start_time = time(NULL);
	QuickSort(duration_test_array, sizeof(int), DURATION_TEST_SIZE, CompareIntFunction);
	end_time = time(NULL);
	printf("\nHeapSort: %d s\n", end_time - start_time);

	start_time = time(NULL);
	qsort(q_duration_test_array, DURATION_TEST_SIZE, sizeof(int), CompareIntFunction);
	end_time = time(NULL);

	printf("qsort:     %d s\n", end_time - start_time);

	for (; i < DURATION_TEST_SIZE; ++i)
	{
		REQUIRE(duration_test_array[i] == q_duration_test_array[i]);
	}
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
