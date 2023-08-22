typedef struct GUIElement {
	void (*draw)();
	double (*checkClick)(double x, double y);
	void (*onClick)(int x, int y, int button);
	double* x;
	double* y;
	double* z;
	double* w;
	double* h;
} GUIElement;