/*******************************************************************************
* NAME: DHCP                                                                   *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  07.01.18                                                      *
*******************************************************************************/
#include <stdlib.h> /* size_t, malloc, free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "dhcp.h"
#include "bit_array.h"

#define BASE_2 2
#define MAX_BITS 32
#define BITS_IN_BYTE 8
#define LAST_IP_FILED 3
#define MAX_NUM_IN_BYTE 255
#define NUMS_IN_BYTE 256
#define LEAF 0

typedef enum {LEFT, RIGHT, NUM_OF_CHILDREN} children;

typedef struct trie_node_s trie_node_t;

struct trie_node_s
{
		trie_node_t *children[NUM_OF_CHILDREN];
		size_t number_of_addresses;
};

struct dhcp_s
{
	trie_node_t *root;
	size_t cidr_mask;
	ip_address_t net_ip;
};
/**************************CREATE**********************************************/

static trie_node_t *TrieNodeCreate(size_t num_of_addresses)
{
	trie_node_t *node = malloc(sizeof(trie_node_t));

	if (NULL == node)
	{
		return (NULL);
	}

	node->children[LEFT] = NULL;
	node->children[RIGHT] = NULL;
	node->number_of_addresses = num_of_addresses;

	return (node);
}
dhcp_t *DhcpCreate(const ip_address_t network_ip, const size_t cidr_mask)
{
	dhcp_t *dhcp = NULL;
	ip_address_t gateway_ip = {0, 0, 0, 254};
	ip_address_t broadcast_ip = {255, 255, 255, 255};
	ip_address_t out_ip = {0, 0, 0, 0};

	assert(network_ip);

	dhcp = malloc(sizeof(dhcp_t));

	if (NULL == dhcp)
	{
		return (NULL);
	}

	dhcp->root = TrieNodeCreate(1 << (MAX_BITS - cidr_mask));

	if (NULL == dhcp->root)
	{
		return (NULL);
	}

	dhcp->cidr_mask = MAX_BITS - cidr_mask;

	memcpy(dhcp->net_ip, network_ip, sizeof(ip_address_t));

	/* create reserved ips*/
	if (SUCCESS != DhcpAllocIP(dhcp, out_ip, network_ip))
	{
		return (NULL);
	}

	if (SUCCESS != DhcpAllocIP(dhcp, out_ip, gateway_ip))
	{
		return (NULL);
	}

	if (SUCCESS != DhcpAllocIP(dhcp, out_ip, broadcast_ip))
	{
		return (NULL);
	}

	return (dhcp);
}
/***********************DESTROY************************************************/

static void DestroyAndFreeNodes(trie_node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	DestroyAndFreeNodes(node->children[LEFT]);
	node->children[LEFT] = NULL;

	DestroyAndFreeNodes(node->children[RIGHT]);
	node->children[RIGHT] = NULL;

	free(node);
}

void DhcpDestroy(dhcp_t *dhcp)
{
	assert(dhcp);

	DestroyAndFreeNodes (dhcp->root);

	dhcp->root = NULL;
	free(dhcp);
}
/************************COUNT*************************************************/

size_t DhcpCountFree(const dhcp_t *dhcp)
{
	assert(dhcp);

	return (dhcp->root->number_of_addresses);
}
/*****************ALLOC SUGGESTED IP*******************************************/

static void FindIpPath(dhcp_t *dhcp, unsigned long *bit_array)
{
	size_t i = 0;
	trie_node_t *node = dhcp->root;

	for (i = dhcp->cidr_mask; 0 < i; --i)
	{
		node = node->children[BitArrayIsOn(*bit_array, i)];

		if (NULL == node)
		{
			return;
		}
	}
	++(*bit_array);
	FindIpPath(dhcp, bit_array);
}

static trie_node_t *AllocSpecificIP(trie_node_t *root, size_t mask,
	unsigned *new_ip_suffix, unsigned long bit_array)
	{
		trie_node_t *node = NULL;

		if (NULL == root)
		{
			root = TrieNodeCreate(1 << mask);

			if (NULL == root)
			{
				return (NULL);
			}
		}

		if (0 == root->number_of_addresses)
		{
			return (NULL);
		}

		if (0 == mask)
		{
			root->number_of_addresses = 0;
			return (root);
		}

		node = AllocSpecificIP(root->children[BitArrayIsOn(bit_array, mask)],
		mask - 1, new_ip_suffix, bit_array);
		if (node)
		{
			root->children[BitArrayIsOn(bit_array, mask)] = node;
			--root->number_of_addresses;

			if (BitArrayIsOn(bit_array, mask))
			{
				*new_ip_suffix += (1 << (mask - 1));
			}

			return (root);
		}

		return (NULL);
	}
