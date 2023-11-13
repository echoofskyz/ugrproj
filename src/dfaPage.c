#include "dfaPage.h"

static int btnDFAdown = 0;
static List testSeq;

static void freePage()
{
	CharLists.delAll(&testSeq);
}

static void draw(Renderer* rend)
{
	if (testSeq.head == NULL)
	{
		testSeq = newList;
		while (testSeq.size < 20)
		{
			CharLists.push(&testSeq, 'a');
		}
	
	}
	
	char testSeqStr[testSeq.size];
	
	for (int i = 0; i < testSeq.size; i++)
	{
		testSeqStr[i] = CharLists.next(&testSeq);
	}
	
	VPLists.resetCursor(&testSeq);
	
	//background
	rend->fillColor = (Color){.r=0.7, .g=0.8, .b=0.7, .a=1.0};
	Renderers.rect(rend, -1.0, 1.0, 2.0, 2.0);
	
	/* 
		render for nodes goes here
	*/
	
	//test seq box background
	rend->fillColor = (Color){.r=0.2, .g=0.2, .b=0.2, .a=1.0};
	Renderers.rect(rend, -1.0, 1.0, 2.0, 0.2);
	
	//test seq box foreground
	rend->fillColor = (Color){.r=0.9, .g=1.0, .b=0.9, .a=1.0};
	Renderers.rect(rend, -0.9, 0.99, 1.8, 0.18);
	Renderers.circle(rend, -0.9, 0.9, 0.09, 3.0);
	Renderers.circle(rend, 0.9, 0.9, 0.09, 3.0);
	
	//test seq text
	// might want to be able to scroll around based on what transition is
	// being run and when the text overflows
	//  overflows at 16 characters
	rend->strokeColor = (Color){.r=0.0, .g=0.0, .b=0.0, .a=1.0};
	rend->strokeWeight = 0.02;
	Renderers.text(rend, -0.95, 0.96, 2.0, testSeqStr, testSeq.size);
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
	.click = click,
	.free = freePage
};
