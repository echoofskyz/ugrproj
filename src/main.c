#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "./render/Renderers.h"
#include "./render/Renderer.h"
#include "page_start.h"

GLFWwindow* window;
Renderer renderer;

// process input on interrupt
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	renderer.width = width;
	renderer.height = height;
}

// process input by frame
void processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void initWindow() {
	// initialize window
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// basically fullscreen mode, no title bar, no resize
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	
	window = glfwCreateWindow(640, 480, "TEST WORDS", NULL, NULL);
}

int init() {
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		return 0;
	}
	
	initWindow();
	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return 0;
	}
	
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return 0;
	}
	
	Renderers.init();
	
	return 1;
}

int main(void) {
	if (!init()) {
		return -1;
	}
	
	// set key handler
	glfwSetKeyCallback(window, key_callback);
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	renderer = newRenderer(width, height);
	
	resize_callback(window, width, height);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	while (!glfwWindowShouldClose(window))
	{
		
		processInput(window);
		glClearColor(0.5f, 0.3f, 0.3f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		draw_start(&renderer);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
