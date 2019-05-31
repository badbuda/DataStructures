/*******************************************************************************
* NAME: unique id Tests                                                        *
*                                                                              *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  21.11.18                                                      *
*******************************************************************************/

#include <stdlib.h> /* free */
#include <string.h> /* strcmp */
#include <time.h> /* time_t*/

#include "test.h"
#include "uid.h"

TestResult UIDCreateTest();
TestResult UIDIsSameTest();
TestResult UIDGetBadIsBadTest();
TestResult UIDToStringTest();

int main()
{
	RUN_TEST(UIDCreateTest);
	RUN_TEST(UIDIsSameTest);
	RUN_TEST(UIDGetBadIsBadTest);
	RUN_TEST(UIDToStringTest);
	return (0);
}
/******************************************************************************/

TestResult UIDCreateTest()
{
	unique_id_t uid1 = UIDCreate();
	unique_id_t uid2 = UIDCreate();

	REQUIRE(uid1.time > 0);
	REQUIRE(uid1.thread_id == pthread_self());
	REQUIRE(uid1.sequence == 0);

	REQUIRE(uid2.time > 0);
	REQUIRE(uid2.thread_id == pthread_self());
	REQUIRE(uid2.sequence == 1);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult UIDIsSameTest()
{
	unique_id_t uid1 = UIDCreate();
	unique_id_t uid2 = UIDCreate();

	REQUIRE(FALSE == UIDIsSame(uid1, uid2));
	REQUIRE(TRUE == UIDIsSame(uid1, uid1));

	return (TEST_PASS);
}
/******************************************************************************/

TestResult UIDGetBadIsBadTest()
{
	unique_id_t uid1 = UIDGetBad();
	unique_id_t uid2 = UIDGetBad();

	REQUIRE(uid1.time > time(NULL));
	REQUIRE(uid1.sequence == 0);

	REQUIRE(uid2.time > time(NULL));
	REQUIRE(uid2.sequence == -1);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult UIDToStringTest()
{
	char* str;
	unique_id_t uid = UIDCreate();
	str = UIDToString(uid);
	free(str);

	uid.time = 1542878041;
	uid.thread_id = 14204;
	uid.sequence = 6;
	str = UIDToString(uid);

	REQUIRE(0 == strcmp(str, "1542878041142046"));
	free(str);

	return (TEST_PASS);
}
/*******************************************************************************
*******************************************************************************/
