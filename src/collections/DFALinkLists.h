#ifndef DFALINKLISTS_H
#define DFALINKLISTS_H

#include "List.h"
#include "../DFATypes.h"
#include "CharLists.h"

struct DFALinkLists
{
	void (*push)(List*, DFANode*, DFANode*);
	DFALink* (*next)(List*);
	DFALink* (*peek)(List*);
	void (*resetCursor)(List*);
	void (*delNode)(List*, DFALink*);
	void (*delAll)(List*);
	DFALink* (*pop)(List*);
	/*
	char (*pop)(List*);
	char (*popFront)(List*);
	
	char (*prev)(List*);
	char (*get)(List*, int);
	
	void (*pushFront)(List*, char);
	void (*del)(List*, int);
	
	void (*delAll)(List*);
	void (*insert)(List*, char, int);
	void (*set)(List*, int, char);
	
	*/
};

extern const struct DFALinkLists DFALinkLists;

#endif
