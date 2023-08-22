#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_man.h"
#include "windowCtrlElement.h"

static const char* vertexShader0Source =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

static const char* fragmentShader0Source = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, 0.5f, 0.5f);"
	"}\0";

static unsigned int shader;
	
void draw(GUIElement* self) {
	
}

double checkClick(GUIElement* self, double x, double y) {
	if ((x > elemX) && (x < elemX + elemW)
			&& (y < elemY) && (y > elemY - elemH)) {
		return elemZ;
	}
	
	return -1;
};

void onClick(GUIElement* self, double x, double y, int button) {
	printf("So triggered\n");
}

GUIElement newWinCtrlElem() {
	GUIElement wcElem = {
		.draw = draw;
		.checkClick = checkClick;
		.onClick = onClick;
		.x = 0.75;
		.y = 0.75;
		.z = 1.0;
		.w = 0.25;
		.h = 0.25;
	};
	
	return wcElem;
}