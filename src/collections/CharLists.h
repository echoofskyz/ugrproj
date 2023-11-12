#ifndef CHARLISTS_H
#define CHARLISTS_H

#include "List.h"

struct CharLists {
	char (*pop)(List*);
	char (*popFront)(List*);
	char (*next)(List*);
	char (*prev)(List*);
	char (*get)(List*, int);
	void (*push)(List*, char);
	void (*pushFront)(List*, char);
	void (*del)(List*, int);
	void (*delNode)(List*, ListNode*);
	void (*delAll)(List*);
	void (*insert)(List*, char, int);
	void (*set)(List*, int, char);
	void (*resetCursor)(List*);
};

extern const struct CharLists CharLists;

#endif
