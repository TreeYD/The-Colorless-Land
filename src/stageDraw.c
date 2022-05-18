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
extern LINE* LineUnion ;
void DrawExistingLines(void);
void DrawEnemy(void);
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
	MovePen(5.364, 0.36);
	NewSetPenColor(242, 11, 25);
	StartFilledRegion(1);
	DrawLine(2.0*myrole.HP / InitialHP, 0);
	DrawLine(0, -0.18);
	DrawLine(-2.0*myrole.HP / InitialHP, 0);
	DrawLine(0, 0.18);
	EndFilledRegion();
	MovePen(4.864, 0.28 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("HP:");
	NewSetPenColor(40, 85, 242);
	StartFilledRegion(1);
	drawBox(9.004, 0.18, 0, 2.0*myrole.colorvolume / InitialColorVolume, 0.18);
	EndFilledRegion();
	MovePen(8.504, 0.28 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("INK:");
	MovePen(11.304, 0.28 - GetFontAscent() / 2);
	DrawTextString("MODE:");
	AddZoomBitMap((myrole.weapon ? "shoot.bmp" : "pen.bmp"), 11.904, 0.112, 0.336, 0.336, SRCAND);
}
void DrawBlock(void)
{
	struct BLOCK*p;
	for (p = blockhead; p != NULL; p = p->next)
	{
		SetPenColor("black");
		StartFilledRegion(1);
		drawBox(p->x, p->y, 0, BlockSize*2, BlockSize*2);
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
	AddZoomBitMap("Goal.bmp", NowGoal.x, NowGoal.y, GoalSize, GoalSize, SRCCOPY);
}

void DrawRole()
{
	static bool currPic = FALSE;
	if (myrole.IsMoving == FALSE)
	{
		if (myrole.direction == LEFT)
			AddZoomBitMap("PICLEFTSTILL.bmp", myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
		else
			AddZoomBitMap("PICRIGHTSTILL.bmp", myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
	}
	else
	{
		currPic = !currPic;
		if (myrole.direction == LEFT)
			AddZoomBitMap((currPic ? "PICLEFT2.bmp" : "PICLEFT1.bmp"), myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
		else
			AddZoomBitMap((currPic ? "PICRIGHT2.bmp" : "PICRIGHT1.bmp"), myrole.x, myrole.y, RoleWidth, RoleHeight, SRCAND);
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
				AddZoomBitMap("COLORBONUS.bmp", bonus[i].x, bonus[i].y, BonusSize, BonusSize, SRCAND);
			else
				AddZoomBitMap("NORMALBONUS.bmp", bonus[i].x, bonus[i].y, BonusSize, BonusSize, SRCAND);
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
	LINE*p;
	DOT* pres;
	for (p = LineUnion; p != NULL; p = p->next)
	{
		for (pres = p->HeadDot; pres != NULL; pres = pres->next)
		{
			
			NewSetPenColor(0, 0, 0);
			StartFilledRegion(1);
			drawBox(pres->x, pres->y,0, 2*DotSize, 2*DotSize);
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
			AddZoomBitMap(enemyPicSource,enemy[i].x,enemy[i].y,enemy[i].width, enemy[i].height,SRCAND);
		}
	}
}