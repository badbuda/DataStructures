/*******************************************************************************
* NAME: Hash Table                                                             *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  27.12.18                                                      *
*******************************************************************************/

#include <stdio.h> /* fscanf, fopen, fclose */
#include <stdlib.h> /* size_t */
#include <string.h>	/* strcmp */
#include <ctype.h> /* tolower */
#include <time.h> /* time */

#include "test.h"
#include "hash_table.h"

#define INT_ARRAY_SIZE 50
#define INT_ARRAY_RANGE 100
#define INT_KEYS 10
#define EMPTY 0
#define NOT_IN_HASH (INT_ARRAY_RANGE + 1)
#define ADD_ONE 1
#define EQUAL 0
/******************************************************************************/

TestResult TestHashCreateDestroy();
TestResult TestHashCreateDestroyForInts();
TestResult TestHashCountForInts();
TestResult TestHashIsEmptyForInts();
TestResult TestHashInsertForInts();
TestResult TestHashHashRemoveForInts();
TestResult TestHashFindForInts();
TestResult TestHashForEachForInts();

TestResult BasicTestHashFunctionForSpellChecker();
TestResult TestHashFunctionForSpellChecker();
TestResult TestHashStatisticsForSpellChecker();
/******************************************************************************/

unsigned ints_test_array[INT_ARRAY_SIZE];
unsigned ints_original_array[INT_ARRAY_SIZE];
unsigned ints_plus_one_array[INT_ARRAY_SIZE];

int main()
{
	RUN_TEST(TestHashCreateDestroy);
	RUN_TEST(TestHashCreateDestroyForInts);
	RUN_TEST(TestHashCountForInts);
	RUN_TEST(TestHashIsEmptyForInts);
	RUN_TEST(TestHashInsertForInts);
	RUN_TEST(TestHashHashRemoveForInts);
	RUN_TEST(TestHashFindForInts);
	RUN_TEST(TestHashForEachForInts);

	RUN_TEST(BasicTestHashFunctionForSpellChecker);
	RUN_TEST(TestHashFunctionForSpellChecker);
	RUN_TEST(TestHashStatisticsForSpellChecker);

	return (0);
}
/******************************************************************************/

static size_t HashFunctionForInts(const void *data)
{
	size_t key = 0;
	unsigned number = *(unsigned *)data;

	while (number)
	{
		key = number;
		number /= 10;
	}

	return (key);
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
}

static void FillIntsHash(hash_table_t *hash)
{
	size_t i = 0;

	for ( ; i < INT_ARRAY_SIZE; ++i)
	{
		HashInsert(hash, &ints_test_array[i]);
	}
}
/**************************TESTS***********************************************/

TestResult TestHashCreateDestroy()
{
	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, 0);
	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashCreateDestroyForInts()
{
	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, INT_KEYS);


	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashCountForInts()
{
	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, INT_KEYS);

	REQUIRE(EMPTY == HashCount(hash));

	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashIsEmptyForInts()
{
	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, INT_KEYS);

	REQUIRE(TRUE == HashIsEmpty(hash));

	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashInsertForInts()
{
	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, INT_KEYS);

	InitBeforeTest();
	FillIntsHash(hash);

	REQUIRE(FALSE == HashIsEmpty(hash));
	REQUIRE(INT_ARRAY_SIZE == HashCount(hash));

	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashHashRemoveForInts()
{
	size_t i = 0;
	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, INT_KEYS);

	InitBeforeTest();
	FillIntsHash(hash);

	for (i = 0 ; i < INT_ARRAY_SIZE; ++i)
	{
		HashRemove(hash, &ints_test_array[i]);
	}

	REQUIRE(TRUE == HashIsEmpty(hash));
	REQUIRE(EMPTY == HashCount(hash));

	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashFindForInts()
{
	size_t i = 0;
	void *find_data = NULL;
	unsigned not_in_array = NOT_IN_HASH;
	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, INT_KEYS);

	InitBeforeTest();
	FillIntsHash(hash);

	for (i = 0 ; i < INT_ARRAY_SIZE; ++i)
	{
		find_data = HashFind(hash, &ints_test_array[i]);
		REQUIRE(TRUE == MatchInts(find_data, &ints_test_array[i]));
	}

	find_data = HashFind(hash, &not_in_array);
	REQUIRE(NULL == find_data);

	HashDestroy(hash);
	return (TEST_PASS);
}

static status CountActionFunction(void *data, void *param)
{
	(void)data;

	*(size_t *)param += 1;

	return (SUCCESS);
}

TestResult TestHashForEachForInts()
{
	size_t counter = 0;

	hash_table_t *hash = HashCreate(HashFunctionForInts, MatchInts, INT_KEYS);

	InitBeforeTest();
	FillIntsHash(hash);

	HashForEach(hash, CountActionFunction, &counter);
	REQUIRE(INT_ARRAY_SIZE == counter);

	HashDestroy(hash);
	return (TEST_PASS);
}
/*******************SPELL CHECKER TESTS****************************************/

