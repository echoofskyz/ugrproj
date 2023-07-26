#include "Renderer.h"

struct Renderers {
	void (*init)();
	void (*renderTriangles)(Renderer*);
	void (*renderFillCircles)(Renderer*);
	void (*renderStrokeEllipses)(Renderer*);
	void (*renderText)(Renderer*);
	void (*drawFillTriangle)(Renderer* renderer,
		float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3);
	void (*drawStrokeTriangle)(Renderer* renderer,
		float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3);
	void (*drawLine)(Renderer* renderer,
		float x1, float y1, float z1,
		float x2, float y2, float z2);
	void (*drawFillRect)(Renderer* renderer,
		float x, float y, float z,
		float w, float h);
	void (*drawFillCircle)(Renderer* renderer,
		float x, float y, float z,
		float r, float h);
	void (*drawStrokeRect)(Renderer* renderer,
		float x, float y, float z,
		float w, float h);
	void (*drawRect)(Renderer* renderer,
		float x, float y, float z,
		float w, float h);
	void (*destroy)(Renderer*);
};

extern const struct Renderers Renderers;