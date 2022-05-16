
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "conio.h"
#include <crtdbg.h>
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include"gamecontrol.h"
#include"parameter.h"
#include"lightgui.h"
#include "stateManager.h"
#include"stageinit.h"
//#pragma comment(lib,"winmm.lib")
#define CRTDBG_MAP_ALLOC



void Main()
{
		SetWindowSize(GraphicsWindowWidth, GraphicsWindowHeight);
		InitGraphics();
		InitConsole();
		InitGUI();
//		Enable();
		InitSetting();
		StageSetting();
		registerTimerEvent(render);
		StateManagerInit(&statemanager);
		startTimer(RENDER,RENDERGAP);
}

