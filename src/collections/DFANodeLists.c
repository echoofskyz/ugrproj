#include "VPLists.h"
#include "DFANodeLists.h"
#include <stdlib.h>

static void push(List* list, int n, float x, float y)
{
	DFANode* dataPtr = malloc(sizeof(DFANode));
	dataPtr->n = n;
	dataPtr->x = x;
	dataPtr->y = y;
	VPLists.push(list, (void*)dataPtr);
}

static DFANode* next(List* list)
{
	return (DFANode*)VPLists.next(list);
	
}

static void resetCursor(List* list)
{
	VPLists.resetCursor(list);
}

static DFANode* pop(List* list)
{
	return (DFANode*)VPLists.pop(list);
}

static void delAll(List* list)
{
	for (int i=list->size;i>0;i--) {
		DFANode* ptr = DFANodeLists.pop(list);
		free(ptr);
	}
}

/*


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

const struct DFANodeLists DFANodeLists = {
	.push = push,
	.next = next,
	.delAll = delAll,
	.pop = pop,
	.resetCursor = resetCursor
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
