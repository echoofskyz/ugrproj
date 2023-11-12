#include "startPage.h"
#include "dfaPage.h"

static int btnDFAdown = 0; 

static void freePage()
{

}

static void draw(Renderer* rend) {
	
	// background
	rend->fillColor = (Color)
			{.r=0.2, .g=0.2, .b=0.2, .a=1.0};
	Renderers.rect(rend, -1.0, 1.0, 2.0, 2.0);
	
	// DFA button
	rend->fillColor = (Color)
			{.r=0.5, .g=0.5, .b=0.5, .a=1.0};
	if (btnDFAdown)
	{
		Renderers.rect(rend, -0.19, 0.19, 0.38, 0.38);
	}
	else
	{
		Renderers.rect(rend, -0.2, 0.2, 0.4, 0.4);
	}
	
	// DFA button text
	rend->strokeColor = (Color)
			{.r=0.05, .g=0.3, .b=0.05, .a=1.0};
	rend->strokeWeight = 0.02;
	Renderers.text(rend, -0.18, 0.15, 2.0, "DFA", 3);
	
	// DFA button graphic
	rend->fillColor = (Color) 
			{.r=0.05, .g=0.3, .b=0.05, .a=1.0};
	rend->strokeColor = (Color)
			{.r=0.0, .g=0.0, .b=0.0, .a=1.0};
	Renderers.line(rend, 0.0, -0.05, 0.1, -0.02);
	Renderers.line(rend, 0.0, -0.05, -0.1, -0.03);
	Renderers.circle(rend, 0.0, -0.05, 0.05, 1.0);
}

static void leftClick(AppData* appdata, int action)
{
	double mouseX = appdata->mouseX;
	double mouseY = appdata->mouseY;
	
	if (action == GLFW_PRESS)
	{
		printf("LEFT PRESSED AT: %f, %f\n", mouseX, mouseY);
		if (mouseX > -0.2 && mouseX < 0.2 && mouseY > -0.2 && mouseY < 0.2) {
			btnDFAdown = 1;
			freePage();
			appdata->currentPage = dfaPage;
		}
		
	}
	else if (action == GLFW_RELEASE)
	{
		printf("LEFT RELEASED AT: %f, %f\n", mouseX, mouseY);
		btnDFAdown = 0;
	}
}

static void rightClick(AppData* appdata, int action)
{
	double mouseX = appdata->mouseX;
	double mouseY = appdata->mouseY;
	
	if (action == GLFW_PRESS)
	{
		printf("RIGHT PRESSED AT: %f, %f\n", mouseX, mouseY);
	}
	else if (action == GLFW_RELEASE)
	{
		printf("RIGHT RELEASED AT: %f, %f\n", mouseX, mouseY);
	}
}

static void click(AppData* appdata, int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		rightClick(appdata, action);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		leftClick(appdata, action);
	}
}

const Page startPage =
{
	.draw = draw,
	.click = click,
	.free = freePage
};
