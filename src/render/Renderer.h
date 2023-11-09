#ifndef RENDERER_H
#define RENDERER_H

#include "Color.h"

typedef struct Renderer {
	Color fillColor;
	Color strokeColor;
	double strokeWeight;
	int width;
	int height;
} Renderer;

Renderer newRenderer(int width, int height);

#endif