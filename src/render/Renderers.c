#include "Renderers.h"
#include "ShaderUtils.h"
#include "../collections/VPLists.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int initialized = 0;

static unsigned int solidShader;
static unsigned int circShader;
static unsigned int textShader;

static unsigned int VAO;
static unsigned int VBO;

static void init() {
	if (initialized != 0) {
		return;
	}
	
	const char* vertShader =
			"#version 330 core\n"
			"layout (location = 0) in vec2 inPos;\n"
			"layout (location = 1) in vec4 inCol;\n"
			"out vec4 vertexColor;\n"
			"void main()\n"
			"{\n"
			"  vertexColor = inCol;\n"
			"  gl_Position = vec4(inPos, 0.0, 1.0);\n"
			"}\0";
	
	const char* circVertShader =
			"#version 330 core\n"
			"layout (location = 0) in vec2 inPos;\n"
			"layout (location = 1) in vec2 inCircPos;\n"
			"layout (location = 2) in vec2 inCircData;\n"
			"layout (location = 3) in vec4 inCol;\n"
			"out vec4 vertexColor;\n"
			"out vec2 circPos;\n"
			"out vec2 circData;\n"
			"void main()\n"
			"{\n"
			"  vertexColor = inCol;\n"
			"  circPos = inCircPos;\n"
			"  circData = inCircData;\n"
			"  gl_Position = vec4(inPos, 0.0, 1.0);\n"
			"}\0";

	const char* solidFragShader = 
			"#version 330 core\n"
			"in vec4 vertexColor;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"  FragColor = vertexColor;"
			"}\0";
	
	const char* circFragShader = 
			"#version 330 core\n"
			"uniform vec2 u_res\n;"
			"in vec4 vertexColor;\n"
			"in vec2 circPos;\n"
			"in vec2 circData;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"  vec2 uv = (gl_FragCoord.xy / u_res - 0.5) * 2.0;\n"
			"  float dist_field = length(circPos.xy*100.0 - uv*100.0) - circData.y*100.0;\n"
			"  dist_field = 1.0 - clamp(dist_field, 0.0, 1.0);\n"
			"  FragColor = vec4(vertexColor.rgb, dist_field);\n"
			"}\0";
	
	// compile shaders
	printf("Renderers.c: Compiling solid shader\n");
	solidShader = ShaderUtils.compileFromChars(vertShader, solidFragShader);
	
	printf("Renderers.c: Compiling circle shader\n");
	circShader = ShaderUtils.compileFromChars(circVertShader, circFragShader);
	
	// initialize VAO & VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	initialized = 1;
}

static void renderTri(Renderer* renderer,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		float r, float g, float b, float a) {
	
	// TODO: replace current color system with uniform
	
	float vertices[18] = {
		x1, y1, r, g, b, a,
		x2, y2, r, g, b, a,
		x3, y3, r, g, b, a
	};
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glUseProgram(solidShader);
	
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}

static void triangle(Renderer* renderer,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3) {
	
	renderTri(renderer,
			x1, y1,
			x2, y2,
			x3, y3,
			renderer->fillColor.r,
			renderer->fillColor.g,
			renderer->fillColor.b,
			renderer->fillColor.a);
}

static void rect(Renderer* renderer,
		float x, float y, float w, float h) {
	
	renderTri(renderer,
			x, y,
			x+w, y,
			x+w, y-h,
			renderer->fillColor.r,
			renderer->fillColor.g,
			renderer->fillColor.b,
			renderer->fillColor.a);
	
	renderTri(renderer,
			x, y,
			x, y-h,
			x+w, y-h,
			renderer->fillColor.r,
			renderer->fillColor.g,
			renderer->fillColor.b,
			renderer->fillColor.a);
}

static void renderCircle(Renderer* renderer,
		float x, float y, float rad, float h,
		float r, float g, float b, float a) {
			
	// TODO: replace current color and circle position with uniform
	
	float vertices[30] = {
		x,
		y + 2.1*rad,
		x, y, h, rad, r, g, b, a,

		x + sqrt(3*rad*rad),
		y - r,
		x, y, h, rad, r, g, b, a,
		
		x - sqrt(3*rad*rad),
		y - rad,
		x, y, h, rad, r, g, b, a
	};
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4*sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(3);
	
	glUniform2f(glGetUniformLocation(circShader, "u_res"), renderer->width, renderer->height);
	
	glUseProgram(circShader);
	
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}

static void circle(Renderer* renderer,
		float x, float y, float r, float h) {
	
	renderCircle(renderer,
			x, y, r, h,
			renderer->fillColor.r,
			renderer->fillColor.g,
			renderer->fillColor.b,
			renderer->fillColor.a);
}

static void line(Renderer* renderer,
		float x1, float y1, float x2, float y2) {
	
	// TODO: add line caps
	
	// get vector perpendicular to line
	float vec[] = {-(y2 - y1), (x2 - x1)};
	
	// normalize vector
	float len = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	vec[0] = vec[0] / len;
	vec[1] = vec[1] / len;
	
	// scale vector to strokeWeight
	vec[0] = vec[0] * renderer->strokeWeight;
	vec[1] = vec[1] * renderer->strokeWeight;
	
	renderTri(renderer,
			x1+vec[0], y1 + vec[1], 
			x1-vec[0], y1 - vec[1],
			x2-vec[0], y2 - vec[1],
			renderer->strokeColor.r,
			renderer->strokeColor.g,
			renderer->strokeColor.b,
			renderer->strokeColor.a);
	
	renderTri(renderer,
			x2+vec[0], y2 + vec[1],
			x2-vec[0], y2 - vec[1],
			x1+vec[0], y1 + vec[1],
			renderer->strokeColor.r,
			renderer->strokeColor.g,
			renderer->strokeColor.b,
			renderer->strokeColor.a);
}

const struct Renderers Renderers = {
	.init = init,
	.triangle = triangle,
	.line = line,
	.rect = rect,
	.circle = circle
};