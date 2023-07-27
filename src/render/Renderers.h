#include "Renderer.h"

struct Renderers {
	void (*init)();
	void (*triangle)(Renderer* renderer,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3);
	void (*line)(Renderer* renderer,
		float x1, float y1, float x2, float y2);
	void (*rect)(Renderer* renderer,
		float x, float y, float w, float h);
	void (*circle)(Renderer* renderer,
		float x, float y, float r, float h);
};

extern const struct Renderers Renderers;