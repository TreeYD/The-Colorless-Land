//===========================================================================
//
//  Author: Mr.Tree
//  Functions: 
//		1)button
//		2)textbox
//		3)seekbar
//		4)label
//	PS: based on libgraphics and refers to simpleGUI
//===========================================================================

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include "gcalloc.h"
#include "lightgui.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>


typedef struct {
	double mX;
	double mY;
	bool inText,inDrag;
	int event, button;
}*UIState;

 


static UIState curState;
static BUTTON headButton;
static TEXTBOX headTextbox,curTextbox;
static SEEKBAR headSeekbar;

//inBox check if the mouse is in textbox/button
//inCircle check if the mouse is in seekbar
bool inBox(double x, double y, double w, double h);
bool inCircle(double x, double y, double r);

void drawBox(double x, double y,double r, double w, double h);
static double MIN(double x, double y);
static double MAX(double x, double y);

/*TODO: Button Process*/
void insertButton(BUTTON ptr);

/*TODO: Seekbar Process*/
void insertSeekbar(SEEKBAR ptr);

/*TODO: Textbox Process*/
void insertTextbox(TEXTBOX ptr);
void drawCursor(TEXTBOX ptr);
void Insert(int id, char c);
void Delete(int id);

bool inBox(double x, double y, double w, double h) {
	return curState->mX > x&&curState->mX < x + w && curState->mY>y&&curState->mY<y + h;
}
bool inCircle(double x, double y, double r) {
	return ((curState->mX - x)*(curState->mX - x) + (curState->mY - y)*(curState->mY - y)) < r*r;
}
void uiMouseEvent(int x,int y,int button, int event) {
	//Update the current state
	curState->mX = ScaleXInches(x);
	curState->mY = ScaleYInches(y);
	curState->button = button;
	curState->event = event;
	//Update the whole elements
	traverseButton();
	traverseSeekbar();
	traverseTextbox();
	//displayAll;
}
void uiKeyboardEvent(int key,int event) {
	if (!curState->inText)return;
	SetEraseMode(TRUE);
	drawCursor(curTextbox);
	drawTextbox(curTextbox);
	SetEraseMode(FALSE);
	switch (event) {
	case KEY_DOWN:
		switch (key) {
		case VK_DELETE: {
			if (curTextbox->curPos == strlen(curTextbox->text))break;
			Delete(curTextbox->curPos);
			break;
		}
		case VK_LEFT: {
			if (curTextbox->curPos == 0)break;
			curTextbox->curPos--;
			break;
		}
		case VK_RIGHT: {
			if (curTextbox->curPos == strlen(curTextbox->text))break;
			curTextbox->curPos++;
			break;
		}
		default:break;
		}
		break;
	default:
		break;
	}
	drawTextbox(curTextbox);
	return;
}

void uiCharEvent(char c)
{
	if (!curState->inText)return;
	SetEraseMode(TRUE);
	drawCursor(curTextbox);
	drawTextbox(curTextbox);
	SetEraseMode(FALSE);
	switch (c) {
		case '\r': {
			curState->inText = 0;
			cancelTimer(CURSOR_BLINK);
			break;
		}
		case '\b': {
			if (curTextbox->curPos == 0)break;
			Delete(curTextbox->curPos-1);
			curTextbox->curPos--;
			break;
		}
		default: {
			if (strlen(curTextbox->text) == curTextbox->maxLength)break;
			Insert(curTextbox->curPos , c);
			curTextbox->curPos++;
			break;
		}
	}
	drawTextbox(curTextbox);
	return;
}

void InitGUI() {
	curState = GetBlock(sizeof(*curState));
	curState->inText = 0;
	curState->inDrag = 0;
	DefineColor("ButtonShadow", .95, .95, .95);
	DefineColor("TextGrey", .8, .8, .8);
}

 /*TODO: Button Process*/

