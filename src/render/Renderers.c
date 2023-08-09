#include "Renderers.h"
#include "ShaderUtils.h"
#include "../collections/VPLists.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int initialized = 0;

static unsigned int solidShader;
static unsigned int circShader;
static unsigned int lineShader;

static unsigned int VAO;
static unsigned int VBO;

static void init() {
	if (initialized != 0) {
		return;
	}

	const char* vertShader =
			"#version 330 core\n"
			"layout (location = 0) in vec2 inPos;\n"
			"void main()\n"
			"{\n"
			"  gl_Position = vec4(inPos, 0.0, 1.0);\n"
			"}\0";

	const char* solidFragShader = 
			"#version 330 core\n"
			"uniform vec4 u_color;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"  FragColor = u_color;"
			"}\0";
	
	const char* circFragShader = 
			"#version 330 core\n"
			"uniform vec2 u_res\n;"
			"uniform vec2 u_pos\n;"
			"uniform vec2 u_data\n;"
			"uniform vec4 u_color\n;"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"  vec2 uv = (gl_FragCoord.xy / u_res - 0.5) * 2.0;\n"
			"  float dist_field = length(u_pos*100.0*u_data.y - uv*100.0*u_data.y) - u_data.x*100.0*u_data.y;\n"
			"  dist_field = 1.0 - clamp(dist_field, 0.0, 1.0);\n"
			"  FragColor = vec4(u_color.rgb, u_color.a * dist_field);\n"
			"}\0";
	
	const char* lineFragShader = 
			"#version 330 core\n"
			"uniform vec2 u_res\n;"
			"uniform vec2 u_pos1\n;"
			"uniform vec2 u_pos2\n;"
			"uniform vec2 u_data\n;"
			"uniform vec4 u_color\n;"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"  float s = (u_pos1.y-u_pos2.y)/(u_pos1.x-u_pos2.x);\n"
			"  vec2 uv = (gl_FragCoord.xy / u_res - 0.5) * 2.0;\n"
			"  float cx;\n"
			"  float cy;\n"
			"  if (abs(u_pos1.x-u_pos2.x) < 0.001)\n"
			"  {\n"
			"    cx = u_pos1.x;\n"
			"    cy = uv.y;\n"
			"  } else\n"
			"  {\n"
			"    cx = (s*(s*u_pos1.x-u_pos1.y+uv.y)+uv.x)/(s*s+1);\n"
			"    cy = (s*(s*uv.y+uv.x-u_pos1.x)+u_pos1.y)/(s*s+1);\n"
			"  }\n"
			"  if (cx < min(u_pos1.x, u_pos2.x)) cx = min(u_pos1.x, u_pos2.x);\n"
			"  if (cx > max(u_pos1.x, u_pos2.x)) cx = max(u_pos1.x, u_pos2.x);\n"
			"  if (cy < min(u_pos1.y, u_pos2.y)) cy = min(u_pos1.y, u_pos2.y);\n"
			"  if (cy > max(u_pos1.y, u_pos2.y)) cy = max(u_pos1.y, u_pos2.y);\n"
			"  vec2 c = vec2(cx, cy);\n"
			"  float dist_field = length(c*100.0*u_data.y - uv*100.0*u_data.y) - u_data.x*100.0*u_data.y;\n"
			"  dist_field = 1.0 - clamp(dist_field, 0.0, 1.0);\n"
			//"  if (abs(uv.x - cx) < 0.01 && abs(uv.y - cy) < 0.01) {\n"
			//"    FragColor= vec4(1.0, 0.0, 0.0, 1.0);\n"
			//"  } else if (dist_field < 0.01) {\n"
			//"  FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
			//"  } else {\n"
			"  FragColor = vec4(u_color.rgb, u_color.a * dist_field);\n"
			//"  }\n"
			"}\0";
	
	// compile shaders
	printf("Renderers.c: Compiling solid shader\n");
	solidShader = ShaderUtils.compileFromChars(vertShader, solidFragShader);
	
	printf("Renderers.c: Compiling circle shader\n");
	circShader = ShaderUtils.compileFromChars(vertShader, circFragShader);
	
	printf("Renderers.c: Compiling line shader\n");
	lineShader = ShaderUtils.compileFromChars(vertShader, lineFragShader);
	
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
	
	float vertices[6] = {
		x1, y1,
		x2, y2,
		x3, y3
	};
	
	// bind the buffers, might not need to do this every time
	//   need to figure out if I'll ever need to bind other ones
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glUseProgram(solidShader);
	
	glUniform4f(glGetUniformLocation(solidShader, "u_color"), r, g, b, a);
	
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
			
	rad = rad * 2;
	
	float vertices[6] = {
		x, y + 2*rad,
		x + sqrt(3*rad*rad), y - rad,
		x - sqrt(3*rad*rad), y - rad
	};
	
	rad = rad / 2;
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glUseProgram(circShader);
	
	glUniform2f(glGetUniformLocation(circShader, "u_res"), renderer->width, renderer->height);
	glUniform2f(glGetUniformLocation(circShader, "u_pos"), x, y);
	glUniform2f(glGetUniformLocation(circShader, "u_data"), rad, h);
	glUniform4f(glGetUniformLocation(circShader, "u_color"), r, g, b, a);
	
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
	
	// get vector perpendicular to line
	float per[2] = {-(y2 - y1), (x2 - x1)};
	
	// get vector parallel to line
	float par[2] = {x2 - x1, y2 - y1};
	
	// normalize vectors
	float len = sqrt(per[0] * per[0] + per[1] * per[1]);
	per[0] = per[0] / len;
	per[1] = per[1] / len;
	
	len = sqrt(par[0] * par[0] + par[1] * par[1]);
	par[0] = par[0] / len;
	par[1] = par[1] / len;
	
	// scale vectors to strokeWeight
	per[0] = per[0] * renderer->strokeWeight;
	per[1] = per[1] * renderer->strokeWeight;
	
	par[0] = par[0] * renderer->strokeWeight;
	par[1] = par[1] * renderer->strokeWeight;
	
	float vertices[12] = {
		x1+per[0]-par[0], y1+per[1]-par[1], 
		x1-per[0]-par[0], y1-per[1]-par[1],
		x2-per[0]+par[0], y2-per[1]+par[1],
		x2+per[0]+par[0], y2+per[1]+par[1],
		x2-per[0]+par[0], y2-per[1]+par[1],
		x1+per[0]-par[0], y1+per[1]-par[1]
	};
	
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glUseProgram(lineShader);
	
	glUniform2f(glGetUniformLocation(lineShader, "u_res"), renderer->width, renderer->height);
	glUniform2f(glGetUniformLocation(lineShader, "u_pos1"), x1, y1);
	glUniform2f(glGetUniformLocation(lineShader, "u_pos2"), x2, y2);
	glUniform2f(glGetUniformLocation(lineShader, "u_data"), renderer->strokeWeight/4.0, 1.5);
	glUniform4f(glGetUniformLocation(lineShader, "u_color"),
			renderer->strokeColor.r,
			renderer->strokeColor.g,
			renderer->strokeColor.b,
			renderer->strokeColor.a);
			
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}

