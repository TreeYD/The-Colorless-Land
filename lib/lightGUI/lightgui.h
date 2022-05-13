#ifndef _lightgui_h
#define _lightgui_h
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
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

#define MAXLINE 100
#define MAXLINE 100
#define FILL 1
#define NOTFILL 0

#define CURSOR_BLINK 0xdeafbee //a small trick to let timerID not confict
#define CURSOR "|"
#define timerseconds 500

typedef struct button {
	double x, y, r;
	double w, h;//The default font size is half the height of the button
	string iconAddress;
	string text;
	void(*clickEvent)();
	struct button* next;
}*BUTTON;

typedef struct textbox {
	double x, y, r;
	double w, h;
	int curPos;
	bool isDisplayed;
	char text[MAXLINE];
	string hint;
	int maxLength;
	struct textbox* next;
	int fontSize;
	bool isFirst;
}*TEXTBOX;

//typedef struct label {
//	double x, y;
//	int fontSize;
//	char text[MAXLINE];
//	struct label *next;
//}*LABEL;

typedef struct seekbar {
	double x, y;
	double w, h, dw;//the percentage is ( dlength/length *100% )
	double *controlObj;//store the address of the controlled object
	struct seekbar *next;
}*SEEKBAR;

void uiMouseEvent(int x, int y, int button, int event);
void uiKeyboardEvent(int key, int event);
void uiCharEvent(char c);
void uiTimeEvent(int timerID);
void drawBox(double x, double y, double r,double w, double h);
//take (x,y) as the bottom left corner , draw a box with width "w" ,height "h" ,and radius "r"
BUTTON setButton(double x, double y, double r,double w, double h, string icon, string text, void *func);
//draw a button, and put a icon and text in the center of the box, the height of the font is 1/3 of the box height
SEEKBAR setSeekbar(double x, double y, double w, double h, double *Obj);
//draw a seekbar, w means the length of seekbar, h means the height of seekbar, Obj means the address of the controlled variable
TEXTBOX setTextbox(double x, double y, double r, double w, double h,string hint,int maxLength);
/*draw a textbox, 
w means the width of textbox, 
h means the height of textbox,
the font size is 1/3 of the box height,
the maxlength means the maxlength you can input*/
//and before input there will a hint in color "Grey" in the box
void setLabel(double x, double y, int fontSize, string text); //e.g. fontSize 5 means the font height is 5 px 
//draw a label, fontSize means the pixel height of the text
void drawButton(BUTTON ptr, bool fill);
void drawTextbox(TEXTBOX ptr);
void drawSeekbar(SEEKBAR ptr);

void InitGUI();
//init GUI, including define some colors, register some events, etc.

void freeTextbox();
// free all the textbox, make it disable
void freeSeekbar();
// free all the seekbar, make it disable
void CacheSorting(void);

void traverseButton();
void traverseSeekbar();
void traverseTextbox();
#endif
