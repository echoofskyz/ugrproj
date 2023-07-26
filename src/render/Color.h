#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct Color {
	float r;
	float g;
	float b;
	float a;
} Color;

extern const Color newColor;

#endif