#include "page_start.h"
#include "render/Renderer.h"
#include "render/Renderers.h"
#include "render/Color.h"

void draw_start(Renderer* rend) {
	//rend->strokeColor = (Color){.r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0};
	rend->fillColor = (Color){.r = 1.0, .g = 0.33, .b = 1.0, .a = 1.0};
	//Renderers.circle(rend, 0.0, 0.0, 1.0, 1.0);
	//Renderers.rect(rend, -0.5, 0.5, 1.0, 1.0);
	//Renderers.triangle(rend, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0);
	rend->strokeWeight = 0.01;
	Renderers.line(rend, -1.0, -0.5, 0.0, 0.3);
	Renderers.line(rend, -1.0, -1.0, -0.5, -0.5);
	Renderers.line(rend, -0.8, -1.0, 0.5, 0.0);
	Renderers.line(rend, 0.0, -0.8, 0.1, 0.5);
	Renderers.line(rend, -1.0, 1.0, 1.0, -1.0);
	Renderers.line(rend, -0.4, -0.8, 0.8, -0.8);
	//rend->fillColor = (Color){.r = 1.0, .g = 0.0, .b = 0.0, .a = 1.0};
	//Renderers.circle(rend, 0.0, 0.0, 0.1, 1.0);
	Renderers.text(rend, -1.0, 1.0, 1.0, "ab\ncdefgh  ijklmno\npqrstu vwxyz1234\n5678 90", 60);
}