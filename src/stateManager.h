#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
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
#include "graphics.h"
#include"lightgui.h"
#define STATENUMBER 5
typedef void(*fnPtr)(void);
typedef void(*keyboardCallback)(int button, int event);
typedef void(*mouseCallback)(int x, int y, int button, int event);

typedef struct {
	string name;
	fnPtr init;//init()里可包括：角色参数初始化、地图初始化、敌人初始化，ui初始化etc，尽量不要包含画图函数（只执行一次，需要重复执行的请放在draw里）
	fnPtr draw;//调用所有绘制该状态的画图函数，会被传到stateRender里根据帧率被重复调用，尽量不要把跟画图没关系的函数放在这个函数里
	fnPtr destroy;//销毁该状态
	keyboardCallback keyboardCallbackfn;//该状态对应的键盘回调（对于游戏场景：操作函数；对于功能页：快捷键）
	mouseCallback mouseCallbackfn;//该状态对应的鼠标回调
}State;


typedef struct {
	State * stack[STATENUMBER];
	int top;
} StateManager;

StateManager statemanager;
void(*stateRender)(void);//在RENDER计时器中调用这个函数指针，以调用不同State里的绘画函数
void StateManagerInit();
void StateFree();
void StatePush(State *state);
void StatePop(string name);//考虑到会出现不止一个状态出栈、出栈状态数量不定的情况（例：从关卡结算回到主菜单或关卡选择页），增加一个参数为最后栈顶的名字（若为NULL则默认出栈一个状态）
State *StateTop();
#endif

/*（能跑的）Main函数example:

void Main()
{
		SetWindowSize(GraphicsWindowWidth, GraphicsWindowHeight);
		InitGraphics();
		InitGUI();
		registerTimerEvent(render);
		StateManagerInit(&statemanager);
		startTimer(RENDER,RENDERGAP);
}
*/