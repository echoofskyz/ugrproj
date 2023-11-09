#ifndef RENDERERS_H
#define RENDERERS_H

#include "Renderer.h"
#include "ShaderUtils.h"
#include "../collections/VPLists.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	void (*text)(Renderer* renderer,
		float x, float y, float s, char* text, int length);
};

extern const struct Renderers Renderers;

#endif