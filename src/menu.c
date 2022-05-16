#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <wincon.h>
#include <Windows.h>
#include "graphics.h"
#include"gamecontrol.h"
#include "stateManager.h"
#include "menu.h"
#include"helpAndPause.h"
#include "lightgui.h"
#include "stageinit.h"
int CurrentRank = 1;
extern State* StageArray[StageNum];
/*CurrentRank: temporary variable to represent present stage process. Waiting to be changed.*/

//function for main menu
void Continue(void);
void setMenu(void);
void drawMenu(void);
void GetBackToMainMenu(void);
void ToHelp(void);
void NewGame(void);
void ExitGame(void);
void ToArchiveManagement(void);

//function for stage selection
void setStageMap(void);
void LoadStage1(void);
void LoadStage2(void);
void LoadStage3(void);
extern State PauseMenu;
extern State HelpMenu;
State  MainMenu = {
	"MAINMENU",
	setMenu,
	drawMenu,
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
	//TO DO:load all info according to current archive
	StatePush(&StageMap);

}

void setMenu()
{
	double x = GetWindowWidth() / 2;
	double half = 4.45;
	setButton(x-half, 2.5, 0.1 , 2.3, 0.6, "continue.bmp", "继续游戏", Continue);
	setButton(x - half+3.3, 2.5, 0.1, 2.3, 0.6, "new.bmp", "新建游戏", NewGame);
	setButton(x - half+6.6, 2.5, 0.1, 2.3, 0.6, "exit.bmp", "退出游戏", ExitGame);
	setButton(x - half, 1.5, 0.1, 2.3, 0.6, "archive.bmp", "存档管理", ToArchiveManagement);
	setButton(x - half + 3.3, 1.5, 0.1, 2.3, 0.6, "help.bmp", "查看帮助", ToHelp);
}

void drawMenu()
{
	setLabel(5, 7, 30, "无色之境-THE COLORLESS LAND");
	traverseButton();
}
void setStageMap()
{
	double x = GetWindowWidth() / 2;
	double half = 3.625;
	setButton(x-half, 6, 0.2, 1.75, 1, (CurrentRank >= 1 ? "" : "lock.bmp"), (CurrentRank>=1?"1":" "), LoadStage1);
	setButton(x-half+2.75, 6, 0.2, 1.75, 1, (CurrentRank >= 2 ? "" : "lock.bmp"), (CurrentRank >= 2 ? "2" : " "), LoadStage2);
	setButton(x - half + 5.5, 6, 0.2, 1.75, 1, (CurrentRank >= 3 ? "" : "lock.bmp"), (CurrentRank >= 2 ? "3" : " "), LoadStage3);
	setButton(x+1, 2, 0.1, 2.3, 0.6, "", "返回主菜单", GetBackToMainMenu);
	setButton(x-3.3, 2, 0.1, 2.3, 0.6, "", "选择存档", ToArchiveManagement);
}


void GetBackToMainMenu()
{
	StatePop("MAINMENU");//or StatePop(NULL)
}

void NewGame()
{
	//TO DO:initialize an archive，
	StatePush(&StageMap);
}

void ExitGame()
{
	StateFree();
	//DisplayExit();
	FreeConsole();
	exit(0);
}

void ToArchiveManagement()
{
	StatePush(&PauseMenu);
	//StatePush(&ArchivePage);
}

void ToHelp()
{
	StatePush(&HelpMenu);
}
void LoadStage1(void)
{
	//伪代码，参数是乱写的hhh等具体规范制定完再改
	/*
	if (CurrentRank<1)
	{
		return;
	}*/
	StatePush(StageArray[0]);
	
}
void LoadStage2(void)
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
