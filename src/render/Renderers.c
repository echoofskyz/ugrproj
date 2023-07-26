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
			"layout (location = 0) in vec3 inPos;\n"
			"layout (location = 1) in vec4 inCol;\n"
			"out vec4 vertexColor;\n"
			"void main()\n"
			"{\n"
			"  vertexColor = inCol;\n"
			"  gl_Position = vec4(inPos, 1.0);\n"
			"}\0";
	
	const char* circVertShader =
			"#version 330 core\n"
			"layout (location = 0) in vec3 inPos;\n"
			"layout (location = 1) in vec3 inCircPos;\n"
			"layout (location = 2) in vec2 inCircData;\n"
			"layout (location = 3) in vec4 inCol;\n"
			"out vec4 vertexColor;\n"
			"out vec3 circPos;\n"
			"out vec2 circData;\n"
			"void main()\n"
			"{\n"
			"  vertexColor = inCol;\n"
			"  circPos = inCircPos;\n"
			"  circData = inCircData;\n"
			"  gl_Position = vec4(inPos, 1.0);\n"
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
			"in vec3 circPos;\n"
			"in vec2 circData;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"  vec2 uv = (gl_FragCoord.xy / u_res - 0.5) * 2.0;\n"
			"  float dist_field = length(circPos.xy*100.0 - uv*100.0) - circData.y*100.0;\n"
			"  dist_field = 1.0 - clamp(dist_field, 0.0, 1.0);\n"
			/*"  if(dist_field < 0.1){\n"
			"    discard;"
			"  }\n"*/
			"  FragColor = vec4(vertexColor.xyz, dist_field);\n"
			"}\0";
	
	// TODO: create circFragShader
	
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

static void renderTriangles(Renderer* renderer) {
	List* triangles = &renderer->triangles;
	float vertices[triangles->size*21];
	
	RenderTri* tri = (RenderTri*)VPLists.pop(triangles);
	for (int i=0;tri != NULL;i+=21) {
		vertices[i] = tri->x1;
		vertices[i+1] = tri->y1;
		vertices[i+2] = tri->z1;
		vertices[i+3] = tri->color.r;
		vertices[i+4] = tri->color.g;
		vertices[i+5] = tri->color.b;
		vertices[i+6] = tri->color.a;
		
		vertices[i+7] = tri->x2;
		vertices[i+8] = tri->y2;
		vertices[i+9] = tri->z2;
		vertices[i+10] = tri->color.r;
		vertices[i+11] = tri->color.g;
		vertices[i+12] = tri->color.b;
		vertices[i+13] = tri->color.a;
		
		vertices[i+14] = tri->x3;
		vertices[i+15] = tri->y3;
		vertices[i+16] = tri->z3;
		vertices[i+17] = tri->color.r;
		vertices[i+18] = tri->color.g;
		vertices[i+19] = tri->color.b;
		vertices[i+20] = tri->color.a;
		
		free(tri);
		tri = (RenderTri*)VPLists.pop(triangles);
	}
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glUseProgram(solidShader);
	//glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}

static void renderFillCircles(Renderer* renderer) {
	// need to figure out what info is needed and how
	//   it should get into the shader
	//   have x, y, z, rad, hard
	//   could probably do three verts/one tri per circle
	//   just do an equilateral triangle around the actual circle location
	//   need to figure out formula to get vert points
	// going to need to set up circle shader
	
	List* fillCircles = &renderer->fillCircles;
	float vertices[144];
	
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8*sizeof(float)));
	glEnableVertexAttribArray(3);
	
	glUseProgram(circShader);
	glUniform2f(glGetUniformLocation(circShader, "u_res"), renderer->width, renderer->height);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	RenderCircle* circ = (RenderCircle*)VPLists.pop(fillCircles);
	while (circ != NULL) {
		vertices[0] = circ->x;
		vertices[1] = circ->y + 2.1*circ->r;
		vertices[2] = circ->z;
		vertices[3] = circ->x;
		vertices[4] = circ->y;
		vertices[5] = circ->z;
		vertices[6] = circ->h;
		vertices[7] = circ->r;
		vertices[8] = circ->color.r;
		vertices[9] = circ->color.g;
		vertices[10] = circ->color.b;
		vertices[11] = circ->color.a;

		vertices[12] = circ->x + sqrt(3*circ->r*circ->r);
		vertices[13] = circ->y - circ->r;
		vertices[14] = circ->z;
		vertices[15] = circ->x;
		vertices[16] = circ->y;
		vertices[17] = circ->z;
		vertices[18] = circ->h;
		vertices[19] = circ->r;
		vertices[20] = circ->color.r;
		vertices[21] = circ->color.g;
		vertices[22] = circ->color.b;
		vertices[23] = circ->color.a;
		
		vertices[24] = circ->x - sqrt(3*circ->r*circ->r);
		vertices[25] = circ->y - circ->r;
		vertices[26] = circ->z;
		vertices[27] = circ->x;
		vertices[28] = circ->y;
		vertices[29] = circ->z;
		vertices[30] = circ->h;
		vertices[31] = circ->r;
		vertices[32] = circ->color.r;
		vertices[33] = circ->color.g;
		vertices[34] = circ->color.b;
		vertices[35] = circ->color.a;
		
		glBufferData(GL_ARRAY_BUFFER, 144, vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 144);
		
		free(circ);
		circ = (RenderCircle*)VPLists.pop(fillCircles);
	}
}

