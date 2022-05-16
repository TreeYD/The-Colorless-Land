
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "conio.h"
#include "linkedlist.h"
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include"moveAndDraw.h"
#include"parameter.h"
extern struct ROLE myrole;
LINE* lineUnion = NULL; //the linklist for all lines drawn.
void render(int TimerID)
{
	switch (TimerID)
	{
	case RENDER:
		DisplayClear();
		DrawSavedLines();
		AddBitMap(currPic, myrole.x, myrole.y, SRCAND);
		break;
	case RIGHTMOVING:
		playerMove(RIGHTMOVING);
		break;
	case LEFTMOVING:
		playerMove(LEFTMOVING);
		break;
	case JUMP:
		playerJump();
		break;
	case FALL:
		playerFall();
		break;
	case PICKUPDOTS:
		PickUpDots();
		break;
	}
}
void move(int key, int event)
{


	if (event == KEY_DOWN)
	{
		switch (key)
		{
		case'A':
			startTimer(LEFTMOVING, RENDERGAP);
			break;
		case'D':
			startTimer(RIGHTMOVING, RENDERGAP);
			break;
		case 'W':
			startTimer(JUMP, RENDERGAP);
			if (isFloating == FALSE)
			{
				verticalSpeed = INITIALVERTICALSPEED;
				isFloating = TRUE;
			}
			break;
		}
	}
	else if (event == KEY_UP) {
		switch (key)
		{
		case'A':
			cancelTimer(LEFTMOVING);
			break;
		case'D':
			cancelTimer(RIGHTMOVING);
			break;

		}
		currPic = PICSTILL;
	}
}
/*TODO: ADD  COLLISION DETECTING*/
void playerMove(int event)
{
	static bool currPicCount = TRUE;
	switch (event)
	{
	case RIGHTMOVING:
		myrole.x = myrole.x + xspeed;
		break;
	case LEFTMOVING:
		if (myrole.x <= xspeed)
			myrole.x = 0;
		else
			myrole.x = myrole.x - xspeed;
		break;
	}
	currPicCount = !currPicCount;
	switch (currPicCount)
	{
	case TRUE:currPic = (event == RIGHTMOVING ? PICRIGHT1 : PICLEFT1);
		break;
	case FALSE:currPic = (event == RIGHTMOVING ? PICRIGHT2 : PICLEFT2);
		break;
	}
}

void playerJump(void)
{
	if (myrole.y >= TESTY)//REMEMBER TO CHANGE LATER
	{
		myrole.y += verticalSpeed;
		verticalSpeed -= G;
	}
	if (myrole.y <= TESTY)
	{
		myrole.y = TESTY;
		isFloating = FALSE;
		cancelTimer(JUMP);
	}
}

void playerFall(void)
{
	//TO DO
}

void mouseRouter(int x, int y, int button, int event)
{
	double x1, y1;
	x1 = ScaleXInches(x);
	y1 = ScaleYInches(y);
	mouseX = x1;
	mouseY = y1;
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
			startTimer(PICKUPDOTS, RENDERGAP);
		}
		else if (event == BUTTON_UP) {
			cancelTimer(PICKUPDOTS);
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
		cancelTimer(PICKUPDOTS);
		return;
	}
	if (MouseButtonIsDown == FALSE)
		return;
	DOT *p;
	p = GetBlock(sizeof(DOT));
	p->x = mouseX;
	p->y = mouseY;
	p->next = NULL;
	AddDot(p);
	myrole.colorvolume=myrole.colorvolume- VOLUMEREDUCINGSPEED;
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
