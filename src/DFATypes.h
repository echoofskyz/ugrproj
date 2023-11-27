#ifndef DFATYPES_H
#define DFATYPES_H

typedef struct DFANode
{
	float x;
	float y;
	int n;
} DFANode;

typedef struct DFALink
{
	DFANode* first;
	DFANode* second;
} DFALink;

#endif
