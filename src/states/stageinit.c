#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "conio.h"
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include"parameter.h"
#include"gamecontrol.h"
#include"judge.h"
#include<math.h>
#include"stageinit.h"
#include "stateManager.h"
#include"lightgui.h"
#include"stageDraw.h"
#include"menu.h"

extern LINE* LineUnion;
State* StageArray[StageNum];
fnPtr InitArray[StageNum];
void SettingMisc(void);
void StageSetting() {
	int i;
	for (i = 0; i < StageNum; i++) {
		StageArray[i] = GetBlock(sizeof(State));
		StageArray[i]->name = GetBlock(sizeof(char));
		sprintf(StageArray[i]->name, "%d", i);
		StageArray[i]->init = InitArray[i];
		StageArray[i]->draw = StageDraw;
		StageArray[i]->destroy = StageClear;
		StageArray[i]->keyboardCallbackfn = KeyBoardControl;
		StageArray[i]->mouseCallbackfn = MouseControl;
	}
}
void RoleSetting(double x, double y) {
	myrole.x = x;
	myrole.y = y;
	myrole.colorvolume = InitialColorVolume;
	myrole.IsMoving = FALSE;
	myrole.direction = RIGHT;
	myrole.HP = InitialHP;
	myrole.live = TRUE;
	myrole.mark = 0;
}
void EnemySetting(int i, double x, double y, double range, int direction, int kind) {
	enemy[i].x = x;
	enemy[i].y = y;
	enemy[i].moverange = range;
	enemy[i].nowrange = 0;
	enemy[i].direction = direction;
	enemy[i].live = TRUE;
	enemy[i].kind = kind;
	switch (enemy[i].kind)
	{
	case 1:
		enemy[i].HP = 1;
		enemy[i].width = 0.5;
		enemy[i].height = 0.5;
		enemy[i].size = 0.25;
		break;
	case 2:
		enemy[i].HP = 3;
		enemy[i].width = 0.7;
		enemy[i].height = 0.7;
		enemy[i].size = 0.35;
		break;
	case 3:
		enemy[i].HP = 5;
		enemy[i].width = 1;
		enemy[i].height = 1;
		enemy[i].size = 0.5;
		break;
	default:
		break;
	}
}
void BonusSetting(int i, double x, double y, bool IsColor) {
	bonus[i].x = x;
	bonus[i].y = y;
	bonus[i].live = TRUE;
	bonus[i].IsColor = IsColor;
}
void BonusSettingX(int i, double StartX, double StartY, bool IsColor, double gap, int number) {
	int j;
	for (j = 0; j < number; j++) {
		BonusSetting(i + j, StartX + j * gap, StartY, IsColor);
	}
	return;
}
void BlockSetting(double X, double Y) {
	if (blockhead == NULL) {
		blockhead = GetBlock(sizeof(struct BLOCK));
		blockhead->x = X;
		blockhead->y = Y;
		blockhead->next = NULL;
		return;
	}
	else {
		struct BLOCK* p = blockhead;
		struct BLOCK* q = NULL;
		while (p != NULL) {
			q = p;
			p = p->next;
		}
		p = GetBlock(sizeof(struct BLOCK));
		p->x = X;
		p->y = Y;
		p->next = NULL;
		q->next = p;
		return;
	}
}
void BlockSettingX(double StartX, double StartY, int number) {
	int i;
	for (i = 0; i < number; i++) {
		BlockSetting(StartX + 2 * (double)i * BlockSize, StartY);
	}
	return;
}
void GoalSetting(double x, double y) {
	NowGoal.x = x;
	NowGoal.y = y;
}
void StageClear() {
	BlockClear();
	LineClear();
	EnemyClear();
	BonusClear();
	BulletClear();
	GoalClear();
	CancelControlTimer();
	cancelTimer(HP);
	return;
}
void BlockClear() {
	struct BLOCK* p = blockhead;
	struct BLOCK* q = NULL;
	if (p == NULL) {
		return;
	}
	while (p != NULL) {
		q = p;
		p = p->next;
		free(q);
	}
	blockhead = NULL;
	return;
}
void LineClear() {
	LINE* line = LineUnion;
	DOT* dot, * p;
	while (line != NULL) {
		dot = line->HeadDot;
		while (dot != NULL) {
			p = dot;
			dot = dot->next;
			free(p);
		}
		line = line->next;
	}
	LineUnion = NULL;
}
void EnemyClear() {
	int i;
	for (i = 0; i < EnemyNum; i++) {
		enemy[i].live = FALSE;
	}
	return;
}
void BonusClear() {
	int i;
	for (i = 0; i < BonusNum; i++) {
		bonus[i].live = FALSE;
	}
	return;
}
void BulletClear() {
	int i;
	for (i = 0; i < BulletNum; i++) {
		bullet[i].IsMoving = FALSE;
	}
}
void GoalClear() {
	NowGoal.live = FALSE;
}
void setPauseButton()
{
	setButton(7.832, 0.112, 0.168, 0.336, 0.336, "", "", ToPause);
}

