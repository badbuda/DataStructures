/*******************************************************************************
* NAME: Bit Array                                                              *
*                                                                              *
* PURPOSE: bit array manipulation functions                                    *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  01.11.18                                                      *
*******************************************************************************/
#include <stdio.h>
#include "test.h"
#include "bit_array.h"

TestResult BitArrayIsOnTest(void);
TestResult BitArrayIsOffTest(void);
TestResult BitArraySetOnTest(void);
TestResult BitArraySetOffTest(void);
TestResult BitArrayFlipTest(void);
TestResult BitArrayRotateRightTest(void);
TestResult BitArrayRotateLeftTest(void);
TestResult BitArrayMirrorTest(void);
TestResult BitArrayCountOnTest(void);
TestResult BitArrayCountOffTest(void);
TestResult BitArrayMirrorLUTTest(void);
TestResult BitArrayCountOnLUTTest(void);
TestResult BitArrayCountOffLUTTest(void);

int main()
{
	RUN_TEST (BitArrayIsOnTest);
	RUN_TEST (BitArrayIsOffTest);
	RUN_TEST (BitArraySetOnTest);
	RUN_TEST (BitArraySetOffTest);
	RUN_TEST (BitArrayFlipTest);
	RUN_TEST (BitArrayRotateRightTest);
	RUN_TEST (BitArrayRotateLeftTest);
	RUN_TEST (BitArrayMirrorTest);
	RUN_TEST (BitArrayCountOnTest);
	RUN_TEST (BitArrayCountOffTest);
	RUN_TEST (BitArrayMirrorLUTTest);
	RUN_TEST (BitArrayCountOnLUTTest);
	RUN_TEST (BitArrayCountOffLUTTest);

	return 0;
}

TestResult BitArrayIsOnTest(void)
{
	REQUIRE (BitArrayIsOn(30 , 1) == 0);
	REQUIRE (BitArrayIsOn(30 , 2) == 1);
	REQUIRE (BitArrayIsOn(30 , 5) == 1);
	REQUIRE (BitArrayIsOn(34, 2) == 1);
	REQUIRE (BitArrayIsOn(34, 4) == 0);
	REQUIRE (BitArrayIsOn(34, 6) == 1);
	REQUIRE (BitArrayIsOn(63, 3) == 1);
	REQUIRE (BitArrayIsOn(63, 7) == 0);
	REQUIRE (BitArrayIsOn(79, 6) == 0);
	REQUIRE (BitArrayIsOn(79, 7) == 1);
	REQUIRE (BitArrayIsOn(79, 64) == 0);

	return (TEST_PASS);
}