static void renderText(Renderer* renderer) {
	
}

static void drawFillTriangle(Renderer* renderer,
		float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3) {
	List* triList = &renderer->triangles;
	RenderTri* triangle = malloc(sizeof(RenderTri));
	(*triangle) = (RenderTri){
		.x1 = x1, .y1 = y1, .z1 = z1,
		.x2 = x2, .y2 = y2, .z2 = z2,
		.x3 = x3, .y3 = y3, .z3 = z3,
		.color = renderer->fillColor
	};
	
	VPLists.push(triList, (void*)triangle);
}

static void drawStrokeTriangle(Renderer* renderer,
		float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3) {
	List* triList = &renderer->triangles;
	RenderTri* triangle = malloc(sizeof(RenderTri));
	(*triangle) = (RenderTri){
		.x1 = x1, .y1 = y1, .z1 = z1,
		.x2 = x2, .y2 = y2, .z2 = z2,
		.x3 = x3, .y3 = y3, .z3 = z3,
		.color = renderer->strokeColor
	};
	
	VPLists.push(triList, (void*)triangle);
}

static void drawLine(Renderer* renderer,
		float x1, float y1, float z1,
		float x2, float y2, float z2) {
	List * triList = &renderer->triangles;
	
	// get vector perpendicular to line
	float vec[] = {-(y2 - y1), (x2 - x1)};
	
	// normalize vector
	float len = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	vec[0] = vec[0] / len;
	vec[1] = vec[1] / len;
	
	// scale vector to strokeWeight
	vec[0] = vec[0] * renderer->strokeWeight;
	vec[1] = vec[1] * renderer->strokeWeight;
	
	RenderTri* tri1 = malloc(sizeof(RenderTri));
	RenderTri* tri2 = malloc(sizeof(RenderTri));
	
	(*tri1) = (RenderTri){
		.x1 = x1+vec[0], .y1 = y1 + vec[1], .z1 = z1, 
		.x2 = x1-vec[0], .y2 = y1 - vec[1], .z2 = z1, 
		.x3 = x2-vec[0], .y3 = y2 - vec[1], .z3 = z2,
		.color = renderer->strokeColor
	};
	(*tri2) = (RenderTri){
		.x1 = x2+vec[0], .y1 = y2 + vec[1], .z1 = z2, 
		.x2 = x2-vec[0], .y2 = y2 - vec[1], .z2 = z2, 
		.x3 = x1+vec[0], .y3 = y1 + vec[1], .z3 = z1,
		.color = renderer->strokeColor
	};
	
	VPLists.push(triList, (void*)tri1);
	VPLists.push(triList, (void*)tri2);
}

static void drawFillRect(Renderer* renderer,
		float x, float y, float z, float w, float h) {
	List * triList = &renderer->triangles;
	RenderTri* tri1 = malloc(sizeof(RenderTri));
	RenderTri* tri2 = malloc(sizeof(RenderTri));
	
	(*tri1) = (RenderTri){
		.x1 = x, .y1 = y, .z1 = z, 
		.x2 = x+w, .y2 = y, .z2 = z, 
		.x3 = x+w, .y3 = y-h, .z3 = z,
		.color = renderer->fillColor
	};
	(*tri2) = (RenderTri){
		.x1 = x, .y1 = y, .z1 = z, 
		.x2 = x, .y2 = y-h, .z2 = z, 
		.x3 = x+w, .y3 = y-h, .z3 = z,
		.color = renderer->fillColor
	};
	
	VPLists.push(triList, (void*)tri1);
	VPLists.push(triList, (void*)tri2);
}

static void drawStrokeRect(Renderer* renderer,
		float x, float y, float z, float w, float h) {
	drawLine(renderer, x, y, z, x+w, y, z);
	drawLine(renderer, x, y, z, x, y-h, z);
	drawLine(renderer, x+w, y, z, x+w, y-h, z);
	drawLine(renderer, x, y-h, z, x+w, y-h, z);
}

static void drawRect(Renderer* renderer,
		float x, float y, float z, float w, float h) {
	drawFillRect(renderer, x, y, z, w, h);
	drawStrokeRect(renderer, x, y, z, w, h);
}

static void drawFillCircle(Renderer* renderer,
		float x, float y, float z, float r, float h) {
	List* circles = &renderer->fillCircles;
	RenderCircle* circle = malloc(sizeof(RenderCircle));
	
	(*circle) = (RenderCircle){
		.x = x, .y = y, .z = z, .r = r, .h = h,
		.color = renderer->fillColor
	};
	
	VPLists.push(circles, (void*)circle);
}

static void destroy(Renderer* renderer) {
	// unsafe? primitive render objects are all malloc'd
	VPLists.delAll(&renderer->triangles);
	VPLists.delAll(&renderer->fillCircles);
	//VPLists.delAll(&renderer->strokeEllipses);
	VPLists.delAll(&renderer->text);
}

const struct Renderers Renderers = {
	.init = init,
	.renderTriangles = renderTriangles,
	.renderFillCircles = renderFillCircles,
	.renderText = renderText,
	.drawFillTriangle = drawFillTriangle,
	.drawStrokeTriangle = drawStrokeTriangle,
	.drawLine = drawLine,
	.drawFillRect = drawFillRect,
	.drawStrokeRect = drawStrokeRect,
	.drawRect = drawRect,
	.drawFillCircle = drawFillCircle,
	.destroy = destroy
};