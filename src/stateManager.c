#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include<string.h>
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
#include"lightgui.h"
#include"gamecontrol.h"
#include "stateManager.h"
#include "menu.h" //在写了在写了.jpg
extern State MainMenu;
extern StateManager statemanager;
static void StateChangedEvent();

void StateManagerInit() {
	statemanager.top = -1;
	StatePush(&MainMenu);
}

void StateFree() {
	do {
		StatePop(NULL);
	} while (statemanager.top > -1);
}

void StatePush(State *state) {
	StateChangedEvent();
	statemanager.top++;
	statemanager.stack[statemanager.top] = state;
	if(state->init!=NULL)
		state->init();
	if(state->draw!=NULL)
	stateRender = state->draw;
	registerKeyboardEvent(state->keyboardCallbackfn);
	registerMouseEvent(state->mouseCallbackfn);
	return;
}

void StatePop(string name) {
	StateChangedEvent();
	if (statemanager.top == -1) 
		return;
	State *top = StateTop();
	if(name!=NULL)
	do {
		if (top->destroy != NULL)
			top->destroy();
		statemanager.stack[statemanager.top] = NULL;
		statemanager.top--;
		if (statemanager.top < 0)
			return;
		top = StateTop();
	} while (strcmp(top->name,name)!=0);
	else
	{
		if (top->destroy != NULL)
			top->destroy();
		statemanager.stack[statemanager.top] = NULL;
		statemanager.top--;
		if (statemanager.top < 0)
			return;
	}
		top = StateTop();
		if(top->init!=NULL)
			top->init();
		if (top->draw != NULL)
			stateRender = top->draw;
		registerKeyboardEvent(top->keyboardCallbackfn);
		registerMouseEvent(top->mouseCallbackfn);
	return;
}

State *StateTop() {
	return statemanager.stack[statemanager.top];
}



void StateChangedEvent()
{
	cancelMouseEvent();
	cancelKeyboardEvent();
	stateRender = NULL;
	CacheSorting();
}