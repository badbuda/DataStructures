/*******************************************************************************
* NAME: Singly Linked List                                                     *
*                                                                              *
* PURPOSE: Singly Linked List                                                  *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  13.11.18                                                      *
*******************************************************************************/
#include <stdio.h>  /* size_t , sizeof */
#include <stdlib.h> /* malloc, free */
#include <assert.h>	/* assert */
#include "s_linked_list.h"

/******************************************************************************/

sll_node_t *SLinkedListCreate(void *data, sll_node_t *next)
{
	sll_node_t *sll_node_var = malloc(sizeof(sll_node_t));
	
	if (NULL == sll_node_var)
	{
		return (NULL);
	}
	
	sll_node_var->data = data;
	sll_node_var->next = next;
	
	return(sll_node_var);
}
/******************************************************************************/

void SLinkedListDestroy(sll_node_t **head)
{
	sll_node_t *next = NULL;
	assert(*head);	
	
	while (*head)
	{
		next = (*head)->next;
		free (*head);
		*head = NULL;
		*head = next;
	}
}
/******************************************************************************/

sll_node_t *SLinkedListInsertAfter(sll_node_t *where, sll_node_t *new_node)
{
	assert(where);
	assert(new_node);
	
	new_node->next = where->next;
	where->next = new_node;
	
	return (new_node);
}

sll_node_t *SLinkedListInsert(sll_node_t *where, sll_node_t *new_node)
{
	void *tmp_data = NULL;
	assert(where);
	assert(new_node);
	
	tmp_data = new_node->data;

	SLinkedListInsertAfter(where, new_node);
	
	new_node->data = where->data;
	where->data = tmp_data;

	return (where);	
}
/******************************************************************************/

sll_node_t *SLinkedListRemoveAfter(sll_node_t *where)
{
	sll_node_t *removed_node = NULL;
	assert(where);
	
	removed_node = where->next;
	where->next = where->next->next;
	removed_node->next = NULL;
	return (removed_node);
}

sll_node_t *SLinkedListRemove(sll_node_t *where)
{
	void *tmp_data = NULL;
	sll_node_t *removed_node = NULL;
	assert(where);
	
	removed_node = where->next;
	tmp_data = where->data;
	where->data = where->next->data;
	where->next = where->next->next;
	removed_node->next = NULL;
	removed_node->data = tmp_data;
	return (removed_node);
}
/******************************************************************************/

size_t SLinkedListGetSize(const sll_node_t *head)
{
	size_t counter = 0;
	assert(head);
	
	while (head)
	{
		head = head->next;
		++counter;
	}
	
	return (counter);
}
/******************************************************************************/

sll_node_t *SLinkedListIntersection(sll_node_t *head1, sll_node_t *head2)
{
	size_t l1 = 0;
	size_t l2 = 0;
	size_t diff = 0;
	size_t i = 0;
	
	assert(head1);
	assert(head2);
	
	l1 = SLinkedListGetSize(head1);
	l2 = SLinkedListGetSize(head2);
	
	if (l1 >= l2)
	{
		diff = l1 - l2;
		
		for ( ; i < diff ; ++i)
		{
			head1 = head1->next;
		}
	} 
	else
	{
		diff = l2 - l1;
		
		for ( ; i < diff ; ++i)
		{
			head2 = head2->next;
		}
	}
	
	while ((head1 != head2) && head1 && head2)
	{
		head1 = head1->next;
		head2 = head2->next;
	}
	
	if (head1 && head2)
	{
		return (head1);
	}
	
	return (NULL);
}
/******************************************************************************/

bool SLinkedListHasLoop(sll_node_t *head)
{
	sll_node_t *slow = NULL;
	sll_node_t *fast = NULL;
	assert(head);
	
	slow = head;
	fast = head;
	
	while (slow && fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;

		if (slow == fast)
		{
			return (TRUE);
		}
	}
	
	return (FALSE);
}
/******************************************************************************/

sll_node_t *SLinkedListFlip(sll_node_t *head)
{
	sll_node_t *new_head = NULL;
	sll_node_t *tmp_head = NULL;
	assert(head);
		
	if (head->next == NULL)
	{
		return (head);
	}
	
	new_head = head->next;
	tmp_head = new_head->next;
	head->next = NULL;
	
	while (tmp_head)
	{
		new_head->next = head;
		head = new_head;
		new_head = tmp_head;
		tmp_head = new_head->next;
	}
	new_head->next = head;
	
	return (new_head);
}
/******************************************************************************/

sll_node_t *SLinkedListFind(sll_node_t *head, match_function_t match,
														 void *param)
{
	assert(head);
	assert(match);
	
	while(head)
	{
		if(TRUE == match(head->data, param))
		{
			return(head);
		}
		
		head = head->next;
	}
	
	return(NULL);
}
/******************************************************************************/

status SLinkedListForEach(sll_node_t *head, action_function_t action,
														 void *param)
{
	assert(head);
	assert(action);
	
	while (head)
	{
		if (ACTION_FAIL == action(head->data, param))
		{
			return (ACTION_FAIL);
		}
		
		head = head->next;
	}
	
	return (SUCCESS);
}
/******************************************************************************/
/******************************************************************************/
