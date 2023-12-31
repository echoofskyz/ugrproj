#ifndef APPDATA_H
#define APPDATA_H

#include "./collections/List.h"
#include "./render/Renderer.h"

typedef struct AppData AppData;
typedef struct Page Page;

struct Page {
	void (*draw)(Renderer*);
	void (*click)(AppData*, int button, int action);
	void (*free)();
	void (*keyPress)(AppData*, int key, int action);
};

struct AppData {
	double mouseX;
	double mouseY;
	int width;
	int height;
	Page currentPage;
};

#endif
