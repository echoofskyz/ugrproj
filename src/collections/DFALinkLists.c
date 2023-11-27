#include "VPLists.h"
#include "DFALinkLists.h"
#include <stdlib.h>

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

static char next(List* list)
{
	return *(char*)VPLists.next(list);
	
}

static char prev(List* list)
{
	return *(char*)VPLists.prev(list);
}

static char get(List* list, int index)
{
	return *(char*)VPLists.get(list, index);
}

static void push(List* list, char data)
{
	char* dataPtr = malloc(sizeof(char));
	*dataPtr = data;
	VPLists.push(list, (void*)dataPtr);
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

static void delNode(List* list, ListNode* node)
{
	free(node->data);
	VPLists.delNode(list, node);
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

static void resetCursor(List* list)
{
	VPLists.resetCursor(list);
}
*/

const struct DFALinkLists DFALinkLists = {
	/*
	.pop = pop,
	.popFront = popFront,
	.next = next,
	.prev = prev,
	.get = get,
	.push = push,
	.pushFront = pushFront,
	.del = del,
	.delAll = delAll,
	.insert = insert,
	.set = set,
	.resetCursor = resetCursor
	*/
};
