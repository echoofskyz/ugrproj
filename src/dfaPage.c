#include "dfaPage.h"

#define EDITNODE 0
#define LINKNODES 1
#define WRITELINK 2
#define SETSTART 3
#define SETENDS 4
#define RUNNING 5

static int btnDFAdown = 0;
static List testSeq;
static List DFANodes;
static List DFALinks;
static int init = 0;
static int pageState;
static DFANode* startNode;
static DFANode* linkOne;
static DFALink* selectedLink;

static void freePage()
{
	CharLists.delAll(&testSeq);
	DFANodeLists.delAll(&DFANodes);
	DFALinkLists.delAll(&DFALinks);
	
	//these should be freed when I clear the lists
	// but not sure?
	free(startNode);
	free(linkOne);
	free(selectedLink);
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

static void drawLinks(Renderer* rend)
{
	rend->strokeColor = (Color){.r=0.0, .g=0.0, .b=0.0, .a=1.0};
	rend->strokeWeight = 0.005;
	for (int i = 0; i < DFALinks.size; i++)
	{
		DFALink* link = DFALinkLists.next(&DFALinks);
		char transString[link->transitions.size];
		for (int n = 0; n < link->transitions.size; n++)
		{
			transString[n] = CharLists.next(&link->transitions);
		}
		CharLists.resetCursor(&link->transitions);
		float mX = link->first->x - 
			(link->first->x - link->second->x)/2.0;
		
		float mY = link->first->y - 
			(link->first->y - link->second->y)/2.0;
		
		if (link->first->x < link->second->x - 0.2)
		{
			Renderers.text(rend, mX, mY+0.05,
				0.6, transString, link->transitions.size);
			Renderers.line(rend, mX-0.05, mY+0.055, mX-0.025, mY+0.04);
			Renderers.line(rend, mX-0.05, mY+0.025, mX-0.025, mY+0.04);
		}
		else if (link->first->x > link->second->x + 0.2)
		{
			Renderers.text(rend, mX, mY-0.025,
				0.6, transString, link->transitions.size);
			Renderers.line(rend, mX-0.025, mY-0.055, mX-0.05, mY-0.04);
			Renderers.line(rend, mX-0.025, mY-0.025, mX-0.05, mY-0.04);
		}
		else if (link->first->y > link->second->y) {
			Renderers.text(rend, mX, mY+0.05,
				0.6, transString, link->transitions.size);
			Renderers.line(rend, mX-0.075, mY+0.055, mX-0.05, mY+0.04);
			Renderers.line(rend, mX-0.025, mY+0.055, mX-0.05, mY+0.04);
		}
		else if (link->first->y < link->second->y) {
			Renderers.text(rend, mX, mY-0.025,
				0.6, transString, link->transitions.size);
			Renderers.line(rend, mX-0.075, mY-0.055, mX-0.05, mY-0.04);
			Renderers.line(rend, mX-0.025, mY-0.055, mX-0.05, mY-0.04);
		}
		Renderers.line(rend, link->first->x, link->first->y,
			link->second->x, link->second->y);
	}
	DFALinkLists.resetCursor(&DFALinks);
}

static void draw(Renderer* rend)
{	
	if (init == 0) 
	{
		testSeq = newList;
		DFANodes = newList;
		pageState = EDITNODE;
		DFALinks = newList;
		
		init = 1;
	}
	
	if (pageState != LINKNODES)
	{
		linkOne = NULL;
	}
	if (pageState != WRITELINK)
	{
		selectedLink = NULL;
	}
	
	//background
	rend->fillColor = (Color){.r=0.7, .g=0.8, .b=0.7, .a=1.0};
	Renderers.rect(rend, -1.0, 1.0, 2.0, 2.0);
	
	drawLinks(rend);
	
	//draw nodes
	for (int i = 0; i < DFANodes.size; i++)
	{
		DFANode* node = DFANodeLists.next(&DFANodes);
		
		rend->fillColor = (Color){.r=0.2, .g=0.2, .b=0.2, .a=0.5};
		Renderers.circle(rend, node->x, node->y, 0.1, 1.0);
		
		if (node->isAccept)
		{
			rend->fillColor = (Color){.r=0.7, .g=0.2, .b=0.2, .a=1.0};
			Renderers.circle(rend, node->x, node->y, 0.08, 1.0);
		}
	}
	DFANodeLists.resetCursor(&DFANodes);
	
	
	//draw start node
	if (startNode)
	{
		rend->fillColor = (Color){.r=0.4, .g=1.0, .b=0.4, .a=1.0};
		Renderers.circle(rend, startNode->x, startNode->y, 0.05, 1.0);
	}
	
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
		
		if (pageState == WRITELINK)
		{
			if (selectedLink->transitions.size == 0)
			{
				DFALinkLists.delNode(&DFALinks, selectedLink);
			}
			pageState = LINKNODES;
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
		
		if (pageState == LINKNODES)
		{
			for (int i = 0; i < DFANodes.size; i++)
			{
				DFANode* node = DFANodeLists.next(&DFANodes);

				if (dist(mouseX, mouseY,
					node->x, node->y) < 0.1)
				{
					if (linkOne == NULL)
					{
						linkOne = node;
					} 
					else if (node != linkOne)
					{
						int foundLink = 0;
						for (int n = 0; n < DFALinks.size; n++)
						{
							DFALink* link = DFALinkLists.next(&DFALinks);
							if (link->first == linkOne 
								&& link->second == node)
							{
								selectedLink = link;
								foundLink = 1;
								break;
							}
						}
						DFALinkLists.resetCursor(&DFALinks);
						
						if (foundLink == 0)
						{
							DFALinkLists.push(&DFALinks, linkOne, node);
							selectedLink = DFALinkLists.peek(&DFALinks);
						}
						
						linkOne = NULL;
						pageState = WRITELINK;
					}
					
					break;
				}
			}
			DFANodeLists.resetCursor(&DFANodes);
		} 
		
		if (pageState == SETSTART)
		{
			for (int i = 0; i < DFANodes.size; i++)
			{
				DFANode* node = DFANodeLists.next(&DFANodes);
								
				if (dist(mouseX, mouseY,
					node->x, node->y) < 0.1)
				{
					startNode = node;
					break;
				}
			}
			
			DFANodeLists.resetCursor(&DFANodes);
		}
		
		if (pageState == SETENDS)
		{
			for (int i = 0; i < DFANodes.size; i++)
			{
				DFANode* node = DFANodeLists.next(&DFANodes);
								
				if (dist(mouseX, mouseY,
					node->x, node->y) < 0.1)
				{
					if (node->isAccept)
					{
						node->isAccept = 0;
					}
					else
					{
						node->isAccept = 1;
					}
					
					break;
				}
			}
			
			DFANodeLists.resetCursor(&DFANodes);
		}
		
		if (dist(mouseX, mouseY, -0.6, -0.9) < 0.1)
		{
			pageState = EDITNODE;
		}
		if (dist(mouseX, mouseY, -0.2, -0.9) < 0.1)
		{
			pageState = LINKNODES;
		}
		if (dist(mouseX, mouseY, 0.2, -0.9) < 0.1)
		{
			pageState = SETSTART;
		}
		if (dist(mouseX, mouseY, 0.6, -0.9) < 0.1)
		{
			pageState = SETENDS;
		}
		if (dist(mouseX, mouseY, 0.0, -0.8) < 0.1)
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
		if (pageState == WRITELINK)
		{
			if (key == GLFW_KEY_BACKSPACE) {
				CharLists.pop(&selectedLink->transitions);
			}
			else if (key == GLFW_KEY_ENTER)
			{
				if (selectedLink->transitions.size == 0)
				{
					DFALinkLists.delNode(&DFALinks, selectedLink);
				}
				pageState = LINKNODES;
			}
			else
			{
				CharLists.push(&selectedLink->transitions, (char)key);
			}
			return;
		}
		
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