TestResult BitArrayIsOffTest(void)
{
	REQUIRE (BitArrayIsOff(30 , 1) == 1);
	REQUIRE (BitArrayIsOff(30 , 2) == 0);
	REQUIRE (BitArrayIsOff(30 , 5) == 0);
	REQUIRE (BitArrayIsOff(34, 2) == 0);
	REQUIRE (BitArrayIsOff(34, 4) == 1);
	REQUIRE (BitArrayIsOff(34, 6) == 0);
	REQUIRE (BitArrayIsOff(63, 3) == 0);
	REQUIRE (BitArrayIsOff(63, 7) == 1);
	REQUIRE (BitArrayIsOff(79, 6) == 1);
	REQUIRE (BitArrayIsOff(79, 7) == 0);
	REQUIRE (BitArrayIsOff(79, 64) == 1);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult BitArraySetOnTest(void)
{

/*	printf("%lu", BitArraySetOn(0 , 0));*/

	REQUIRE (BitArraySetOn(1 , 1) == 1);
	REQUIRE (BitArraySetOn(1 , 2) == 3);
	REQUIRE (BitArraySetOn(5, 2) == 7);
	REQUIRE (BitArraySetOn(19 , 4) == (19 + 8));

	return (TEST_PASS);
}

TestResult BitArraySetOffTest(void)
{

/*	printf("%lu", BitArraySetOff(0 , 0));*/

	REQUIRE (BitArraySetOff(1 , 1) == 0);
	REQUIRE (BitArraySetOff(1 , 2) == 1);
	REQUIRE (BitArraySetOff(5, 1) == 4);
	REQUIRE (BitArraySetOff(5, 2) == 5);
	REQUIRE (BitArraySetOff(5, 3) == 1);
	REQUIRE (BitArraySetOff(19 , 5) == (19 - 16));

	return (TEST_PASS);

}
/******************************************************************************/
TestResult BitArrayFlipTest(void)
{

/*	printf("%lu", BitArrayFlip(0 , 11));*/
	REQUIRE (BitArrayFlip(0 , 11) == 1024);
	REQUIRE (BitArrayFlip(1 , 1) == 0);
	REQUIRE (BitArrayFlip(1 , 2) == 3);
	REQUIRE (BitArrayFlip(5, 1) == 4);
	REQUIRE (BitArrayFlip(5, 2) == 7);
	REQUIRE (BitArrayFlip(5, 3) == 1);
	REQUIRE (BitArrayFlip(19 , 4) == 27);
	REQUIRE (BitArrayFlip(19 , 5) == 3);

	return (TEST_PASS);
}
/******************************************************************************/


TestResult BitArrayRotateRightTest(void)
{

/*	printf("%lu", BitArrayFlip(0 , 11));*/
	REQUIRE (BitArrayRotateRight(0 , 11) == 0);
	REQUIRE (BitArrayRotateRight(1 , 63) == 2);
	REQUIRE (BitArrayRotateRight(1 , 64) == 1);
	REQUIRE (BitArrayRotateRight(4, 1) == 2);
	REQUIRE (BitArrayRotateRight(4, 2) == 1);
	REQUIRE (BitArrayRotateRight(12, 1) == 6);
	REQUIRE (BitArrayRotateRight(12 , 2) == 3);
	REQUIRE (BitArrayRotateRight(19 , 63) == 38);

	return (TEST_PASS);
}


TestResult BitArrayRotateLeftTest(void)
{

/*	printf("%lu", BitArrayFlip(0 , 11));*/
	REQUIRE (BitArrayRotateLeft(0 , 11) == 0);
	REQUIRE (BitArrayRotateLeft(1 , 63) == 9223372036854775808u);
	REQUIRE (BitArrayRotateLeft(1 , 64) == 1);
	REQUIRE (BitArrayRotateLeft(4, 1) == 8);
	REQUIRE (BitArrayRotateLeft(4, 2) == 16);
	REQUIRE (BitArrayRotateLeft(12, 1) == 24);
	REQUIRE (BitArrayRotateLeft(11 , 2) == 44);
	REQUIRE (BitArrayRotateLeft(9223372036854775808u , 2) == 2);
	REQUIRE (BitArrayRotateLeft(16140901064495857665u , 4) == (16+8+4+2));
	REQUIRE (BitArrayRotateLeft(4294967295 , 5) == 137438953440);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult BitArrayMirrorTest(void)
{
/*printf("%lu\n",BitArrayMirror(3));*/
/*printf("%lu\n",BitArrayMirror(13835058055282163712));*/

/*printf("%lu\n",BitArrayMirror(18446744073709551615));*/

/*printf("%lu\n",BitArrayMirror(0));*/

/*printf("%lu\n",BitArrayMirror(18446744069414584320));*/
/*printf("%lu\n",BitArrayMirror(4294967295));*/

	REQUIRE (BitArrayMirror(3) == 13835058055282163712u);
	REQUIRE (BitArrayMirror(13835058055282163712u) == 3);
	REQUIRE (BitArrayMirror(18446744073709551615u) == 18446744073709551615u);
	REQUIRE (BitArrayMirror(0) == 0);
	REQUIRE (BitArrayMirror(18446744069414584320u) == 4294967295);
	REQUIRE (BitArrayMirror(4294967295) == 18446744069414584320u);
	REQUIRE (BitArrayMirror(1) == 9223372036854775808u);

	return (TEST_PASS);
}

TestResult BitArrayMirrorLUTTest(void)
{
/*printf("%lu\n",BitArrayMirror(3));*/
/*printf("%lu\n",BitArrayMirror(13835058055282163712));*/

/*printf("%lu\n",BitArrayMirror(18446744073709551615));*/

/*printf("%lu\n",BitArrayMirror(0));*/

/*printf("%lu\n",BitArrayMirror(18446744069414584320));*/
/*printf("%lu\n",BitArrayMirror(4294967295));*/

	REQUIRE (BitArrayMirrorLUT(3) == 13835058055282163712u);
	REQUIRE (BitArrayMirrorLUT(13835058055282163712u) == 3);
	REQUIRE (BitArrayMirrorLUT(18446744073709551615u) == 18446744073709551615u);
	REQUIRE (BitArrayMirrorLUT(0) == 0);
	REQUIRE (BitArrayMirrorLUT(18446744069414584320u) == 4294967295);
	REQUIRE (BitArrayMirrorLUT(4294967295) == 18446744069414584320u);
	REQUIRE (BitArrayMirrorLUT(1) == 9223372036854775808u);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult BitArrayCountOnTest(void)
{
	REQUIRE (BitArrayCountOn(1) == 1);
	REQUIRE (BitArrayCountOn(3) == 2);
	REQUIRE (BitArrayCountOn(7) == 3);
	REQUIRE (BitArrayCountOn(9) == 2);
	REQUIRE (BitArrayCountOn(13) == 3);

	return (TEST_PASS);
}

TestResult BitArrayCountOffTest(void)
{

	REQUIRE (BitArrayCountOff(1) == 64-1);
	REQUIRE (BitArrayCountOff(3) == 64-2);
	REQUIRE (BitArrayCountOff(7) == 64-3);
	REQUIRE (BitArrayCountOff(9) == 64-2);
	REQUIRE (BitArrayCountOff(13) == 64-3);

	return (TEST_PASS);
}

TestResult BitArrayCountOnLUTTest(void)
{
	REQUIRE (BitArrayCountOnLUT(1) == 1);
	REQUIRE (BitArrayCountOnLUT(3) == 2);
	REQUIRE (BitArrayCountOnLUT(7) == 3);
	REQUIRE (BitArrayCountOnLUT(9) == 2);
	REQUIRE (BitArrayCountOnLUT(13) == 3);

	return (TEST_PASS);
}

TestResult BitArrayCountOffLUTTest(void)
{

	REQUIRE (BitArrayCountOffLUT(1) == 64-1);
	REQUIRE (BitArrayCountOffLUT(3) == 64-2);
	REQUIRE (BitArrayCountOffLUT(7) == 64-3);
	REQUIRE (BitArrayCountOffLUT(9) == 64-2);
	REQUIRE (BitArrayCountOffLUT(13) == 64-3);

	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
