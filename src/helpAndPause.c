#include <windows.h>
#include<stdio.h>
#include"strlib.h"
#include"graphics.h"
#include<WinUser.h>
#include <time.h>
#include <wincon.h>
#include <Windows.h>
#include "stateManager.h"
#include "menu.h"
#include"parameter.h"
#include "lightgui.h"
#include"helpAndPause.h"
/*for pause page*/
void setPauseMenu();
void drawPauseMenu();

/*for help page*/
void setHelpMenu();
void drawHelpMenu();
void drawHelpMenuP1();
void drawHelpMenuP2();
void Goback();
void helpEvent(int key, int event);
void TurnPage();
static int page = 1;

extern State PauseMenu = {
	"PAUSEMENU",
	setPauseMenu,
	drawPauseMenu,
	NULL,
	NULL,
	uiMouseEvent,
};

extern State HelpMenu = {
	"HELPMENU",
	setHelpMenu,
	drawHelpMenu,
	NULL,
	helpEvent,
	uiMouseEvent,
};

void setPauseMenu()
{
	double x = GetWindowWidth();
	double y = GetWindowHeight();
	setButton(x / 2 - 0.75, y / 2 - 0.15, 0.1, 1.5, 0.4, "", "继续游戏", Goback);
	setButton(x / 2 - 0.75, y / 2 - 0.7, 0.1, 1.5, 0.4, "", "查看帮助", ToHelp);
	setButton(x / 2 - 0.75, y / 2 - 1.25, 0.1, 1.5, 0.4, "", "回到主菜单", GetBackToMainMenu);
}

void drawPauseMenu()
{
	string pause = "PAUSE";
	statemanager.stack[statemanager.top - 1]->draw();
	MovePen(4, 6);
	SetPenColor("white");
	StartFilledRegion(0);
	DrawLine(8, 0);
	DrawLine(0, -3);
	DrawLine(-8, 0);
	DrawLine(0, 3);
	EndFilledRegion();
	SetPenColor("black");
	MovePen(4, 6);
	DrawLine(8, 0);
	DrawLine(0, -3);
	DrawLine(-8, 0);
	DrawLine(0, 3);
	int regPointSize = GetPointSize();
	SetPointSize(regPointSize * 2.5);
	SetStyle(1);
	double width = TextStringWidth(pause);
	MovePen(GetWindowWidth() / 2 - width / 2, 5.5);
	DrawTextString(pause);
	SetPointSize(regPointSize);
	SetStyle(0);
	traverseButton();

}


void setHelpMenu()
{
	double x = GetWindowWidth();
	double y = GetWindowHeight();
	page = 1;
	setButton(x / 2 - 0.8, y / 4, 0.1, 0.6, 0.4, "", "返回", Goback);
	setButton(x / 2 + 0.2, y / 4, 0.1, 0.6, 0.4, "NextPage.bmp", "", TurnPage);

}

void drawHelpMenu()
{
	switch (page)
	{
	case 1:drawHelpMenuP1(); break;
	case 2:drawHelpMenuP2(); break;
	}
}
void drawHelpMenuP1()
{
	const string helpsP1[2][4] = { "W:跳跃","A/D:向左/右运动","鼠标左键：绘制图线/向当前方向射击","鼠标右键：切换绘图/射击模式","ESC：退出关卡回到主菜单","P：暂停游戏","H：帮助页面","←/→：（帮助页面）上一页/下一页" };
	int regPointSize = GetPointSize();
	SetPointSize(regPointSize * 2.5);
	SetStyle(1);
	double width = TextStringWidth("操作提示");
	MovePen(GetWindowWidth() / 2 - width / 2, 7);
	DrawTextString("操作提示");
	SetPointSize(regPointSize * 2);
	SetStyle(0);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			MovePen(1.5 + 7 * i, 6 - j * 1);
			DrawTextString(helpsP1[i][j]);
		}
	}
	SetPointSize(regPointSize);
	double x = GetWindowWidth() / 2 + 0.2;
	double y = GetWindowHeight() / 4;
	double w = 0.15, h = 0.3;
	traverseButton();
}

