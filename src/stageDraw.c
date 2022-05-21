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
#include"stageDraw.h"
#include"graphics.h"
#include"parameter.h"
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include "lightgui.h"
#include "menu.h"
extern LINE* LineUnion;
void DrawExistingLines(void);
void DrawEnemy(void);
char MarkString[20] ;
void DrawStatusBar()
{
	SetPointSize(13);
	NewSetPenColor(196, 196, 196);
	StartFilledRegion(1);
	MovePen(0, 0.56);
	DrawLine(16, 0);
	DrawLine(0, -0.56);
	DrawLine(-16, 0);
	DrawLine(0, 0.56);
	EndFilledRegion();
	AddZoomBitMap("pause.bmp", 7.832, 0.112, 0.336, 0.336, SRCAND);
	//MovePen(5.364, 0.36);
	NewSetPenColor(242, 11, 25);
	StartFilledRegion(1);
	drawBox(5.364, 0.18, 0, (myrole.HP > 0 ? 2.0 * myrole.HP / InitialHP : 0), 0.18);
	//DrawLine(2.0*myrole.HP / InitialHP, 0);
	//DrawLine(0, -0.18);
	//DrawLine(-2.0*myrole.HP / InitialHP, 0);
	//DrawLine(0, 0.18);
	EndFilledRegion();
	MovePen(4.864, 0.28 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("HP:");
	NewSetPenColor(40, 85, 242);
	StartFilledRegion(1);
	drawBox(9.004, 0.18, 0, (myrole.colorvolume > 0 ? 2.0 * myrole.colorvolume / MaxColorVolume : 0), 0.18);
	EndFilledRegion();
	MovePen(8.504, 0.28 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("INK:");
	MovePen(11.304, 0.28 - GetFontAscent() / 2);
	DrawTextString("MODE:");
	AddZoomBitMap((myrole.weapon ? "shoot.bmp" : "pen.bmp"), 11.904, 0.112, 0.336, 0.336, SRCAND);
	char MarkString[100];
	MovePen(2.56, 0.28 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	sprintf(MarkString, "Your Mark : % d", myrole.mark);
	DrawTextString(MarkString);
}
void DrawBlock(void)
{
	struct BLOCK* p;
	for (p = blockhead; p != NULL; p = p->next)
	{
		SetPenColor("black");
		StartFilledRegion(1);
		drawBox(p->x, p->y, 0, BlockSize * 2, BlockSize * 2);
		EndFilledRegion();
	}
}

void DrawBullet()
{
	int i;
	for (i = 0; i < BulletNum; i++)
	{
		if (bullet[i].live == TRUE && bullet[i].IsMoving == TRUE)
		{
			MovePen(bullet[i].x + 2 * BulletSize, bullet[i].y + BulletSize);
			SetPenColor("Black");
			StartFilledRegion(1);
			DrawArc(BulletSize, 0, 360);
			EndFilledRegion();
		}
	}
}

void DrawGoal()
{
	AddZoomBitMap("Goal.bmp", NowGoal.x, NowGoal.y, GoalSize * 2, GoalSize * 2, SRCCOPY);
}

void DrawRole()
{
	static int currPic = 1;
	static int counter = 20;
	string picName;
	if (myrole.HP > 0)
	{
		counter = 24;
	}
	if (myrole.HP <= 0)
	{
		if(counter>16)
		picName = "PICRETIRED2.bmp";
		else if(counter>8)
			picName = "PICRETIRED1.bmp";
		else
			picName = "PICRETIRED.bmp";
		AddZoomBitMap(picName, myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
		counter--;
		return;
	}
	if (myrole.IsMoving == FALSE)
	{
		if (!myrole.weapon) {
			if (myrole.direction == LEFT)
				AddZoomBitMap("PICLEFTSTILL.bmp", myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
			else
				AddZoomBitMap("PICRIGHTSTILL.bmp", myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
		}
		else
		{
			if (myrole.direction == LEFT)
				AddZoomBitMap("PICLEFTGUN.bmp", myrole.x - 0.12, myrole.y, RoleHeight, RoleHeight, SRCAND);
			else
				AddZoomBitMap("PICRIGHTGUN.bmp", myrole.x, myrole.y, RoleHeight, RoleHeight, SRCAND);
		}
	}
	else
	{
		currPic++;
		if (currPic == 7)
			currPic = 1;
		if (myrole.direction == LEFT)
		{
			switch (currPic)
			{
			case 1:case 2:picName = "PICLEFT1.bmp"; break;
			case 3:case 4:picName = "PICLEFT2.bmp"; break;
			case 5:case 6:picName = "PICLEFT3.bmp"; break;
			}
			AddZoomBitMap(picName, myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
		}

		else
		{
			switch (currPic)
			{
			case 1:case 2:picName = "PICRIGHT1.bmp"; break;
			case 3:case 4:picName = "PICRIGHT2.bmp"; break;
			case 5:case 6:picName = "PICRIGHT3.bmp"; break;
			}
			AddZoomBitMap(picName, myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
		}
	}
}

void DrawBonus()
{
	int i;
	for (i = 0; i < BonusNum; i++)
	{
		if (bonus[i].live == TRUE)
		{
			if (bonus[i].IsColor == TRUE)
				AddZoomBitMap("COLORBONUS.bmp", bonus[i].x, bonus[i].y, BonusSize * 2, BonusSize * 2, SRCAND);
			else
				AddZoomBitMap("NORMALBONUS.bmp", bonus[i].x, bonus[i].y, BonusSize * 2, BonusSize * 2, SRCAND);
		}
	}
}

void StageDraw()
{
	DrawBlock();
	DrawBonus();
	DrawGoal();
	DrawBullet();
	DrawRole();
	DrawEnemy();
	DrawExistingLines();
	traverseButton();
	DrawStatusBar();
}

void DrawExistingLines()
{
	LINE* p;
	DOT* pres;
	for (p = LineUnion; p != NULL; p = p->next)
	{
		for (pres = p->HeadDot; pres != NULL; pres = pres->next)
		{

			NewSetPenColor(0, 0, 0);
			StartFilledRegion(1);
			drawBox(pres->x, pres->y, 0, 2 * DotSize, 2 * DotSize);
			EndFilledRegion();
		}
	}
}

void DrawEnemy()
{
	int i;
	string enemyPicSource;
	for (i = 0; i < EnemyNum; i++)
	{
		if (enemy[i].live == TRUE)
		{
			switch (enemy[i].kind)
			{
			case 1:enemyPicSource = "ghost.bmp";
				break;
			case 2:enemyPicSource = "skull.bmp";
				break;
			case 3:enemyPicSource = "chicken.bmp";
				break;
			default:
				enemyPicSource = "chicken.bmp";
				break;
			}
			AddZoomBitMap(enemyPicSource, enemy[i].x, enemy[i].y, enemy[i].width, enemy[i].height, SRCAND);
		}
	}
}
