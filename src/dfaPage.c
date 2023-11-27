#include "dfaPage.h"

#define EDITNODE 0
#define LINKNODES 1
#define SETSTART 2
#define SETENDS 3
#define RUNNING 4

static int btnDFAdown = 0;
static List testSeq;
static List DFANodes;
static int init = 0;
static int pageState;
static DFANode* startNode;
static List endNodes;

static void freePage()
{
	CharLists.delAll(&testSeq);
}

static void drawTestSeqBox(Renderer* rend)
{

	char testSeqStr[testSeq.size];
	
	for (int i = 0; i < testSeq.size; i++)
	{
		testSeqStr[i] = CharLists.next(&testSeq);
	}
	
	CharLists.resetCursor(&testSeq);
	
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

static void drawButtons(Renderer* rend)
{
	rend->fillColor = (Color){.r=0.2, .g=0.2, .b=0.2, .a=1.0};
	rend->strokeColor = (Color){.r=0.9, .g=1.0, .b=0.9, .a=1.0};
	rend->strokeWeight = 0.01;
	
	Renderers.circle(rend, 0.6, -0.9, 0.1, 3.0);
	Renderers.text(rend, 0.52, -0.85, 0.6, "Set\nEnds", 8);
	if (pageState == SETENDS)
	{
		Renderers.circle(rend, 0.6, -0.75, 0.04, 3.0);
	}
	
	Renderers.circle(rend, 0.2, -0.9, 0.1, 3.0);
	Renderers.text(rend, 0.12, -0.85, 0.6, "Set\nStart", 9);
	if (pageState == SETSTART)
	{
		Renderers.circle(rend, 0.2, -0.75, 0.04, 3.0);
	}
	
	Renderers.circle(rend, -0.2, -0.9, 0.1, 3.0);
	Renderers.text(rend, -0.28, -0.85, 0.6, "Link\nNodes", 10);
	if (pageState == LINKNODES)
	{
		Renderers.circle(rend, -0.2, -0.75, 0.04, 3.0);
	}
	
	Renderers.circle(rend, -0.6, -0.9, 0.1, 3.0);
	Renderers.text(rend, -0.68, -0.85, 0.6, "Edit\nNode", 9);
	if (pageState == EDITNODE)
	{
		Renderers.circle(rend, -0.6, -0.75, 0.04, 3.0);
	}
	
	Renderers.circle(rend, 0.0, -0.8, 0.1, 3.0);
	rend->fillColor = (Color){.r=0.2, .g=1.0, .b=0.2, .a=1.0};
	Renderers.triangle(rend, -0.02, -0.77, -0.02, -0.83, 0.02, -0.8);
	if (pageState == RUNNING)
	{
		Renderers.circle(rend, 0.0, -0.8, 0.05, 3.0);
	}
}

static void draw(Renderer* rend)
{
	if (init == 0) 
	{
		testSeq = newList;
		DFANodes = newList;
		pageState = EDITNODE;
		
		init = 1;
	}
	
	//background
	rend->fillColor = (Color){.r=0.7, .g=0.8, .b=0.7, .a=1.0};
	Renderers.rect(rend, -1.0, 1.0, 2.0, 2.0);
	
	rend->fillColor = (Color){.r=0.2, .g=0.2, .b=0.2, .a=0.5};
	for (int i = 0; i < DFANodes.size; i++)
	{
		DFANode* node = DFANodeLists.next(&DFANodes);
		
		Renderers.circle(rend, node->x, node->y, 0.1, 1.0);
	}
	DFANodeLists.resetCursor(&DFANodes);
	
	drawButtons(rend);
	drawTestSeqBox(rend);
}

static float dist(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
}

static void leftClick(AppData* appdata, int action)
{
	double mouseX = appdata->mouseX;
	double mouseY = appdata->mouseY;
	
	if (action == GLFW_PRESS)
	{
		printf("LEFT PRESSED AT: %f, %f\n", mouseX, mouseY);
		
		if (pageState == RUNNING)
		{
			return;
		}
		
		if (pageState == EDITNODE
			&& mouseY < 0.7 && mouseY > -0.6
			&& mouseX < 0.9 && mouseX > -0.9)
		{
			int collisionCheck = 1;
			for (int i = 0; i < DFANodes.size; i++)
			{
				DFANode* node = DFANodeLists.next(&DFANodes);

				
				if (dist(mouseX, mouseY,
					node->x, node->y) < 0.4)
				{
					collisionCheck = 0;
					break;
				}
			}
			
			DFANodeLists.resetCursor(&DFANodes);
			
			if (collisionCheck == 1)
			{
				DFANodeLists.push(&DFANodes,
					DFANodes.size+1, mouseX, mouseY);
			}
		}
		
		if (dist(mouseX, mouseY, -0.6, -0.9) < 0.2)
		{
			pageState = EDITNODE;
		}
		if (dist(mouseX, mouseY, -0.2, -0.9) < 0.2)
		{
			pageState = LINKNODES;
		}
		if (dist(mouseX, mouseY, 0.2, -0.9) < 0.2)
		{
			pageState = SETSTART;
		}
		if (dist(mouseX, mouseY, 0.6, -0.9) < 0.2)
		{
			pageState = SETENDS;
		}
		if (dist(mouseX, mouseY, 0.0, -0.8) < 0.2)
		{
			pageState = RUNNING;
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

static void keyPress(AppData* appdata, int key, int action)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) 
	{
		if (key == GLFW_KEY_BACKSPACE) {
			CharLists.pop(&testSeq);
		}
		else
		{
			CharLists.push(&testSeq, (char)key);
		}
	}
}

const Page dfaPage =
{
	.draw = draw,
	.click = click,
	.keyPress = keyPress,
	.free = freePage
};