#define SPELL_CHECKER_LUT_SIZE 22179
#define WORD_BUFFER_SIZE 1000000
#define NUMBER_OF_WORDS 102287
#define NUMBER_OF_WORDS_BASIC_TEST 4
#define MAX_SIZE_OF_WORD 24
#define  NUM_OF_LETTERS_FOR_KEY 3
#define BITS_RANGE_OF_LETTERS 5
#define UNUSED_BITS ((1 << BITS_RANGE_OF_LETTERS) - 26)

static size_t HashFunctionForSpellChecker(const void *data)
{
	size_t key = 0, i = 0;
	char* string = (char *)data;
	char first_char = *string;
	size_t unused_bits = UNUSED_BITS;

	for ( ; *string && i < NUM_OF_LETTERS_FOR_KEY ; ++string, ++i)
	{
		key <<= BITS_RANGE_OF_LETTERS;
		key = key + ((tolower(*string) - 'a' + 1));
		key -= (i * unused_bits) * ((tolower(first_char) - 'a' + 1));
	}

	return (key % SPELL_CHECKER_LUT_SIZE);
}

static bool MatchForSpellChecker(void *data, void *param)
{
	return (EQUAL == strcmp((char *)data, (char *)param));
}

static hash_table_t *InitSpellCheckerBeforeTest(char **data)
{
	size_t i = 0;
	FILE *fptr;
	hash_table_t *hash = HashCreate(HashFunctionForSpellChecker,
		                          MatchForSpellChecker, SPELL_CHECKER_LUT_SIZE);

	if ((fptr = fopen("/usr/share/dict/american-english", "r")) == NULL)
	{
		printf("\nError! opening file\n");
		return(TEST_FAIL);
	}

	for ( ; i < NUMBER_OF_WORDS ; ++i)
	{
		data[i] = malloc(MAX_SIZE_OF_WORD);
		if (EOF == fscanf(fptr,"%s",  data[i]))
		{
			fclose(fptr);
			return (hash);
		}
		HashInsert(hash, data[i]);
	}

	fclose(fptr);
	return (hash);
}
/******************************************************************************/

TestResult BasicTestHashFunctionForSpellChecker()
{
	size_t i = 0;
	void *find_data = NULL;
	char *str[NUMBER_OF_WORDS_BASIC_TEST] = {"con", "hello", "Hi", "yeah"};

	/*printf("%lu\n", HashFunctionForSpellChecker(str[0]));*/
	hash_table_t *hash = HashCreate(HashFunctionForSpellChecker,
		                          MatchForSpellChecker, SPELL_CHECKER_LUT_SIZE);

	for ( ; i < NUMBER_OF_WORDS_BASIC_TEST; ++i)
	{
		HashInsert(hash, &str[i]);
	}

	REQUIRE(NUMBER_OF_WORDS_BASIC_TEST == HashCount(hash));

	for (i = 0 ; i < NUMBER_OF_WORDS_BASIC_TEST; ++i)
	{
		find_data = HashFind(hash, &str[i]);
		REQUIRE(TRUE == MatchInts(find_data, &str[i]));
	}

	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashFunctionForSpellChecker()
{
	size_t i = 0;
	void *find_data = NULL;
	char **data = malloc(NUMBER_OF_WORDS * sizeof(char *));
	hash_table_t *hash = InitSpellCheckerBeforeTest(data);

	REQUIRE(NUMBER_OF_WORDS == HashCount(hash));

	for (i = 0 ; i < NUMBER_OF_WORDS; ++i)
	{
		find_data = HashFind(hash, data[i]);
		REQUIRE(TRUE == MatchForSpellChecker(find_data, data[i]));
	}

	for (i = 0 ; i < NUMBER_OF_WORDS; ++i)
	{
		HashRemove(hash, data[i]);
	}

	REQUIRE(TRUE == HashIsEmpty(hash));
	REQUIRE(EMPTY == HashCount(hash));

	for (i = 0 ; i < NUMBER_OF_WORDS; ++i)
	{
		free(data[i]);
	}

	free(data);
	HashDestroy(hash);
	return (TEST_PASS);
}

TestResult TestHashStatisticsForSpellChecker()
{
	size_t i = 0;
	char **data = malloc(NUMBER_OF_WORDS * sizeof(char *));
	hash_table_t *hash = InitSpellCheckerBeforeTest(data);

	HashStatistics(hash);

	for (i = 0 ; i < NUMBER_OF_WORDS; ++i)
	{
		free(data[i]);
	}

	free(data);
	HashDestroy(hash);
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