static void text(Renderer* renderer,
		float x, float y, float s, char* text, int length) {
	float ox = -0.03;
	float oy = 0;
	float k = 0.05;
	float ls = 0.07;
	
	for(int i=0;i<length;i++) {
		switch(text[i]) {
			case 'A':
			case 'a':
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.04*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'B':
			case 'b':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.035*s+y+oy, 0.05*s+x+ox, -0.035*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.06*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.03*s+y+oy, 0.05*s+x+ox, -0.035*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.035*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case 'C':
			case 'c':
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.03*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				ox+=k*s;
				break;
			
			case 'D':
			case 'd':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.055*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.055*s+y+oy, 0.06*s+x+ox, -0.035*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.035*s+y+oy, 0.05*s+x+ox, -0.015*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.015*s+y+oy, 0.02*s+x+ox, -0.01*s+y+oy);
				ox+=k*s;
				break;
			
			case 'E':
			case 'e':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.035*s+y+oy, 0.05*s+x+ox, -0.035*s+y+oy);
				ox+=k*s;
				break;
			
			case 'F':
			case 'f':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.035*s+y+oy, 0.05*s+x+ox, -0.035*s+y+oy);
				ox+=k*s;
				break;
			
			case 'G':
			case 'g':
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.03*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				ox+=k*s;
				break;
				
			case 'H':
			case 'h':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.035*s+y+oy, 0.06*s+x+ox, -0.035*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case 'I':
			case 'i':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'J':
			case 'j':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.05*s+y+oy, 0.04*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.04*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				ox+=k*s;
				break;
				
			case 'K':
			case 'k':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.03*s+y+oy, 0.04*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.03*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.03*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'L':
			case 'l':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'M':
			case 'm':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.03*s+y+oy);
				ox+=k*s;
				break;
			
			case 'N':
			case 'n':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'O':
			case 'o':
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.03*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				ox+=k*s;
				break;
			
			case 'P':
			case 'p':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.03*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.04*s+y+oy, 0.05*s+x+ox, -0.04*s+y+oy);
				ox+=k*s;
				break;
			
			case 'Q':
			case 'q':
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.03*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case 'R':
			case 'r':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.03*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.04*s+y+oy, 0.05*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case 'S':
			case 's':
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.03*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.03*s+y+oy, 0.03*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.03*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case 'T':
			case 't':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'U':
			case 'u':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case 'V':
			case 'v':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'W':
			case 'w':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.04*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.06*s+y+oy, 0.04*s+x+ox, -0.04*s+y+oy);
				ox+=k*s;
				break;
			
			case 'X':
			case 'x':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case 'Y':
			case 'y':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.06*s+y+oy, 0.04*s+x+ox, -0.04*s+y+oy);
				ox+=k*s;
				break;
				
			case 'Z':
			case 'z':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case '1':
				line(renderer, 0.02*s+x+ox, -0.03*s+y+oy, 0.04*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.01*s+y+oy, 0.04*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case '2':
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.06*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case '3':
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.035*s+y+oy, 0.045*s+x+ox, -0.035*s+y+oy);
				ox+=k*s;
				break;
			
			case '4':
				line(renderer, 0.02*s+x+ox, -0.04*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case '5':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.03*s+y+oy, 0.02*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.03*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
			
			case '6':
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.05*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.03*s+y+oy, 0.05*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.03*s+y+oy, 0.02*s+x+ox, -0.04*s+y+oy);
				ox+=k*s;
				break;
				
			case '7':
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.04*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				ox+=k*s;
				break;
			
			case '8':
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.035*s+y+oy, 0.02*s+x+ox, -0.03*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.035*s+y+oy, 0.02*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.02*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.03*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.03*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.03*s+y+oy, 0.05*s+x+ox, -0.035*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.05*s+x+ox, -0.035*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.035*s+y+oy, 0.05*s+x+ox, -0.035*s+y+oy);
				ox+=k*s;
				break;
			
			case '9':
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.03*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.03*s+y+oy, 0.03*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.04*s+y+oy, 0.03*s+x+ox, -0.04*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				ox+=k*s;
				break;
				
			case '0':
				line(renderer, 0.06*s+x+ox, -0.02*s+y+oy, 0.05*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.01*s+y+oy, 0.03*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.02*s+y+oy, 0.02*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.03*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.03*s+x+ox, -0.06*s+y+oy, 0.05*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.05*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.05*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.05*s+y+oy, 0.06*s+x+ox, -0.02*s+y+oy);
				ox+=k*s;
				break;
				
			case ' ':
				ox+=k*s;
				break;
			
			case '\n':
				ox = -0.03;
				oy-=ls*s;
				break;
			
			default:
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.02*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.06*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.01*s+y+oy, 0.06*s+x+ox, -0.06*s+y+oy);
				line(renderer, 0.02*s+x+ox, -0.06*s+y+oy, 0.06*s+x+ox, -0.01*s+y+oy);
				ox+=k*s;
				break;
		}
	}
}

const struct Renderers Renderers = {
	.init = init,
	.triangle = triangle,
	.line = line,
	.rect = rect,
	.circle = circle,
	.text = text
};