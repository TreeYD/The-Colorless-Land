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
#include "menu.h" //ÔÚÐ´ÁËÔÚÐ´ÁË.jpg
#include "stageinit.h"
extern State MainMenu;
extern StateManager statemanager;
static void StateChangedEvent();

void StateManagerInit() {
	statemanager.top = -1;
	ReadArchFile();//从文件中读取存档到archBuf
	StatePush(&MainMenu);
}

void StateFree() {
	do {
		StatePop(NULL);
	} while (statemanager.top > -1);
}

void StatePush(State *state) {
	StateChangedEvent();
	if(state!=&MainMenu)
	{
		State* top = StateTop();
	if (strlen(top->name) == 1)
	{
		CancelControlTimer();
	}
	}
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
	char lastName[15] = { 0 };
	strcpy(lastName, top->name);
	if(name!=NULL)
	do {
		strcpy(lastName, top->name);
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
		if(top->init!=NULL&&strcmp(lastName,"PAUSEMENU")!=0&&!(strcmp(lastName,"HELPMENU")==0&&strlen(top->name)==1))
			top->init();
		if (strlen(top->name) == 1)
			SettingMisc();
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

	stateRender = NULL;
	CacheSorting();
}
