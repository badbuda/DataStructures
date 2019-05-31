/*******************************************************************************
* NAME: unique id C file                                                       *
*                                                                              *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  21.11.18                                                      *
*******************************************************************************/
#include <stdio.h> /* sizeof */
#include <stdlib.h> /* malloc */
#include <time.h> /* time_t, time*/
#include <assert.h>/* assert */
#include "uid.h"

/******************************************************************************/

unique_id_t UIDCreate()
{
	static int counter = 0;

	unique_id_t uid;

	uid.time = time(NULL);
	uid.thread_id = pthread_self();
	uid.sequence = counter;
	++counter;

	return (uid);
}
/******************************************************************************/

bool UIDIsSame(const unique_id_t uid1, const unique_id_t uid2)
{
	if (uid1.time != uid2.time || uid1.thread_id != uid2.thread_id ||
		uid1.sequence != uid2.sequence)
	{
		return (FALSE);
	}
	else
	{
		return (TRUE);
	}
}
/******************************************************************************/

bool UIDIsBad(const unique_id_t uid)
{
	if (uid.time < 0 || uid.time > time(NULL) || uid.sequence < 0)
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
}
/******************************************************************************/

unique_id_t UIDGetBad()
{
	static int bad_counter = 0;

	unique_id_t uid;

	uid.time = 2 * time(NULL);
	uid.sequence = bad_counter;
	--bad_counter;

	return (uid);
}
/******************************************************************************/

char *UIDToString(const unique_id_t uid)
{
	char* str = malloc(sizeof(char) * 30);

/*	printf("Current date and time is: %s\n", asctime(localtime(&uid.time)));*/
	sprintf(str, "%lu%ld%d", uid.time, uid.thread_id, uid.sequence);

	return (str);
}
/*******************************************************************************
*******************************************************************************/