void drawBox(double x, double y,double r ,double w, double h) {
	SetPenSize(1);
	MovePen(x, y+r);
	DrawArc(r, 180, 90);
	DrawLine(w-2*r,0);
	DrawArc(r, 270, 90);
	DrawLine(0, h-2*r);
	DrawArc(r, 0, 90);
	DrawLine(-(w-2*r), 0);
	DrawArc(r, 90, 90);
	DrawLine(0, -(h-2*r));
	return;
}
static double MIN(double x, double y)
{
	return x<y?x:y;
}
static double MAX(double x, double y)
{
	return x > y ? x : y;
}
void insertButton(BUTTON ptr) {
	BUTTON temp = headButton;
	if (temp == NULL) {
		headButton = ptr;
		return;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = ptr;
	return;
}

void drawButton(BUTTON ptr,bool fill) {
	if (fill) StartFilledRegion(1);
	drawBox(ptr->x, ptr->y, ptr->r,ptr->w, ptr->h);
	if(fill)EndFilledRegion();
	drawBox(ptr->x, ptr->y, ptr->r,ptr->w, ptr->h);
	double length = TextStringWidth(ptr->text);
	if (ptr->iconAddress != "")length += ptr->h*0.7;
	double staX = ptr->x + (ptr->w - length) / 2;
	if(!fill)AddZoomBitMap(ptr->iconAddress, staX, ptr->y+0.15*ptr->h, ptr->h*0.7, ptr->h*0.7, SRCAND);
	if (ptr->iconAddress != "")staX += ptr->h*0.7;
	setLabel(staX, ptr->y + ptr->h / 3, ScalePixels(ptr->h / 3), ptr->text);
}
BUTTON setButton(double x,double y,double r,double w,double h,string icon,string text,void *func) {
	BUTTON ptr=GetBlock(sizeof(*ptr));
	*ptr = (struct button) { x, y, r,w, h, icon,text,func,NULL};
	drawButton(ptr, NOTFILL);
	insertButton(ptr);
	return ptr;
}
void traverseButton() {
	BUTTON ptr = headButton;
	while (ptr != NULL) {
		if (inBox(ptr->x, ptr->y, ptr->w, ptr->h)) {
			if (curState->button==LEFT_BUTTON&&curState->event == BUTTON_DOWN) {
				ptr->clickEvent();
			}
			if (ptr == NULL)break;
			SetPenColor("ButtonShadow");
			drawButton(ptr, FILL);
			SetPenColor("black");
			drawButton(ptr, NOTFILL);
		}else {
			SetPenColor("white");
			drawButton(ptr, FILL);
			SetPenColor("black");
			drawButton(ptr, NOTFILL);
		}
		if(ptr!=NULL)ptr = ptr->next;
	}
	return;
}

void freeButton()
{
	BUTTON *ptr = &headButton;
	BUTTON tmp;
	while (*ptr != NULL) {
		
		tmp = *ptr;
		*ptr = tmp->next;
		free(tmp);
	}
	return;
}

 /*TODO: Label Process*/

void setLabel(double x, double y, int fontSize, string text) {
	MovePen(x, y);
	SetPointSize(fontSize);
	DrawTextString(text);
	return;
}

void insertSeekbar(SEEKBAR ptr)
{
	SEEKBAR temp = headSeekbar;
	if (temp == NULL) {
		headSeekbar = ptr;
		return;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = ptr;
	return;
}

void drawSeekbar(SEEKBAR ptr)
{
	SetPenColor("black");
	StartFilledRegion(1);
	drawBox(ptr->x, ptr->y, ptr->h / 2, ptr->dw, ptr->h);
	EndFilledRegion();
	drawBox(ptr->x + ptr->dw, ptr->y, ptr->h / 2, ptr->w - ptr->dw, ptr->h);
	MovePen(ptr->x + ptr->dw + ptr->h *0.75, ptr->y + ptr->h / 2);
	SetPenColor("white");
	StartFilledRegion(1);
	DrawArc(ptr->h*0.75, 0, 360);
	EndFilledRegion();
	SetPenColor("black");
	DrawArc(ptr->h*0.75, 0, 360);
}

SEEKBAR setSeekbar(double x, double y,double w, double h,double* Obj)
{
	SEEKBAR ptr = GetBlock(sizeof(*ptr));
	*ptr = (struct seekbar) { x, y, w,h, *Obj, Obj,NULL };
	drawSeekbar(ptr);
	insertSeekbar(ptr);
	return ptr;
}

void traverseSeekbar()
{
	if (curState->event == BUTTON_UP) {
		curState->inDrag = 0;
		return;
	}
	if (!curState->inDrag&&curState->event != BUTTON_DOWN)return;
	if (curState->button == RIGHT_BUTTON)return;
	SEEKBAR ptr = headSeekbar;
	while (ptr != NULL) {
		if (inCircle(ptr->x + ptr->dw, ptr->y + ptr->h / 2, ptr->h*0.75)) {
			if (curState->button == LEFT_BUTTON && curState->event == BUTTON_DOWN) {
				curState->inDrag = 1;
			}
		}
		if (curState->inDrag&&curState->event == MOUSEMOVE) {
			SetEraseMode(TRUE);
			drawSeekbar(ptr);
			SetEraseMode(FALSE);
			ptr->dw = MIN(ptr->w, MAX(0,curState->mX - ptr->x));
			*(ptr->controlObj) = ptr->dw / ptr->w * 100;
		}
		drawSeekbar(ptr);
		if(ptr!=NULL)ptr = ptr->next;
	}
	return;
}

void freeSeekbar()
{
	SEEKBAR *ptr = &headSeekbar;
	SEEKBAR tmp;
	while (*ptr != NULL) {
		tmp = *ptr;
		*ptr = tmp->next;
		free(tmp);
	}
	return;
}

void insertTextbox(TEXTBOX ptr)
{
	TEXTBOX temp = headTextbox;
	if (temp == NULL) {
		headTextbox = ptr;
		return;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = ptr;
	return;
}

void drawTextbox(TEXTBOX ptr)
{
	if (ptr == NULL)return;
	drawBox(ptr->x, ptr->y, ptr->r, ptr->w, ptr->h);
	SetPointSize(ptr->fontSize);;
	double iv = ScaleXInches(ptr->fontSize),staX= ptr->x + iv,staY=ptr->y+ptr->h/3;
	MovePen(staX, staY);
	if (ptr->isFirst) {
		SetPenColor("TextGrey");
		DrawTextString(ptr->hint);
		SetPenColor("black");
	}
	else { 
		DrawTextString(ptr->text); 
	}
	return;
}

void drawCursor(TEXTBOX ptr)
{
	if (ptr == NULL)return;
	if (ptr->curPos<0 || ptr->curPos>strlen(ptr->text))return;
	SetPointSize(ptr->fontSize);
	double staX, staY = ptr->y + ptr->h / 3,iv = ScaleXInches(ptr->fontSize);
	staX = ptr->x + iv + TextStringWidth(SubString(ptr->text, 0,ptr->curPos-1));
	MovePen(staX, staY);
	DrawLine(0, iv);
	return;
}

TEXTBOX setTextbox(double x, double y, double r,double w, double h,string hint,int maxLength)
{
	TEXTBOX ptr = GetBlock(sizeof(*ptr));
	*ptr = (struct textbox) { x, y, r,w, h, 0, 0,0, "\0", hint,maxLength,NULL ,ScalePixels(h/3),1};
	drawTextbox(ptr);
	insertTextbox(ptr);
	curTextbox = ptr;
	return ptr;
}

void traverseTextbox()
{	
	if (curState->button != LEFT_BUTTON || curState->event != BUTTON_DOWN)return;
	TEXTBOX ptr=headTextbox;
	bool isClickText=0;
	while (ptr != NULL) {
		if (inBox(ptr->x, ptr->y, ptr->w, ptr->h)) {
			isClickText = 1;
			SetPointSize(ptr->fontSize);
			int length = 0;
			double staX = ptr->x + ScaleXInches(ptr->fontSize);
			while (length < (int)(strlen(ptr->text))) {
				if (TextStringWidth(SubString(ptr->text, 0,  length)) < curState->mX-staX)
					length++;
				else break;
			}
			startTimer(CURSOR_BLINK);
			curTextbox = ptr;
			SetEraseMode(TRUE);
			drawCursor(curTextbox);
			drawTextbox(curTextbox);
			SetEraseMode(FALSE);
			ptr->curPos = length;
			ptr->isFirst = 0;
		}
		drawTextbox(ptr);
		ptr = ptr->next;
	}
	if (isClickText == 1) {
		startTimer(CURSOR_BLINK,timerseconds);
	}
	else {
		SetEraseMode(TRUE);
		drawCursor(curTextbox);
		SetEraseMode(FALSE);
		cancelTimer(CURSOR_BLINK);
	}
	curState->inText = isClickText;
	return;
}

void uiTimeEvent(int timerID)
{
	switch (timerID) {
		case CURSOR_BLINK: 
			SetEraseMode(!curTextbox->isDisplayed);
			drawCursor(curTextbox);
			SetEraseMode(FALSE);
			drawTextbox(curTextbox);
			curTextbox->isDisplayed=!curTextbox->isDisplayed;
			break;
		default:
			break;
	}
}
void Delete(int id) {
	int len = strlen(curTextbox->text);
	while (id < len) {
		curTextbox->text[id] = curTextbox->text[id + 1];
		id++;
	}
	return;
}
void freeTextbox()
{
	TEXTBOX *ptr = &headTextbox;
	TEXTBOX tmp;
	while (*ptr != NULL) {
		tmp = *ptr;
		*ptr = tmp->next;
		free(tmp);
	}
	curTextbox = NULL;
	return;
}
void Insert(int id, char c) {
	int k = strlen(curTextbox->text);
	while (k >= id) {
		curTextbox->text[k + 1] = curTextbox->text[k];
		k--;
	}
	curTextbox->text[id] = c;
	return;
}
// This part is added by Ag2SO4
//	tql %%%
/*cache the previous buttons, release them next time to avoid that 
Mouse / timer callbacks may access memory that has been freed while in progress this time*/
void CacheSorting(void)
{
	static BUTTON tmp = NULL;
	if (tmp == NULL)
	{
		tmp = headButton;
		headButton = NULL;
	}
	else
	{
		BUTTON rubbish = NULL;
		while (tmp != NULL)
		{
			rubbish = tmp;
			tmp = tmp->next;
			free(rubbish);
		}
		tmp = headButton;
		headButton = NULL;
	}
}