#include "stateManager.h"
#include "menu.h"
#include"gamecontrol.h"
#ifndef CurrentRank //乱写的，蹲一波当前表示关卡的参数
#define CurrentRank
#endif

//菜单功能
void Continue(void);
void setMenu(void);
void GetBackToMainMenu(void);
void ToHelp(void);
void NewGame(void);
void ExitGame();
void ToArchiveManagement();

//关卡选择功能（未完）
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
	//TO DO:根据存档，同步关卡信息
	StatePush(&StageMap);

}

void setMenu()
{
	double x = GetWindowWidth() / 2;
	double half = 4.45;
	setLabel(x - half,8,30,"这是一个游戏标题");
	setButton(x-half, 2.5, 0.1 , 2.3, 0.6, "continue.bmp", "继续游戏", Continue);
	setButton(x - half+3.3, 2.5, 0.1, 2.3, 0.6, "new.bmp", "新建游戏", NewGame);
	setButton(x - half+6.6, 2.5, 0.1, 2.3, 0.6, "exit.bmp", "退出游戏", ExitGame);
	setButton(x - half, 1.5, 0.1, 2.3, 0.6, "archive.bmp", "存档管理", ToArchiveManagement);
	setButton(x - half + 3.3, 1.5, 0.1, 2.3, 0.6, "help.bmp", "查看帮助", ToHelp);
}

void setStageMap()
{
	
	setButton(1, 3, 0.2, 3.5, 0.7, "", (CurrentRank>=1?"1":"lock.bmp"), LoadStage1);
	setButton(8.5, 2, 0.1, 2.3, 0.6, "", "返回主菜单", GetBackToMainMenu);

}


void GetBackToMainMenu()
{
	StatePop("MAINMENU");//或StatePop(NULL)
}

void NewGame()
{
	//TO DO:初始化一个存档，
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
	//伪代码，参数是乱写的hhh等具体规范制定完再改
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
