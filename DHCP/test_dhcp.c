/*******************************************************************************
* NAME: DHCP TEST                                                              *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  07.01.18                                                      *
*******************************************************************************/
#include <stdlib.h> /* size_t */

#include "test.h"
#include "dhcp.h"

TestResult TestDhcpCreateDestroy();
TestResult TestDhcpCountFree();
TestResult TestDhcpAllocIP();
TestResult TestDhcpFreeIP();
TestResult TestSuggestedAllocIP();
/******************************************************************************/

int main()
{
	RUN_TEST(TestDhcpCreateDestroy);
	RUN_TEST(TestDhcpCountFree);
	RUN_TEST(TestDhcpAllocIP);
	RUN_TEST(TestDhcpFreeIP);
	RUN_TEST(TestSuggestedAllocIP);

return (0);
}
/******************************************************************************/

TestResult TestDhcpCreateDestroy()
{
	ip_address_t net_ip = {10, 1, 0, 0};
	size_t cidr_mask = 16;
	dhcp_t *dhcp = DhcpCreate(net_ip, cidr_mask);
	DhcpDestroy(dhcp);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestDhcpCountFree()
{
	ip_address_t net_ip = {10, 1, 0, 0};
	size_t cidr_mask = 24;
	dhcp_t *dhcp = DhcpCreate(net_ip, cidr_mask);

	/*printf("%lu\n", DhcpCountFree(dhcp));*/
	REQUIRE((size_t)((1 << (32 - cidr_mask)) - 3) == DhcpCountFree(dhcp));

	DhcpDestroy(dhcp);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestDhcpAllocIP()
{
	int i = 0;
	ip_address_t net_ip = {10, 0, 0, 0};
	ip_address_t out_ip1 = {0, 0, 0, 0};
	size_t cidr_mask = 24;
	size_t count = 0;
	dhcp_t *dhcp = DhcpCreate(net_ip, cidr_mask);

	count = DhcpCountFree(dhcp);

	for ( ; i < (1 << (32 - cidr_mask)) - 3; ++i)
	{
		REQUIRE(SUCCESS == DhcpAllocIP(dhcp, out_ip1, NULL));
		--count;
		REQUIRE(count == DhcpCountFree(dhcp));
		/*printf("%d.%d.%d.%d\n", out_ip1[0], out_ip1[1], out_ip1[2], out_ip1[3]);*/
	}

	REQUIRE(FAIL == DhcpAllocIP(dhcp, out_ip1, NULL));

	DhcpDestroy(dhcp);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestDhcpFreeIP()
{
	int i = 0, j = 0;
	size_t cidr_mask = 24;
	ip_address_t net_ip = {10, 1, 0, 0};
	ip_address_t out_ip_array[256];
	size_t count = 0;
	dhcp_t *dhcp = DhcpCreate(net_ip, cidr_mask);

	for (i = 0 ; i < 256; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			out_ip_array[i][j] = 0;
		}
	}

	for (i = 0 ; i < (1 << (32 - cidr_mask)) - 3; ++i)
	{
		DhcpAllocIP(dhcp, out_ip_array[i], NULL);
	}

	count = DhcpCountFree(dhcp);
	for (i = 0 ; i < (1 << (32 - cidr_mask)) - 3; ++i)
	{
		DhcpFreeIP(dhcp, out_ip_array[i]);
		++count;
		REQUIRE(count == DhcpCountFree(dhcp));
		/*printf("%d.%d.%d.%d\n", out_ip_array[i][0], out_ip_array[i][1],
		                          out_ip_array[i][2], out_ip_array[i][3]);*/
	}

	DhcpDestroy(dhcp);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestSuggestedAllocIP()
{
	int i = 0;
	size_t cidr_mask = 24;
	ip_address_t net_ip = {10, 1, 0, 0};
	ip_address_t out_ip1 = {0, 0, 0, 0};
	ip_address_t suggested_ip = {10, 1, 0, 250};

	dhcp_t *dhcp = DhcpCreate(net_ip, cidr_mask);

	for (i = 0; i < 4; ++i)
	{
		REQUIRE(SUCCESS == DhcpAllocIP(dhcp, out_ip1, suggested_ip));
		/*printf("%d.%d.%d.%d\n", out_ip1[0], out_ip1[1], out_ip1[2], out_ip1[3]);*/
	}

	REQUIRE(FAIL == DhcpAllocIP(dhcp, out_ip1, suggested_ip));

	DhcpDestroy(dhcp);
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
