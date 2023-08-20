#include "../src/collections/VPLists.h"
#include <stdio.h>
#include <assert.h>

void display_List(List* list) {
	printf("List %p:\n", list);
	for (int i=0;i<list->size;i++) {
		printf("  Val %d: %p\n", i, VPLists.get(list, i));
	}
}

void test_push_to_empty() {
	printf("Testing pushing to empty List.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	
	assert(test.size == 1);
	assert(test.head == test.tail);
	assert(test.head->data == (void*)0x1234);
	assert(test.head->next == NULL);
	assert(test.head->prev == NULL);
	
	printf("  Test passed.\n");
}

void test_push() {
	printf("Testing pushing to non empty List.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	VPLists.push(&test, (void*)0x5678);
	
	assert(test.size == 2);
	
	assert(test.head->data == (void*)0x1234);
	assert(test.head->next == test.tail);
	assert(test.head->prev == NULL);
	
	assert(test.tail->data == (void*)0x5678);
	assert(test.tail->prev == test.head);
	assert(test.tail->next == NULL);
	
	printf("  Test passed.\n");
}

void test_pop_empty() {
	printf("Testing popping from empty list.\n");
	
	List test = newList;

	assert(VPLists.pop(&test) == NULL);
	
	printf("  Test passed.\n");
}

void test_pop_one() {
	printf("Testing popping from one element list.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);

	assert(VPLists.pop(&test) == (void*)0x1234);
	assert(test.size == 0);
	assert(test.head == NULL);
	assert(test.tail == NULL);
	
	printf("  Test passed.\n");
}

void test_pop_multi() {
	printf("Testing popping from multi element list.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	VPLists.push(&test, (void*)0x5678);

	assert(VPLists.pop(&test) == (void*)0x5678);
	assert(test.size == 1);
	assert(test.head == test.tail);
	
	printf("  Test passed.\n");
}

void test_del_empty() {
	printf("Testing deleting from empty list.\n");
	
	List test = newList;

	VPLists.del(&test, 0);
	assert(test.size == 0);
	assert(test.head == NULL);
	assert(test.tail == NULL);
	
	printf("  Test passed.\n");
}

void test_del_one() {
	printf("Testing deleting from one element list.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	VPLists.del(&test, 0);
	
	assert(test.size == 0);
	assert(test.head == NULL);
	assert(test.tail == NULL);
	
	printf("  Test passed.\n");
}

void test_del_multi_first() {
	printf("Testing deleting first member from multi element list.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	VPLists.push(&test, (void*)0x5678);
	VPLists.del(&test, 0);
	
	assert(test.head->data == (void*)0x5678);
	assert(test.size == 1);
	assert(test.head == test.tail);
	
	printf("  Test passed.\n");
}

void test_del_multi_middle() {
	printf("Testing deleting middle member from multi element list.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	VPLists.push(&test, (void*)0x5678);
	VPLists.push(&test, (void*)0x9abc);
	VPLists.del(&test, 1);
	
	assert(test.size == 2);
	assert(test.head->data == (void*)0x1234);
	assert(test.head->next == test.tail);
	assert(test.tail->data == (void*)0x9abc);
	assert(test.tail->next == NULL);
	
	printf("  Test passed.\n");
}

void test_get() {
	printf("Testing get method.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	VPLists.push(&test, (void*)0x5678);
	VPLists.push(&test, (void*)0x9abc);
	
	assert(VPLists.get(&test, 0) == (void*)0x1234);
	assert(VPLists.get(&test, 1) == (void*)0x5678);
	assert(VPLists.get(&test, 2) == (void*)0x9abc);
	
	printf("  Test passed.\n");
}

void test_get_out_of_bounds() {
	printf("Testing get method out of bounds.\n");
	
	List test = newList;

	assert(VPLists.get(&test, 0) == NULL);
	
	printf("  Test passed.\n");
}

void test_set() {
	printf("Testing set method.\n");
	
	List test = newList;
	VPLists.push(&test, (void*)0x1234);
	VPLists.push(&test, (void*)0x5678);
	VPLists.push(&test, (void*)0x9abc);
	
	VPLists.set(&test, 0, (void*)0x999);
	VPLists.set(&test, 1, (void*)0x888);
	VPLists.set(&test, 2, (void*)0x777);
	
	assert(VPLists.get(&test, 0) == (void*)0x999);
	assert(VPLists.get(&test, 1) == (void*)0x888);
	assert(VPLists.get(&test, 2) == (void*)0x777);
	
	printf("  Test passed.\n");
}

void test_set_out_of_bounds() {
	printf("Testing set method out of bounds.\n");
	
	List test = newList;
	
	VPLists.set(&test, 0, (void*)0x999);
	
	printf("  Test passed.\n");
}

void test_insert_empty() {
	printf("Testing insert to empty.\n");
	
	List test = newList;
	
	VPLists.insert(&test, (void*)0x999, 0);
	assert(test.size == 1);
	assert(test.head == test.tail);
	assert(test.head->data == (void*)0x99);
	
	printf("  Test passed.\n");
}

void test_insert_tail() {
	printf("Testing insert to tail.\n");
	
	List test = newList;
	
	VPLists.push(&test, (void*)0x123);
	VPLists.insert(&test, (void*)0x999, 1);
	
	assert(test.size == 2);
	assert(test.tail->data == (void*)0x999);
	assert(test.head->next == test.tail);
	assert(test.tail->prev == test.head);
	
	printf("  Test passed.\n");
}

void test_insert_middle() {
	printf("Testing insert to middle.\n");
	
	List test = newList;
	
	VPLists.push(&test, (void*)0x123);
	VPLists.push(&test, (void*)0x456);
	VPLists.insert(&test, (void*)0x999, 1);
	
	assert(test.size == 3);
	assert(test.head->next->data == (void*)0x999);
	assert(test.head->next == test.tail->prev);
	assert(test.head->next->next == test.tail);
	assert(test.head->prev->prev == test.head);
	
	printf("  Test passed.\n");
}

void test_next() {
	printf("Testing next.\n");
	
	List test = newList;
	
	VPLists.push(&test, (void*)0x123);
	VPLists.push(&test, (void*)0x456);
	
	assert(VPLists.next(&test) == (void*)0x123);
	assert(VPLists.next(&test) == (void*)0x456);
	assert(VPLists.next(&test) == NULL);
	
	printf("  Test passed.\n");
}

void test_prev() {
	printf("Testing prev.\n");
	
	List test = newList;
	
	VPLists.push(&test, (void*)0x123);
	VPLists.push(&test, (void*)0x456);
	
	assert(VPLists.prev(&test) == (void*)0x456);
	assert(VPLists.prev(&test) == (void*)0x123);
	assert(VPLists.prev(&test) == NULL);
	
	printf("  Test passed.\n");
}

void test_reset_cursor() {
	printf("Testing cursor reset.\n")
	List test = newList;
	
	VPLists.push(&test, (void*)0x123);
	VPLists.push(&test, (void*)0x456);
	VPLists.next(&test);
	VPLists.resetCursor(&test);
	
	assert(test.cursor == NULL);
	
	printf("  Test passed.\n");
}

int main() {
	printf("----Running tests----\n");
	
	test_push_to_empty();
	test_push();
	test_pop_empty();
	test_pop_one();
	test_pop_multi();
	test_del_empty();
	test_del_one();
	test_del_multi_first();
	test_del_multi_middle();
	test_get();
	test_get_out_of_bounds();
	test_set();
	test_set_out_of_bounds();
	test_next();
	test_prev();
	test_reset_cursor();
	
	return 0;
}