#ifndef DFATYPES_H
#define DFATYPES_H

#include "./collections/List.h"

typedef struct DFANode
{
	float x;
	float y;
	int n;
	int isAccept;
} DFANode;

typedef struct DFALink
{
	DFANode* first;
	DFANode* second;
	List transitions;
} DFALink;

#endif
