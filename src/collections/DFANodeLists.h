#ifndef DFANODELISTS_H
#define DFANODELISTS_H

#include "List.h"
#include "../DFATypes.h"

struct DFANodeLists
{
	void (*push)(List*, int n, float x, float y);
	void (*add)(List*, DFANode*);
	DFANode* (*next)(List*);
	void (*resetCursor)(List*);
	void (*delAll)(List*);
	void (*delNode)(List*, DFANode*);
	DFANode* (*pop)(List*);
	/*
	char (*pop)(List*);
	char (*popFront)(List*);
	
	char (*prev)(List*);
	char (*get)(List*, int);
	void (*pushFront)(List*, char);
	void (*del)(List*, int);
	void (*delNode)(List*, ListNode*);
	void (*delAll)(List*);
	void (*insert)(List*, char, int);
	void (*set)(List*, int, char);
	
	*/
};

extern const struct DFANodeLists DFANodeLists;

#endif