void FreeAllStages()
{
	int i;
	for (i = 0; i < StageNum; i++)
	{
		free(StageArray[i]);
	}
}
void InitSetting() {
	InitArray[0] = StageInit1;
	InitArray[1] = StageInit2;
	InitArray[2] = StageInit3;
	InitArray[3] = StageInit4;
	InitArray[4] = StageInit5;
	InitArray[5] = StageInit6;
}
void StageInit1() {
	RoleSetting(0, 1.06);
	BlockSettingX(0, 0.56, 12);
	BlockSettingX(4, 1.06, 4);
	BlockSettingX(4, 1.56, 4);
	BlockSettingX(7, 2.56, 2);
	BlockSettingX(8, 4.56, 4);
	BlockSettingX(12, 2.56, 8);
	BlockSettingX(0, 5.56, 12);
	BlockSettingX(12, 6.56, 8);
	EnemySetting(0, 8, 5.06, 1.5, RIGHT, 1);
	EnemySetting(1, 12, 3.06, 3.5, RIGHT, 1);
	BonusSettingX(0, 2.5, 1.06, FALSE, 0.4, 3);
	BonusSettingX(3, 2, 6.06, FALSE, 0.4, 5);
	BonusSettingX(8, 12, 7.06, FALSE, 0.4, 5);
	BonusSettingX(13, 4.9, 2.06, TRUE, 0.4, 2);
	BonusSettingX(15, 15, 3.06, TRUE, 0.4, 2);
	GoalSetting(1, 6.06);
	SettingMisc();
}
void StageInit2() {
	RoleSetting(15.5, 1.56);
	BlockSettingX(0, 6.56, 8);
	BlockSettingX(11, 0.56, 10);
	BlockSettingX(11, 1.06, 10);
	BlockSettingX(12, 5.56, 8);
	BlockSettingX(5, 4.56, 2);
	BlockSettingX(7, 3.56, 2);
	BlockSettingX(7, 5.56, 2);
	BlockSettingX(11, 7.06, 2);
	EnemySetting(0, 5, 5.06, 0.5, RIGHT, 1);
	EnemySetting(1, 12, 6.06, 3.5, RIGHT, 2);
	BonusSettingX(0, 11.2, 1.56, FALSE, 0.4, 5);
	BonusSettingX(5, 11.5, 2.26, FALSE, 0.4, 3);
	BonusSettingX(8, 1, 3.56, FALSE, 0.4, 5);
	BonusSettingX(13, 12.5, 6.06, FALSE, 0.4, 3);
	BonusSetting(16, 12, 4.06, TRUE);
	BonusSetting(17, 7, 6.06, TRUE);
	BonusSetting(18, 11, 7.56, FALSE);
	GoalSetting(0.5, 7.06);
	SettingMisc();
}
void StageInit3() {
	RoleSetting(0, 4.06);
	BlockSettingX(0, 3.56, 12);
	BlockSettingX(0, 6.06, 10);
	BlockSettingX(10, 3.56, 12);
	EnemySetting(0, 5.5, 4.06, 2.5, LEFT, 1);
	EnemySetting(1, 15.5, 4.06, 5.5, LEFT, 2);
	BonusSettingX(0, 7, 1.86, FALSE, 0.4, 5);
	BonusSettingX(5, 12.2, 4.06, FALSE, 0.4, 3);
	BonusSettingX(8, 2.5, 6.56, FALSE, 0.4, 3);
	BonusSettingX(11, 0.8, 4.06, FALSE, 0.4, 5);
	BonusSetting(16, 4, 4.06, TRUE);
	BonusSetting(17, 11.5, 4.06, TRUE);
	GoalSetting(1, 6.56);
	SettingMisc();
}
void StageInit4() {
	RoleSetting(1, 1.06);
	BlockSettingX(0, 0.56, 32);
	BlockSettingX(0, 3.56, 6);
	BlockSettingX(13, 4.56, 6);
	EnemySetting(0, 6, 1.06, 2.5, RIGHT, 2);
	EnemySetting(1, 15.5, 1.06, 3.5, LEFT, 2);
	EnemySetting(2, 0, 4.06, 2.5, RIGHT, 3);
	EnemySetting(3, 13, 5.06, 2.5, RIGHT, 3);
	BonusSettingX(0, 13.2, 5.06, FALSE, 0.4, 4);
	BonusSettingX(4, 6, 1.26, FALSE, 0.4, 5);
	BonusSettingX(9, 0.2, 4.06, TRUE, 0.4, 4);
	BonusSettingX(13, 11.2, 1.26, TRUE, 0.4, 4);
	GoalSetting(7.5, 7.56);
	SettingMisc();
}
void StageInit5() {
	RoleSetting(1, 1.06);
	BlockSettingX(0, 0.56, 32);
	EnemySetting(0, 5, 1.06, 3.5, RIGHT, 3);
	EnemySetting(1, 10, 1.06, 4.5, RIGHT, 3);
	BonusSettingX(0, 6.2, 1.06, FALSE, 0.4, 5);
	BonusSettingX(5, 4, 5.06, FALSE, 0.4, 5);
	BonusSettingX(10, 2, 1.06, TRUE, 0.4, 3);
	BonusSettingX(13, 12, 5.06, TRUE, 0.4, 3);
	BonusSettingX(16, 12, 1.06, TRUE, 0.4, 3);
	GoalSetting(7.5, 7.56);
	SettingMisc();
}
void StageInit6() {
	RoleSetting(0.5, 7.06);
	BlockSettingX(0, 6.56, 8);
	BlockSettingX(12, 6.56, 8);
	BlockSettingX(0, 0.56, 32);
	EnemySetting(0, 15.5, 7.06, 3.5, LEFT, 3);
	EnemySetting(1, 15.5, 1.06, 15.5, LEFT, 3);
	BonusSettingX(0, 2, 7.16, TRUE, 0.4, 2);
	BonusSettingX(2, 12, 7.16, TRUE, 0.4, 3);
	BonusSettingX(5, 6.8, 3.56, FALSE, 0.6, 5);
	BonusSettingX(10, 7.4, 4.26, FALSE, 0.4, 4);
	BonusSettingX(14, 7.6, 4.96, FALSE, 0.4, 3);
	BonusSettingX(17, 7.8, 5.66, FALSE, 0.4, 2);
	BonusSetting(19, 8.0, 6.36, FALSE);
	GoalSetting(7.5, 1.06);
	SettingMisc();
}

void SettingMisc()
{
	StartAutoTimer();
	setPauseButton();
	startTimer(HP, RENDERGAP);
}
