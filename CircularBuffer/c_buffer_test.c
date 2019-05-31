/*******************************************************************************
* NAME: Circular Buffer                                                        *
*                                                                              *
* PURPOSE: Circular Buffer Tests                                               *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  10.11.18                                                      *
*******************************************************************************/
#include <stdio.h>
#include <errno.h>  /* errno */
#include <string.h> /*  strcmp */
#include "test.h"
#include "c_buffer.h"

TestResult TestCBufferClear(void);
TestResult TestCBufferWrite(void);
TestResult TestCBufferRead(void);
TestResult TestCBufferCopy(void);

int main()
{
	RUN_TEST (TestCBufferClear);
	RUN_TEST (TestCBufferWrite);
	RUN_TEST (TestCBufferRead);
	RUN_TEST (TestCBufferCopy);

	return 0;
}

TestResult TestCBufferClear(void)
{
	c_buffer_t *buff = CBufferCreate(7);
	char src[50] = "123456789abxdefghijklmnopqrstuvwxyz";
/*	char dest[50] = "0000000000";*/

	REQUIRE(7u == CBufferGetCapacity(buff));

	REQUIRE(0 == CBufferGetSize(buff));
	CBufferWrite(buff, src, 7);
	REQUIRE(7u == CBufferGetSize(buff));
	CBufferWrite(buff, src, 7);
	REQUIRE(ENOBUFS == errno);
	CBufferClear(buff);
	REQUIRE(0 == CBufferGetSize(buff));
	REQUIRE(0 == CBufferWrite(buff, src, 0));
	CBufferWrite(buff, src, 2);
	REQUIRE(2u == CBufferGetSize(buff));
	CBufferWrite(buff, src, 5);
	REQUIRE(7u == CBufferGetSize(buff));
	CBufferClear(buff);
	REQUIRE(0 == CBufferGetSize(buff));

	CBufferDestroy(&buff);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestCBufferWrite(void)
{
	size_t i = 0;
	c_buffer_t *buff = CBufferCreate(7);
	char src[50] = "123456789abxdefghijklmnopqrstuvwxyz";
	char dest[50] = "0000000000";


	REQUIRE(0 == CBufferWrite(buff, src, 0));
	REQUIRE(7 == CBufferWrite(buff, src, 7));
	REQUIRE(7u == CBufferGetSize(buff));

	REQUIRE(-1 == CBufferWrite(buff, src, 1));
	REQUIRE(ENOBUFS == errno);
	REQUIRE(7u == CBufferGetSize(buff));

/*	REQUIRE(-1 == CBufferWrite(buff, src, 0));*/
	REQUIRE(7u == CBufferGetSize(buff));

	CBufferClear(buff);

	for(i=1 ; i < 8 ; ++i)
	{
		REQUIRE(1 == CBufferWrite(buff, src, 1));
		REQUIRE(i == CBufferGetSize(buff));
	}

	CBufferClear(buff);

	REQUIRE(4 == CBufferWrite(buff, src, 4));
	REQUIRE(4 == CBufferRead(buff, dest, 4));
	REQUIRE(7 == CBufferWrite(buff, src, 7));
	REQUIRE(7 == CBufferGetSize(buff));
	REQUIRE(-1 == CBufferWrite(buff, src, 7));
	REQUIRE(ENOBUFS == errno);

	REQUIRE(4 == CBufferRead(buff, dest, 4));
	REQUIRE(4 == CBufferWrite(buff, src, 100));
	REQUIRE(7 == CBufferGetSize(buff));

	CBufferDestroy(&buff);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestCBufferRead(void)
{
	size_t i = 0;
	c_buffer_t *buff = CBufferCreate(7);
	char src[50] = "123456789abxdefghijklmnopqrstuvwxyz";
	char dest[50] = "0000000000";

	CBufferWrite(buff, src, 7);
	REQUIRE(0 == CBufferRead(buff, dest, 0));

	for(i=1 ; i < 8 ; ++i)
	{
		REQUIRE(1 == CBufferRead(buff, dest, 1));
		REQUIRE(7-i == CBufferGetSize(buff));
	}

	REQUIRE(-1 == CBufferRead(buff, dest, 1));
	REQUIRE(ENODATA == errno);
	REQUIRE(0 == CBufferGetSize(buff));

	REQUIRE(4 == CBufferWrite(buff, src, 4));
	REQUIRE(4 == CBufferRead(buff, dest, 4));
	REQUIRE(7 == CBufferWrite(buff, src, 7));
	REQUIRE(7 == CBufferRead(buff, dest, 7));

/*	REQUIRE(-1 == CBufferRead(buff, dest, 0));*/
/*	REQUIRE(ENODATA == errno);*/

	REQUIRE(5 == CBufferWrite(buff, src, 5));
	REQUIRE(5 == CBufferRead(buff, dest, 100));

	CBufferDestroy(&buff);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestCBufferCopy(void)
{
	c_buffer_t *buff = CBufferCreate(7);
	char src[50] = "123456789abxdefghijklmnopqrstuvwxyz";
	char dest[50] = "0000000000";

	CBufferWrite(buff, src, 7);
	CBufferRead(buff, dest, 4);
/*	printf("%s\n",dest);*/
	REQUIRE(0 == strcmp(dest, "1234000000"));

	CBufferWrite(buff, src, 3);
	CBufferRead(buff, dest, 6);
/*	printf("%s\n",dest);*/
	REQUIRE(0 == strcmp(dest, "5671230000"));

	CBufferRead(buff, dest, 6);
/*	printf("%s\n",dest);*/
	REQUIRE(0 == strcmp(dest, "5671230000"));

	CBufferWrite(buff, src, 7);
	CBufferRead(buff, dest, 6);
/*	printf("%s\n",dest);*/
	REQUIRE(0 == strcmp(dest, "1234560000"));

	CBufferWrite(buff, src, 4);
	CBufferRead(buff, dest, 3);
/*	printf("%s\n",dest);*/
	REQUIRE(0 == strcmp(dest, "7124560000"));

	CBufferRead(buff, dest, 2);
/*	printf("%s\n",dest);*/
	REQUIRE(0 == strcmp(dest, "3424560000"));

	CBufferWrite(buff, src, 7);
	CBufferRead(buff, dest, 7);
/*	printf("%s\n",dest);*/
	REQUIRE(0 == strcmp(dest, "1234567000"));

	CBufferDestroy(&buff);

	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
