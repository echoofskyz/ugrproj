#include "dfaPage.h"

static int btnDFAdown = 0;
static List testSeq;

static void draw(Renderer* rend)
{
	// don't know the right way to do this,
	//   basically check if testSeq might
	//   not be initialized and if so
	//   reinitialize it
	// -might not be necessary
	if (testSeq.head == NULL)
	{
		testSeq = newList;
	}
	
	if (testSeq.size < 10)
	{
		char a = 'a';
		VPLists.push(&testSeq, (void*)&a);
	}
	
	char testSeqStr[testSeq.size];
	
	for (int i = 0; i < testSeq.size; i++)
	{
		char b = 'b';
		testSeqStr[i] = b;
		//((char*)VPLists.next(&testSeq))[0];
	}
	printf("%d, %s\n", testSeq.size, testSeqStr);
	
	VPLists.resetCursor(&testSeq);
	
	// background
	rend->fillColor = (Color)
			{.r=0.4, .g=0.4, .b=0.4, .a=1.0};
	Renderers.rect(rend, -1.0, 1.0, 2.0, 2.0);
	
	// DFA button text
	rend->strokeColor = (Color)
			{.r=0.05, .g=0.3, .b=0.05, .a=1.0};
	rend->strokeWeight = 0.02;
	Renderers.text(rend, -0.18, 0.15, 2.0, testSeqStr, testSeq.size);
	
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
		if (mouseX > -0.2 && mouseX < 0.2
				&& mouseY > -0.2 && mouseY < 0.2)
		{
			btnDFAdown = 1;
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

const Page dfaPage =
{
	.draw = draw,
	.click = click
};