/*******************ALLOC IP***************************************************/
static trie_node_t *RecursiveAllocIp(trie_node_t *root, size_t mask,
                                     unsigned *new_ip_suffix)
{
	trie_node_t *node = NULL;

	if (NULL == root)
	{
		root = TrieNodeCreate(1 << mask);

		if (NULL == root)
		{
			return (NULL);
		}
	}

	if (0 == root->number_of_addresses)
	{
		return (NULL);
	}

	if (0 == mask)
	{
		root->number_of_addresses = 0;
		return (root);
	}

	node = RecursiveAllocIp(root->children[LEFT], mask - 1, new_ip_suffix);

	if (node)
	{
		root->children[LEFT] = node;
		--root->number_of_addresses;
		return (root);
	}

	node = RecursiveAllocIp(root->children[RIGHT], mask - 1, new_ip_suffix);

	if (node)
	{
		root->children[RIGHT] = node;
		*new_ip_suffix += (1 << (mask - 1));
		--root->number_of_addresses;
		return (root);
	}

	return (NULL);
}

/*******************ALLOC LEADING FUNCTION*************************************/

status DhcpAllocIP(dhcp_t *dhcp,
                   ip_address_t out_ip,
                   const ip_address_t suggested_ip)
{
	size_t i = 0;
	trie_node_t *node = NULL;
	unsigned new_ip_suffix = 0;

	assert(dhcp);
	assert(out_ip);

	if (0 == dhcp->root->number_of_addresses)
	{
		return (FAIL);
	}

	if (NULL != suggested_ip)
	{
		unsigned long bit_array = 0;
		unsigned long max_bit_array = 0;

		for ( ; i < dhcp->cidr_mask / BITS_IN_BYTE; ++i)
		{
			bit_array += (suggested_ip[LAST_IP_FILED - i] << (BITS_IN_BYTE * i));
			max_bit_array += MAX_NUM_IN_BYTE <<  (BITS_IN_BYTE * i);
		}

		FindIpPath(dhcp, &bit_array);

		if (bit_array > max_bit_array)
		{
			return (FAIL);
		}

		node = AllocSpecificIP(dhcp->root, dhcp->cidr_mask, &new_ip_suffix, bit_array);
	}

	else
	{
		node = RecursiveAllocIp(dhcp->root, dhcp->cidr_mask, &new_ip_suffix);
	}

 if (NULL == node)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	memcpy(out_ip, dhcp->net_ip, sizeof(ip_address_t));

	for(i = LAST_IP_FILED ; 0 < new_ip_suffix; --i, new_ip_suffix /= NUMS_IN_BYTE)
	{
			out_ip[i] = new_ip_suffix % NUMS_IN_BYTE;
	}

	return (SUCCESS);
}
/***********************REMOVE IP**********************************************/

static trie_node_t *FindIpAddress(dhcp_t *dhcp, const ip_address_t required_ip,
                                  unsigned long *bit_array)
{
	size_t i = 0;
	trie_node_t *node = dhcp->root;

	for ( ; i <= LAST_IP_FILED; ++i)
	{
		*bit_array += (required_ip[LAST_IP_FILED - i] << (BITS_IN_BYTE * i));
	}

	for (i = dhcp->cidr_mask; 0 < i; --i)
	{
		node = node->children[BitArrayIsOn(*bit_array, i)];

		if (NULL == node)
		{
			return (NULL);
		}
	}

	return (node);
}

static trie_node_t *RemoveAndFreeNode(trie_node_t *root, size_t mask,
	                          size_t step, unsigned long bit_array)
{
	size_t height = mask - step;

	if (LEAF == height)
	{
		free(root);
		return (NULL);
	}

	root->children[BitArrayIsOn(bit_array, height)] =
		RemoveAndFreeNode(root->children[BitArrayIsOn(bit_array, height)],
	                                        mask, step + 1, bit_array);
	++root->number_of_addresses;

	if (((size_t)(1 << height) == root->number_of_addresses) && (0 != step))
	{
		free(root);
		return (NULL);
	}

	return (root);
}

status DhcpFreeIP(dhcp_t *dhcp, const ip_address_t ip_to_free)
{
	trie_node_t *node = NULL;
	unsigned long bit_array = 0;

	assert(dhcp);
	assert(ip_to_free);

	node = FindIpAddress(dhcp, ip_to_free, &bit_array);

	if (NULL == node)
	{
		return (FAIL);
	}

	RemoveAndFreeNode(dhcp->root, dhcp->cidr_mask, 0, bit_array);

	return (SUCCESS);
}
/******************************************************************************/
/******************************************************************************/
