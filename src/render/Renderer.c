#include "Renderer.h"

Renderer newRenderer(int width, int height) {
	Renderer render = {
		.fillColor = newColor,
		.strokeColor = newColor,
		.strokeWeight = 0.005,
		.width = width,
		.height = height
	};
	
	return render;
};