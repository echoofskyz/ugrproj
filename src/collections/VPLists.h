// Void Pointer List manager
#ifndef VPLISTS_H
#define VPLISTS_H

#include "List.h"

struct VPLists {
	void* (*pop)(List*);
	void* (*popFront)(List*);
	void* (*next)(List*);
	void* (*prev)(List*);
	void* (*get)(List*, int);
	void (*push)(List*, void*);
	void (*pushFront)(List*, void*);
	void (*del)(List*, int);
	void (*delNode)(List*, ListNode*);
	void (*delAll)(List*);
	void (*insert)(List*, void*, int);
	void (*set)(List*, int, void*);
	void (*resetCursor)(List*);
};

extern const struct VPLists VPLists;

#endif
