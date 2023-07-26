#ifndef RENDERER_H
#define RENDERER_H

#include "Color.h"
#include "../collections/List.h"

typedef struct Renderer {
	Color fillColor;
	Color strokeColor;
	double strokeWeight;
	List triangles;
	List fillCircles;
	List text;
	int width;
	int height;
} Renderer;

Renderer newRenderer(int width, int height);

typedef struct RenderTri {
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
	float x3;
	float y3;
	float z3;
	Color color;
} RenderTri;

typedef struct RenderCircle {
	float x;
	float y;
	float z;
	float r;
	float h;
	Color color;
} RenderCircle;

#endif