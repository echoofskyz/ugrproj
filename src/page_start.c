#include "page_start.h"
#include "render/Renderer.h"
#include "render/Renderers.h"
#include "render/Color.h"

void draw_start(Renderer* rend) {
	/*rend->fillColor = (Color){.r = 1.0, .g = 0.33, .b = 0.0, .a = 0.5};
	Renderers.drawFillTriangle(rend,
		1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f);
	rend->fillColor = (Color){.r = 0.66, .g = 0.33, .b = 0.0, .a = 0.9};
	Renderers.drawFillTriangle(rend,
		1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f);
	rend->fillColor = (Color){.r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5};
	
	Renderers.drawFillTriangle(rend,
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f);
	
	rend->fillColor = (Color){.r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0};
	Renderers.drawLine(rend, 1.0, 1.0, 0.0, -1.0, -1.0, 0.0);
	Renderers.drawRect(rend, -0.25, 0.25, -0.75, 0.3, 0.40);
	*/
	rend->fillColor = (Color){.r = 1.0, .g = 0.33, .b = 0.0, .a = 0.5};
	Renderers.drawFillCircle(rend, 0.0, 0.0, 0.0, 1.0, 1.0);
	Renderers.drawFillCircle(rend, -0.5, 0.5, -1.0, 0.5, 1.0);
	Renderers.drawFillCircle(rend, -0.5, -0.5, -1.0, 0.5, 1.0);
	Renderers.drawFillCircle(rend, 0.5, 0.5, -1.0, 0.5, 1.0);
	Renderers.drawFillCircle(rend, 0.5, -0.5, -1.0, 0.5, 1.0);
}