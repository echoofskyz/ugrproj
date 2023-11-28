#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "AppData.h"
#include "./render/Renderers.h"
#include "./render/Renderer.h"

#include "startPage.h"
#include "dfaPage.h"

GLFWwindow* window;
Renderer renderer;
AppData appData;
static float maxFPS = 30.0;

static void key_callback(GLFWwindow* window,
		int key, int scancode, int action, int mods) 
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else
	{
		appData.currentPage.keyPress(&appData, key, action);
	}
}

static void resize_callback(GLFWwindow* window,
		int width, int height)
{
	glViewport(0, 0, width, height);
	renderer.width = width;
	renderer.height = height;
	appData.width = width;
	appData.height = height;
}

static void mouse_button_callback(GLFWwindow* window,
		int button, int action, int mods)
{
	appData.currentPage.click(&appData, button, action);
}

static void mouse_pos_callback(GLFWwindow* window,
		double mouseX, double mouseY)
{
	appData.mouseX = mouseX/appData.width*2.0-1.0;
	appData.mouseY = mouseY/appData.height*-2.0+1.0;
}

static void initWindow()
{
	// initialize window
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// basically fullscreen mode, no title bar, no resize
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	
	window = glfwCreateWindow(640, 480, "TEST WORDS", NULL, NULL);
}

static int init()
{
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		return 0;
	}
	
	initWindow();
	if (!window)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return 0;
	}
	
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return 0;
	}
	
	Renderers.init();
	
	return 1;
}

int main(void)
{
	if (!init())
	{
		return -1;
	}
	
	appData.mouseX = 0.0;
	appData.mouseY = 0.0;
	appData.currentPage = startPage;
	
	// set key handler
	glfwSetKeyCallback(window, key_callback);
	
	glfwGetFramebufferSize(window, &appData.width, &appData.height);

	renderer = newRenderer(appData.width, appData.height);
	
	// run to initialize 
	resize_callback(window, appData.width, appData.height);
	
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);
	
	glfwSetWindowAspectRatio(window, 1, 1);
	
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetTime() > 1.0/maxFPS)
		{
			glClearColor(0.5f, 0.3f, 0.3f, 0.1f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			appData.currentPage.draw(&renderer);

			glfwSwapBuffers(window);
			
			glfwSetTime(0.0);
		}
		
		glfwPollEvents();
	}
	
	appData.currentPage.free();
	glfwTerminate();
	
	return 0;
}
