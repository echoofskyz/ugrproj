#include "VPLists.h"
#include "DFALinkLists.h"
#include <stdlib.h>

static void push(List* list, DFANode* one, DFANode* two)
{
	DFALink* dataPtr = malloc(sizeof(DFALink));
	dataPtr->first = one;
	dataPtr->second = two;
	dataPtr->transitions = newList;
	VPLists.push(list, (void*)dataPtr);
}

static DFALink* next(List* list)
{
	return (DFALink*)VPLists.next(list);
	
}

static DFALink* peek(List* list)
{
	return (DFALink*)list->tail->data;
	
}

static void delNode(List* list, DFALink* node)
{
	ListNode* curs = list->head;
	for (int i=0; i < list->size; i++)
	{
		if ((DFALink*)(curs->data) == node)
		{
			break;
		}
		curs = curs->next;
	}
	
	free(curs->data);
	VPLists.delNode(list, curs);
}

static void resetCursor(List* list)
{
	VPLists.resetCursor(list);
}

static DFALink* pop(List* list)
{
	return (DFALink*)VPLists.pop(list);
}

static void delAll(List* list)
{
	for (int i=list->size;i>0;i--) {
		DFALink* ptr = DFALinkLists.pop(list);
		CharLists.delAll(&ptr->transitions);
		free(ptr);
	}
}

/*
static char pop(List* list)
{
	void* dataPtr = VPLists.pop(list);
	char data = *(char*)dataPtr;
	free(dataPtr);
	return data;
}

static char popFront(List* list)
{
	void* dataPtr = VPLists.popFront(list);
	char data = *(char*)dataPtr;
	free(dataPtr);
	return data;
}

static char prev(List* list)
{
	return *(char*)VPLists.prev(list);
}

static char get(List* list, int index)
{
	return *(char*)VPLists.get(list, index);
}



static void pushFront(List* list, char data)
{
	char* dataPtr = malloc(sizeof(char));
	*dataPtr = data;
	VPLists.pushFront(list, (void*)dataPtr);
}

static void del(List* list, int index)
{
	free(VPLists.get(list, index));
	VPLists.del(list, index);
}

static void delAll(List* list)
{
	for (int i=list->size;i>0;i--) {
		DFANodeLists.pop(list);
	}
}

static void insert(List* list, char data, int index)
{
	char* dataPtr = malloc(sizeof(char));
	*dataPtr = data;
	VPLists.insert(list, (void*)dataPtr, index);
}

static void set(List* list, int index, char data)
{
	char* dataPtr = malloc(sizeof(char));
	*dataPtr = data;
	VPLists.set(list, index, (void*)dataPtr);
}
*/

const struct DFALinkLists DFALinkLists = {
	.push = push,
	.pop = pop,
	.peek = peek,
	.delAll = delAll,
	.next = next,
	.delNode = delNode,
	.resetCursor = resetCursor
	/*
	
	.popFront = popFront,
	
	.prev = prev,
	.get = get,
	
	.pushFront = pushFront,
	.del = del,
	
	.insert = insert,
	.set = set,
	
	*/
};
