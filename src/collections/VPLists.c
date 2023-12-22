// definitions for void pointer list manager

#include "VPLists.h"
#include <stdlib.h>
#include <stdio.h>

static void* pop(List* list) {
	// pops element from back of listy
	
	if (list->size == 0) {
		return NULL;
	}
	
	void* data = list->tail->data;
	
	if (list->size == 1) {
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->cursor = NULL;
		list->size--;
		return data;
	}
	
	ListNode* target = list->tail;
	
	// if the cursor is on this node
	//   move it to the previous one
	if (list->cursor == target) {
		list->cursor = target->prev;
	}
	
	target->prev->next = NULL;
	list->tail = target->prev;
	free(target);
	list->size--;
	
	return data;
}

static void* popFront(List* list) {
	// pops element from front of list
	
	if (list->size == 0) {
		return NULL;
	}
	
	void* data = list->head->data;
	
	if (list->size == 1) {
		free(list->tail);
		list->head = NULL;
		list->tail = NULL;
		list->cursor = NULL;
		list->size--;
		return data;
	}
	
	ListNode* target = list->head;
	
	// if the cursor is on this node
	//   move it to the next one
	if (list->cursor == target) {
		list->cursor = target->next;
	}
	
	target->next->prev = NULL;
	list->head = target->next;
	free(target);
	list->size--;
	
	return data;
}

static void* next(List* list) {
	// moves cursor to next node and returns nodes data
	
	// cursor uninitialized, start at head
	if (list->cursor == NULL) {
		list->cursor = list->head;
		return list->head->data;
	}

	// no next element (cursor at tail), return null
	if (list->cursor->next == NULL) {
		return NULL;
	}

	list->cursor = list->cursor->next;
	return list->cursor->data;
}

static void* prev(List* list) {
	// moves cursor to previous node and returns nodes data
	
	// cursor uninitialized, start at tail
	if (list->cursor == NULL) {
		list->cursor = list->tail;
		return list->cursor->data;
	}

	// no previous element (cursor at head), return null
	if (list->cursor->prev == NULL) {
		return NULL;
	}

	list->cursor = list->cursor->prev;
	return list->cursor->data;
}

static void* get(List* list, int index) {
	// returns the data in the given list at the given index
	
	if (index < 0 || index >= list->size) {
		return NULL;
	}
	
	ListNode* n;
	
	// depending on which half the list the index is
	//   in start at either the head or the tail
	if (index < (list->size >> 1)) {
		n = list->head;
		
		for (int i=0;i<index;i++) {
			n = n->next;
		}
	} else {
		n = list->tail;
		
		for (int i=list->size-1;i>index;i--) {
			n = n->prev;
		}
	}
	
	return n->data;
}

static void push(List* list, void* data) {
	// pushes element to back of list
	
	ListNode* n = malloc(sizeof(ListNode));
	n->data = data;
	n->prev = NULL;
	n->next = NULL;
	
	if (list->size == 0) {
		list->head = n;
		list->tail = n;
		list->size++;
		return;
	}
	
	list->tail->next = n;
	n->prev = list->tail;
	list->tail = n;
	list->size++;
}

static void pushFront(List* list, void* data) {
	// pushes element to front of list
	
	ListNode* n = malloc(sizeof(ListNode));
	n->data = data;
	n->prev = NULL;
	n->next = NULL;
	
	if (list->size == 0) {
		list->head = n;
		list->tail = n;
		list->size++;
		return;
	}
	
	list->head->prev = n;
	n->next = list->head;
	list->head = n;
}

static void del(List* list, int index) {
	// deletes element at given index
	
	// index out of bounds, do nothing
	if (list->size == 0
			|| index >= list->size
			|| index < 0) {
		return;
	}

	// index is the tail node, so just use pop
	if (index == list->size - 1) {
		VPLists.pop(list);
		
		return;
	}
	
	// deleting head node
	if (index == 0) {
		ListNode* oldHead = list->head;
		list->head = oldHead->next;
		list->head->prev = NULL;
		
		// if the cursor is on the old head node, move it to the new one
		if (list->cursor == oldHead) {
			list->cursor == list->head;
		}
		
		free(oldHead);
		list->size--;

		return;
	}
	
	// target node is between to others
	ListNode* n;
	
	// depending on which half the list the index is
	//   in start at either the head or the tail
	if (index < (list->size >> 1)) {
		n = list->head;
		
		for (int i=0;i<index;i++) {
			n = n->next;
		}
	} else {
		n = list->tail;
		
		for (int i=list->size-1;i>index;i--) {
			n = n->prev;
		}
	}
	
	// if the cursor is on the target node, move it
	if (list->cursor == n) {
		list->cursor = n->next;
	}
	
	n->next->prev = n->prev;
	n->prev->next = n->next;
	free(n);
	list->size--;
}

static void delNode(List* list, ListNode* node) {
	// deletes a given node from the list
	if (node == list->tail) {
		VPLists.pop(list);
		return;
	}
	
	if (node == list->head) {
		VPLists.popFront(list);
		return;
	}
	
	node->prev->next = node->next;
	node->next->prev = node->prev;
	
	free(node);
	list->size--;
}

static void delAll(List* list) {
	// removes/frees all elements in list
	
	for (int i=list->size;i>0;i--) {
		VPLists.pop(list);
	}
}

static void insert(List* list, void* data, int index) {
	// inserts element at given index
	
	// out of bounds do nothing
	if (index < 0 || index > list->size) {
		return;
	}
	
	// inserting to tail, use push
	if (index == list->size) {
		VPLists.push(list, data);
		return;
	}
	
	// inserting to head, use pushFront
	if (index == 0) {
		VPLists.pushFront(list, data);
		return;
	}
	
	// target node is between two other nodes
	ListNode* new = malloc(sizeof(ListNode));
	new->data = data;
	
	ListNode* n;
	
	// depending on which half the list the index is
	//   in start at either the head or the tail
	if (index < (list->size >> 1)) {
		n = list->head;
		
		for (int i=0;i<index;i++) {
			n = n->next;
		}
	} else {
		n = list->tail;
		
		for (int i=list->size-1;i>index;i--) {
			n = n->prev;
		}
	}
	
	new->prev = n->prev;
	new->next = n;
	n->prev = new;
	new->prev->next = n;

	list->size++;
}

static void set(List* list, int index, void* data) {
	// sets the data in the given list at the given index
	
	// if index is out of bounds do nothing
	if (index < 0 || index >= list->size) {
		return;
	}
	
	ListNode* n;
	
	// depending on which half the list the index is
	//   in start at either the head or the tail
	if (index < (list->size >> 1)) {
		n = list->head;
		
		for (int i=0;i<index;i++) {
			n = n->next;
		}
	} else {
		n = list->tail;
		
		for (int i=list->size-1;i>index;i--) {
			n = n->prev;
		}
	}
	
	n->data = data;
}

static void resetCursor(List* list)
{
	// resets the cursor	
	list->cursor = NULL;
}

const struct VPLists VPLists = {
	.pop = pop,
	.popFront = popFront,
	.next = next,
	.prev = prev,
	.get = get,
	.push = push,
	.pushFront = pushFront,
	.del = del,
	.delAll = delAll,
	.delNode = delNode,
	.insert = insert,
	.set = set,
	.resetCursor = resetCursor
};
