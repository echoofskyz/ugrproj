#include "Renderer.h"

Renderer newRenderer(int width, int height) {
	Renderer render = {
		.fillColor = newColor,
		.strokeColor = newColor,
		.strokeWeight = 0.01,
		.triangles = newList,
		.fillCircles = newList,
		.text = newList,
		.width = width,
		.height = height
	};
	
	return render;
};