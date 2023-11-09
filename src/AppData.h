#ifndef APPDATA_H
#define APPDATA_H

#include "./collections/List.h"
#include "./Page.h" 

typedef struct
{
	double mouseX;
	double mouseY;
	int width;
	int height;
	Page currentPage;
} AppData;

#endif