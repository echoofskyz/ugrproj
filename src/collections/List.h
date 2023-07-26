// List type definition

#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct ListNode {
	struct ListNode* prev;
	struct ListNode* next;
	void* data;
} ListNode;

typedef struct List {
	unsigned int size;
	ListNode* head;
	ListNode* tail;
	ListNode* cursor;
} List;

extern const List newList;

#endif