void Goback()
{
	StatePop(NULL);
}

void helpEvent(int key, int event)
{
	if (page == 1 && key == VK_RIGHT && event == KEY_DOWN)
	{
		page = 2;
		return;
	}
	if (page == 2 && key == VK_LEFT && event == KEY_DOWN)
	{
		page = 1;
		return;
	}
}


void drawHelpMenuP2()
{
	int regPointSize = GetPointSize();
	SetPointSize(regPointSize * 2.5);
	SetStyle(1);
	double width = TextStringWidth("图标解释");
	MovePen(GetWindowWidth() / 2 - width / 2, 7);
	DrawTextString("图标解释");
	SetPointSize(regPointSize * 1.5);
	SetStyle(0);
	NewSetPenColor(40, 85, 242);
	StartFilledRegion(1);
	MovePen(GetWindowWidth() / 2 - 1, 5.5);
	DrawLine(2, 0);
	DrawLine(0, -0.2);
	DrawLine(-2, 0);
	DrawLine(0, 0.2);
	EndFilledRegion();
	MovePen(GetWindowWidth() / 2 - 1.4 - TextStringWidth("INK:"), 5.4 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("INK:");
	MovePen(GetWindowWidth() / 2 + 1.2, 5.4 - GetFontAscent() / 2);
	DrawTextString("当前剩余墨水量");
	NewSetPenColor(242, 11, 25);
	StartFilledRegion(1);
	MovePen(GetWindowWidth() / 2 - 1, 6);
	DrawLine(2, 0);
	DrawLine(0, -0.2);
	DrawLine(-2, 0);
	DrawLine(0, 0.2);
	EndFilledRegion();
	MovePen(GetWindowWidth() / 2 - 1.4 - TextStringWidth("HP:"), 5.9 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("HP:");
	MovePen(GetWindowWidth() / 2 + 1.2, 5.9 - GetFontAscent() / 2);
	DrawTextString("当前剩余生命值");
	AddZoomBitMap("shoot.bmp", GetWindowWidth() / 2 - 0.4, 4, 0.8, 0.8, SRCCOPY);
	AddZoomBitMap("pen.bmp", GetWindowWidth() / 2 - 2.2, 4, 0.8, 0.8, SRCCOPY);
	AddZoomBitMap("ghost.bmp", GetWindowWidth() / 2 + 1.4, 4, 0.8, 0.8, SRCCOPY);
	SetPointSize(regPointSize);
	MovePen(GetWindowWidth() / 2 - TextStringWidth("射击模式") / 2, 3.2);
	DrawTextString("射击模式");
	MovePen(GetWindowWidth() / 2 - 1.8 - TextStringWidth("绘画模式") / 2, 3.2);
	DrawTextString("绘画模式");
	MovePen(GetWindowWidth() / 2 + 1.8 - TextStringWidth("敌人，可通过射击击破") / 2, 3.2);
	DrawTextString("敌人，可通过射击击破");
	traverseButton();
}

void TurnPage()
{
	double x = GetWindowWidth();
	double y = GetWindowHeight();
	if (page == 1)
	{
		CacheSorting();
		setButton(x / 2 - 0.8, y / 4, 0.1, 0.6, 0.4, "", "返回", Goback);
		setButton(x / 2 + 0.2, y / 4, 0.1, 0.6, 0.4, "PrevPage.bmp", "", TurnPage);
		page = 2;
	}
	else
	{
		CacheSorting();
		setButton(x / 2 - 0.8, y / 4, 0.1, 0.6, 0.4, "", "返回", Goback);
		setButton(x / 2 + 0.2, y / 4, 0.1, 0.6, 0.4, "NextPage.bmp", "", TurnPage);
		page = 1;
	}
	return;
}

