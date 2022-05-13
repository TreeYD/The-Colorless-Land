#include "stateManager.h"
#include "menu.h"
#include"gamecontrol.h"
#ifndef CurrentRank //��д�ģ���һ����ǰ��ʾ�ؿ��Ĳ���
#define CurrentRank
#endif

//�˵�����
void Continue(void);
void setMenu(void);
void GetBackToMainMenu(void);
void ToHelp(void);
void NewGame(void);
void ExitGame();
void ToArchiveManagement();

//�ؿ�ѡ���ܣ�δ�꣩
void setStageMap(void);
void LoadStage1(void);

State  MainMenu = {
	"MAINMENU",
	setMenu,
	traverseButton,
	NULL,
	NULL,
	uiMouseEvent,
};
State StageMap = {
	"STAGEMAP",
	setStageMap,
	traverseButton,
	NULL,
	NULL,
	uiMouseEvent,
};
void Continue()
{
	//TO DO:���ݴ浵��ͬ���ؿ���Ϣ
	StatePush(&StageMap);

}

void setMenu()
{
	double x = GetWindowWidth() / 2;
	double half = 4.45;
	//setLabel(x - half,8,30,"����һ����Ϸ����");
	setButton(x-half, 2.5, 0.1 , 2.3, 0.6, "continue.bmp", "������Ϸ", Continue);
	setButton(x - half+3.3, 2.5, 0.1, 2.3, 0.6, "new.bmp", "�½���Ϸ", NewGame);
	setButton(x - half+6.6, 2.5, 0.1, 2.3, 0.6, "exit.bmp", "�˳���Ϸ", ExitGame);
	setButton(x - half, 1.5, 0.1, 2.3, 0.6, "archive.bmp", "�浵����", ToArchiveManagement);
	setButton(x - half + 3.3, 1.5, 0.1, 2.3, 0.6, "help.bmp", "�鿴����", ToHelp);
}

void setStageMap()
{
	
	setButton(1, 3, 0.2, 3.5, 0.7, "", (CurrentRank>=1?"1":"lock.bmp"), LoadStage1);
	setButton(8.5, 2, 0.1, 2.3, 0.6, "", "�������˵�", GetBackToMainMenu);

}


void GetBackToMainMenu()
{
	StatePop("MAINMENU");//��StatePop(NULL)
}

void NewGame()
{
	//TO DO:��ʼ��һ���浵��
	StatePush(&StageMap);
}

void ExitGame()
{
	StateFree();
	FreeConsole();
	exit(0);
}

void ToArchiveManagement()
{
	//StatePush(&ArchivePage);
}

void ToHelp()
{
	//StatePush(&HelpPage);
}
void LoadStage1(void)
{
	//α���룬��������д��hhh�Ⱦ���淶�ƶ����ٸ�
	/*
	if (CurrentRank<1)
	{
		return;
	}
	StatePush(&GameState[0]);
	*/
}
void LoadStage3(void)
{
	/*
	if (CurrentRank<2)
	{
		return;
	}
	StatePush(&GameState[1]);
	*/
}
void LoadStage3(void)
{
	/*
	if (CurrentRank<3)
	{
		return;
	}
	StatePush(&GameState[2]);
	*/
}
