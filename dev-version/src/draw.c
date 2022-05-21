#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "conio.h"
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include"parameter.h"
#include"draw.h"
#include "gamecontrol.h"
typedef struct dot {
	double x, y;
	struct dot *next;
}DOT;
typedef struct line {
	DOT* headDot;
	int duration;
	struct line* next;
}LINE;
LINE* lineUnion = NULL; //the linklist for all lines drawn.
static void AddLine(LINE* newLine);
static void AddDot(DOT *newDot);
static void FreeDotList(LINE*line);
static void FreeFirstLine(void);
void DrawingRouter(double x1, double y1, int button, int event)
{

	MouseX = x1;
	MouseY = y1;
	if (button == VK_LBUTTON)
	{
		if (event == BUTTON_DOWN)
		{
			if (myrole.colorvolume <= 0)
				return;
			LINE *line = GetBlock(sizeof(LINE));
			DOT *lineHead = GetBlock(sizeof(DOT));
			line->headDot = lineHead;
			line->duration = LINEDURATION;
			line->next = NULL;
			AddLine(line);
			lineHead->x = x1;
			lineHead->y = y1;
			lineHead->next = NULL;
			startTimer(DRAW, RENDERGAP);
		}
		else if (event == BUTTON_UP) {
			cancelTimer(DRAW);
		}
	}

}
void AddLine(LINE* newLine)
{
	LINE* p = lineUnion;
	if (p == NULL)
	{
		lineUnion = newLine;
		return;
	}
	while (p->next != NULL)
		p = p->next;
	p->next = newLine;
	return;
}

void AddDot(DOT *newDot)
{
	LINE* p = lineUnion;
	DOT* dot = NULL;
	while (p->next != NULL)
		p = p->next;
	dot = p->headDot;
	if (dot == NULL)
	{
		p->headDot = newDot;
		return;
	}
	while (dot->next != NULL)
		dot = dot->next;
	dot->next = newDot;
	return;
}

void PickUpDots(void)
{
	if (myrole.colorvolume <= 0)
	{
		cancelTimer(DRAW);
		return;
	}
	if (MouseButtonIsDown == FALSE)
		return;
	DOT *p;
	p = GetBlock(sizeof(DOT));
	p->x = MouseX;
	p->y = MouseY;
	p->next = NULL;
	AddDot(p);
	myrole.colorvolume = myrole.colorvolume - VOLUMEREDUCINGSPEED;
}

void DrawSavedLines(void)
{
	LINE*p = NULL;
	DOT*prev = NULL;
	DOT*pres = NULL;
	int colorFactor;
	for (p = lineUnion; p != NULL; p = p->next)
	{
		pres = p->headDot;
		colorFactor = 255 - p->duration - 5;

		if (p->duration == 0)
		{
			FreeFirstLine();
			p = lineUnion;
			if (p == NULL)
				return;
			continue;
		}
		NewSetPenColor(colorFactor, colorFactor, colorFactor);
		SetPenSize(3);
		prev = pres;
		while (pres != NULL)
		{
			MovePen(prev->x, prev->y);
			DrawLine(pres->x - prev->x, pres->y - prev->y);
			prev = pres;
			pres = pres->next;
		}
		NewSetPenColor(255, 255, 255);
		if (p->duration > 0)
			p->duration = p->duration - 1;
		SetPenSize(1);
	}
}

void FreeFirstLine(void)
{
	LINE*p = lineUnion;

	if (p->duration == 0)
	{
		FreeDotList(p);
		lineUnion = p->next;
	}

}

void FreeDotList(LINE*line)
{
	DOT*p = line->headDot;
	DOT*rubbish = NULL;
	while (p != NULL)
	{
		rubbish = p;
		p = p->next;
		free(rubbish);
	}
}
