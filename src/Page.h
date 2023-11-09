#ifndef PAGE_H
#define PAGE_H

#include "./render/Renderer.h"

typedef struct Page {
	void (*draw)(Renderer*);
	void (*click)(int button, int action,
			float mouseX, float mouseY);
} Page;

#endif