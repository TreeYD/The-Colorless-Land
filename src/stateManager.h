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
	fnPtr init;//init()��ɰ�������ɫ������ʼ������ͼ��ʼ�������˳�ʼ����ui��ʼ��etc��������Ҫ������ͼ������ִֻ��һ�Σ���Ҫ�ظ�ִ�е������draw�
	fnPtr draw;//�������л��Ƹ�״̬�Ļ�ͼ�������ᱻ����stateRender�����֡�ʱ��ظ����ã�������Ҫ�Ѹ���ͼû��ϵ�ĺ����������������
	fnPtr destroy;//���ٸ�״̬
	keyboardCallback keyboardCallbackfn;//��״̬��Ӧ�ļ��̻ص���������Ϸ�������������������ڹ���ҳ����ݼ���
	mouseCallback mouseCallbackfn;//��״̬��Ӧ�����ص�
}State;


typedef struct {
	State * stack[STATENUMBER];
	int top;
} StateManager;

StateManager statemanager;
void(*stateRender)(void);//��RENDER��ʱ���е����������ָ�룬�Ե��ò�ͬState��Ļ滭����
void StateManagerInit();
void StateFree();
void StatePush(State *state);
void StatePop(string name);//���ǵ�����ֲ�ֹһ��״̬��ջ����ջ״̬��������������������ӹؿ�����ص����˵���ؿ�ѡ��ҳ��������һ������Ϊ���ջ�������֣���ΪNULL��Ĭ�ϳ�ջһ��״̬��
State *StateTop();
#endif

/*�����ܵģ�Main����example:

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