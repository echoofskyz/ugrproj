#ifndef DFALINKLISTS_H
#define DFALINKLISTS_H

#include "List.h"
#include "../DFATypes.h"

struct DFALinkLists
{
	void (*push)(List*, DFANode*, DFANode*);
	DFALink* (*next)(List*);
	void (*resetCursor)(List*);
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

extern const struct DFALinkLists DFALinkLists;

#endif
