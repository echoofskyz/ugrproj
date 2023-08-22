#include "../collections/List.h"

typedef struct Page {
	List* toolBarMenus;
	List* GUIElements;
	void (*draw)();
	void (*onClick)(int x, int y, int button);
} Page;