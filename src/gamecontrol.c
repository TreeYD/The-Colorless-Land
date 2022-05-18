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
#include "stateManager.h"
#include"helpAndPause.h"
extern struct ROLE myrole;
extern struct ENEMY enemy[EnemyNum];
extern struct BULLET bullet[BulletNum];
extern void(*stateRender)(void);
extern struct BLOCK* blockhead;
extern LINE* LineUnion = NULL; //the linklist for all lines drawn.
extern State PauseMenu;
extern State HelpMenu;
void ScreenRender(void) {
	DisplayClear();
	if (stateRender != NULL) {
		stateRender();
	}
}
void StartAutoTimer() {
	startTimer(FALL, RENDERGAP);//FALL的Timer需要一直开着，因为需要一直判断，不需要按键来触发
	startTimer(JUDGE, JUDGEGAP);
	startTimer(BULLETMAKE, RENDERGAP);//子弹的不断产生
	startTimer(BULLETMOVE, RENDERGAP);//子弹运动的Timer需要一直开着
	return;
}
void CancelControlTimer() {
	cancelTimer(FALL);
	cancelTimer(JUDGE);
	cancelTimer(BULLETMAKE);
	cancelTimer(BULLETMOVE);
	cancelTimer(LEFTMOVING);
	cancelTimer(RIGHTMOVING);
	cancelTimer(JUMP);
	cancelTimer(SHOT);
	cancelTimer(DRAW);
	return;
}
void render(int TimerID)//计时器回调函数
{
	switch (TimerID)
	{
	case LEFTMOVING:
		PlayerMove(LEFTMOVING);
		break;
	case RIGHTMOVING:
		PlayerMove(RIGHTMOVING);
		break;
	case JUMP:
		PlayerMove(JUMP);
		break;
	case FALL:
		PlayerMove(FALL);
		break;
	case JUDGE:
		BonusJudge();
		EnemyJudge();
		GoalJudge();
		break;
	case BULLETMAKE:
		BulletMake();
		break;
	case SHOT:
		Shot();
		break;
	case BULLETMOVE:
		BulletMove();
		break;
	case DRAW://还在写
		MakeLine();
		PickUpDots();
		break;
	case RENDER:
		ScreenRender();
		break;
	}
	return;
}
void KeyBoardControl(int key, int event) {//键盘信息回调函数
	if (event == KEY_DOWN)
	{
		switch (key)
		{
		case'A':
			myrole.IsMoving = TRUE;
			startTimer(LEFTMOVING, RENDERGAP);
			break;
		case'D':
			myrole.IsMoving = TRUE;
			startTimer(RIGHTMOVING, RENDERGAP);
			break;
		case 'W':
			startTimer(JUMP, RENDERGAP);
			break;
		case 'F'://切换武器，按键可以改，也可以改鼠标
			myrole.weapon = !myrole.weapon;
			break;
		case 'P':
			StatePush(&PauseMenu);
			break;
		case VK_ESCAPE:
			CancelControlTimer();
			StatePop("MAINMENU");
			break;
		case 'H':
			StatePush(&HelpMenu);
			break;
		default:
			break;
		}
	}
	else if (event == KEY_UP) {
		switch (key)
		{
		case'A':
			myrole.IsMoving = FALSE;
			cancelTimer(LEFTMOVING);
			break;
		case'D':
			myrole.IsMoving = FALSE;
			cancelTimer(RIGHTMOVING);
			break;
		default:
			break;
		}
	}
	return;
}
void PlayerMove(int event)
{
	switch (event)
	{
	case LEFTMOVING:
		myrole.direction = LEFT;
		if (myrole.x >= 0 && !LeftMoveJudgeBlock() && !LeftMoveJudgeDot()) {//窗口边界判定和地面障碍判定
			myrole.x -= RoleSpeed;
		}
		break;
	case RIGHTMOVING:
		myrole.direction = RIGHT;
		if (myrole.x <= GraphicsWindowWidth - RoleWidth && !RightMoveJudgeBlock() && !RightMoveJudgeDot()) {
			//窗口边界判定和地面障碍判定
			myrole.x += RoleSpeed;
		}
		break;
	case JUMP:
		if (!IsJumping && !IsDropping && (JumpJudgeBlock() || JumpJudgeDot() || myrole.y <= 1)) {
			IsJumping = TRUE;
			VerticalSpeed = INITIALVERTICALSPEED;
			myrole.y += VerticalSpeed;
		}
		if (IsJumping && !(JumpJudgeBlock() || JumpJudgeDot())) {
			VerticalSpeed -= G;
			myrole.y += VerticalSpeed;
		}
		if (JumpJudgeBlock() || JumpJudgeDot() || myrole.y <= 1) {
			IsJumping = FALSE;
			cancelTimer(JUMP);
		}
		break;
	case FALL://FALL的Timer需要一直开着，因为需要一直判断，不需要按键来触发
		if (myrole.y <= 1) {//保证角色不穿过下边界
			myrole.y = 1;
			IsDropping = FALSE;
		}
		if (!IsJumping && !IsDropping && !(JumpJudgeBlock() || JumpJudgeDot()) && myrole.y > 1) {
			IsDropping = TRUE;
			FallingSpeed = 0;
		}
		if (IsDropping) {
			myrole.y -= FallingSpeed;
			FallingSpeed += G;
		}
		if (JumpJudgeBlock() || JumpJudgeDot()) {
			IsDropping = FALSE;
		}
		break;
	default:
		break;
	}
	return;
}
void BonusJudge() {
	int i;
	for (i = 0; i < BonusNum; i++) {
		if (bonus[i].live) {
			if (RoleAndBonus(bonus[i])) {
				if (bonus[i].IsColor) {
					myrole.colorvolume += VOLUMEREDUCINGSPEED;
				}
				else {
					myrole.mark++;
				}
				bonus[i].live = FALSE;
			}
		}
	}
	return;
}
void EnemyJudge() {
	int i;
	for (i = 0; i < EnemyNum; i++) {
		if (RoleAndEnemy(enemy[i])) {
			myrole.HP--;
		}
		enemy[i].x = enemy[i].x + enemy[i].direction * EnemySpeed;//往复运动的判断
		enemy[i].nowrange += EnemySpeed;
		if (enemy[i].nowrange >= enemy[i].moverange) {
			enemy[i].direction = -enemy[i].direction;
			enemy[i].nowrange = 0;
		}
	}
	if (myrole.HP <= 0) {
		myrole.live = FALSE;
	}
	return;
}
void GoalJudge() {
	/*if (RoleAndGoal(NowGoal)) {
		CurrentRank++;
		StatePush(&GameState[CurrentRank]);
	}*/
}
void BulletMake() {//子弹产生
	int i;
	for (i = 0; i < BulletNum; i++) {
		if (!bullet[i].live) {
			bullet[i].live = TRUE;
		}
	}
	return;
}
void Shot() {//发射时调用的函数
	int i;
	if (myrole.colorvolume > 0) {
		for (i = 0; i < BulletNum; i++) {
			if (bullet[i].live && !bullet[i].IsMoving) {
				bullet[i].x = myrole.x + RoleWidth / 2;
				bullet[i].y = myrole.y + RoleHeight / 2;
				bullet[i].SpeedX = BulletSpeed * COS;
				bullet[i].SpeedY = BulletSpeed * SIN;
				bullet[i].IsMoving = TRUE;
				myrole.colorvolume--;
				return;
			}
		}
	}
	return;
}
void BulletMove() {//子弹发射出去以后自动运动的函数
	int i, j;
	for (i = 0; i < BulletNum; i++) {
		if (bullet[i].live && bullet[i].IsMoving) {
			bullet[i].x += bullet[i].SpeedX;
			bullet[i].y += bullet[i].SpeedY;
		}
	}
	for (i = 0; i < BulletNum; i++) {
		if (bullet[i].live && bullet[i].IsMoving) {
			if (BulletAndGround(bullet[i], blockhead) || bullet[i].x >= GraphicsWindowWidth || bullet[i].x <= 0 || bullet[i].y >= GraphicsWindowHeight || bullet[i].y <= 0) {
				bullet[i].live = FALSE;
				bullet[i].IsMoving = FALSE;
				//子弹撞墙 超出边界均算FALSE
			}
			for (j = 0; j < EnemyNum; j++) {
				if (EnemyAndBullet(enemy[j], bullet[i])) {
					enemy[j].HP--;
					bullet[i].live = FALSE;
					bullet[i].IsMoving = FALSE;
				}
				if (enemy[j].HP <= 0) {
					enemy[j].live = FALSE;
					myrole.mark++;//击杀敌人得分
				}
			}
		}
	}
	return;
}
void MouseControl(int x, int y, int button, int event) {//鼠标信息回调函数
	MouseX = ScaleXInches(x);
	MouseY = ScaleYInches(y);
	COS = (MouseX - myrole.x) / sqrt(pow(MouseX - myrole.x, 2) + pow(MouseY - myrole.y, 2));
	SIN = (MouseY - myrole.y) / sqrt(pow(MouseX - myrole.x, 2) + pow(MouseY - myrole.y, 2));
	if (button == LEFT_BUTTON) {
		if (event == BUTTON_DOWN) {
			if (myrole.colorvolume <= 0) {
				return;
			}
			if (myrole.weapon) {//用枪的情况
				startTimer(SHOT, RENDERGAP);
			}
			else {//用笔的情况
				if (MouseAndGround(blockhead) || MouseAndAllLine()) {//鼠标接触地面才能画桥
					startTimer(DRAW, DRAWGAP);
				}
			}
		}
		if (event == BUTTON_UP) {
			if (myrole.weapon) {
				cancelTimer(SHOT);
			}
			else {
				IsDrawing = FALSE;
				cancelTimer(DRAW);
			}
		}
	}
	if (button == RIGHT_BUTTON) {//右键点击桥梁回收
		if (event == BUTTON_DOWN) {
			if (!myrole.weapon) {
				Delete();
			}
		}
	}
}
void MakeLine() {
	if (!IsDrawing && myrole.colorvolume >= 0) {
		IsDrawing = TRUE;
		LINE* line = GetBlock(sizeof(LINE));
		DOT* LineHead = GetBlock(sizeof(DOT));
		line->HeadDot = LineHead;
		line->next = NULL;
		AddLine(line);
		myrole.colorvolume -= VOLUMEREDUCINGSPEED / 10;
		LineHead->x = MouseX;
		LineHead->y = MouseY;
		LineHead->next = NULL;
	}
	else {
		return;
	}
}
void AddLine(LINE* NewLine)
{
	LINE* p = LineUnion;
	if (p == NULL)
	{
		LineUnion = NewLine;
		return;
	}
	while (p->next != NULL)
		p = p->next;
	p->next = NewLine;
	return;
}
void PickUpDots(void)
{
	if (IsDrawing && myrole.colorvolume >= 0) {
		DOT* p;
		p = GetBlock(sizeof(DOT));
		p->x = MouseX;
		p->y = MouseY;
		p->next = NULL;
		myrole.colorvolume -= VOLUMEREDUCINGSPEED;
		AddDot(p);
	}
}
void AddDot(DOT* NewDot)
{
	LINE* p = LineUnion;
	DOT* dot = NULL;
	while (p->next != NULL)
		p = p->next;
	dot = p->HeadDot;
	if (dot == NULL)
	{
		p->HeadDot = NewDot;
		return;
	}
	while (dot->next != NULL)
		dot = dot->next;
	dot->next = NewDot;
	return;
}
void Delete() {
	LINE* line = LineUnion;
	LINE* prev = line;
	while (line != NULL) {
		if (MouseAndLine(line)) {
			if (line == LineUnion) {
				LineUnion = line->next;
			}
			else {
				prev->next = line->next;
			}
			CacheLineSorting(line);
			return;
		}
		prev = line;
		line = line->next;
	}
	return;
}
void CacheLineSorting(LINE* line) {
	LINE* temp = NULL;
	if (temp == NULL) {
		temp = line;
		recycleInk(temp);
	}
	else {
		DeleteLine(temp);
		temp = line;
		recycleInk(temp);
	}
}
void recycleInk(LINE* line) {
	DOT* q = NULL;
	for (q = line->HeadDot; q != NULL; q = q->next) {
		myrole.colorvolume += VOLUMEREDUCINGSPEED;
	}
}
void DeleteLine(LINE* line) {
	if (line == NULL) {
		return;
	}
	LINE* p = LineUnion;
	while (p != line) {
		p = p->next;
	}
	DOT* q = line->HeadDot;//删除桥里面的dot
	DOT* r = NULL;
	while (q != NULL) {
		r = q;
		q = q->next;
		free(r);
		r = NULL;
		myrole.colorvolume += VOLUMEREDUCINGSPEED;//颜料回收
	}
	free(line);//删除桥本身
	line = NULL;
	return;
}
