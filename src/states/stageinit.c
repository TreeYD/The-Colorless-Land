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
State* StageArray[StageNum];
fnPtr InitArray[StageNum];
void StageSetting() {
	int i;
	for (i = 0; i < StageNum; i++) {
		strcpy(StageArray[i].name, "STAGE");
		(StageArray[i].name)[5] = i + '1';
		(StageArray[i].name)[6] = '\0';
		StageArray[i]->init = InitArray[i];//待改
		StageArray[i]->draw = StageDraw;//在写
		StageArray[i]->destroy = StageClear;
		StageArray[i]->keyboardCallbackfn = KeyBoardControl;
		StageArray[i]->mouseCallbackfn = MouseControl;
	}
}
void RoleSetting(double x, double y) {
	myrole.x = x;
	myrole.y = y;
	myrole.colorvolume = InitialColorVolume;
	myrole.direction = RIGHT;
	myrole.HP = InitialHP;
	myrole.live = TRUE;
}
void EnemySetting(int i, double x, double y, double range, int direction, int kind) {
	enemy[i].x = x;
	enemy[i].y = y;
	enemy[i].moverange = range;
	enemy[i].nowrange = 0;
	enemy[i].direction = direction;
	enemy[i].live = TRUE;
	switch (enemy[i].kind)
	{
	case 1:
		enemy[i].HP = 1;
		enemy[i].width = 0.3;
		enemy[i].height = 0.3;
		enemy[i].size = 0.15;
		break;
	case 2:
		enemy[i].HP = 3;
		enemy[i].width = 0.5;
		enemy[i].height = 0.5;
		enemy[i].size = 0.25;
		break;
	case 3:
		enemy[i].HP = 5;
		enemy[i].width = 0.7;
		enemy[i].height = 0.7;
		enemy[i].size = 0.35;
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
		BonusSetting(i + j, startX + j * gap, StartY, IsColor);
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
void BlockSettingY(double StartX, double StartY, int number) {
	int i;
	for (i = 0; i < number; i++) {
		BlockSetting(StartX, StartY + 2 * (double)i * BlockSize);
	}
	return;
}
void GoalSetting(double x, double y) {
	NowGoal.x = x;
	NowGoal.y = y;
}
void StageClear() {
	BlockClear();
	EnemyClear();
	BonusClear();
	GoalClear();
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
	return;
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
void GoalClear() {
	NowGoal.live = FALSE;
}
void StageInit1() {
	BlockSettingX(0, 1, 3);
	BlockSettingX(3, 2, 4);
	BlockSettingX(12, 3, 4);
	BlockSettingX(13, 7, 3);
	BlockSettingX(0, 6, 7);
	RoleSetting(0, 1);
	EnemySetting(0, 12, 4, 3, RIGHT, 1);
	BonusSetting(0, 3, 3, TRUE);
	BonusSetting(1, 15, 4, TRUE);
	BonusSettingX(2, 4, 3, FALSE, 0.2, 5);
	BonusSettingX(7, 13, 8, FALSE, 0.2, 3);
	BonusSettingX(10, 3, 7, FALSE, 0.2, 5);
	GoalSetting(1, 7);
}
