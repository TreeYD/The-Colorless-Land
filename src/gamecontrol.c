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
extern struct ROLE myrole;
struct ENEMY enemy[EnemyNum];
struct BULLET bullet[BulletNum];
extern void(*stateRender)(void);
struct BLOCK* blockhead;
LINE* LineUnion = NULL; //the linklist for all lines drawn.
void ScreenRender(void) {
	DisplayClear();
	if (stateRender != NULL) {
		stateRender();
	}
}
void render(int TimerID)//计时器回调函数
{
	startTimer(FALL, RENDERGAP);//FALL的Timer需要一直开着，因为需要一直判断，不需要按键来触发
	startTimer(JUDGE, JUDGEGAP);
	startTimer(BULLETMAKE, RENDERGAP);//子弹的不断产生
	startTimer(BULLETMOVE, RENDERGAP);//子弹运动的Timer需要一直开着
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
			startTimer(LEFTMOVING, RENDERGAP);
			break;
		case'D':
			startTimer(RIGHTMOVING, RENDERGAP);
			break;
		case 'W':
			startTimer(JUMP, RENDERGAP);
			break;
		case 'F'://切换武器，按键可以改，也可以改鼠标
			myrole.weapon = !myrole.weapon;
			break;
		}
	}
	else if (event == KEY_UP) {
		switch (key)
		{
		case'A':
			cancelTimer(LEFTMOVING);
			break;
		case'D':
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
		if (myrole.x >= 0 && !RoleAndGroundX(blockhead) && !RoleAndLineX()) {//窗口边界判定和地面障碍判定
			myrole.x -= RoleSpeed;
		}
		break;
	case RIGHTMOVING:
		myrole.direction = RIGHT;
		if (myrole.x <= GraphicsWindowWidth - RoleWidth && !RoleAndGroundX(blockhead) && !RoleAndLineX()) {//窗口边界判定和地面障碍判定
			myrole.x += RoleSpeed;
		}
		break;
	case JUMP:
		if (!IsJumping && !IsDropping && (RoleAndGroundY(blockhead) || RoleAndLineY())) {
			IsJumping = TRUE;
			VerticalSpeed = INITIALVERTICALSPEED;
		}
		if (IsJumping) {
			myrole.y += VerticalSpeed;
			VerticalSpeed -= G;
		}
		if (RoleAndGroundY(blockhead) || RoleAndLineY()) {
			IsJumping = FALSE;
			cancelTimer(JUMP);
		}
		break;
	case FALL://FALL的Timer需要一直开着，因为需要一直判断，不需要按键来触发
		if (!IsJumping && !IsDropping && !RoleAndGroundY(blockhead) && !RoleAndLineY()) {
			IsDropping = TRUE;
			FallingSpeed = 0;
		}
		if (IsDropping) {
			myrole.y -= FallingSpeed;
			FallingSpeed += G;
		}
		if (RoleAndGroundY(blockhead) || RoleAndLineY()) {
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
		if (RoleAndBonus(bonus[i])) {
			if (bonus[i].IsColor) {
				myrole.colorvolume++;
			}
			else {
				myrole.mark++;
			}
			bonus[i].live = FALSE;
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
	if (RoleAndGoal(NowGoal)) {
		CurrentRank++;
		StatePush(&GameState[CurrentRank]);
	}
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
				bullet[i].SpeedX = BulletSpeed * COS;
				bullet[i].SpeedY = BulletSpeed * SIN;
				bullet[i].IsMoving = TRUE;
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
	if (button == VK_LBUTTON) {
		if (event == BUTTON_DOWN) {
			if (myrole.colorvolume <= 0) {
				return;
			}
			if (myrole.weapon) {//用枪的情况
				startTimer(SHOT, RENDERGAP);
			}
			else {//用笔的情况
				if (MouseAndGround(blockhead)) {//鼠标接触地面才能画桥
					startTimer(DRAW, RENDERGAP);
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
	if (button = VK_RBUTTON) {//右键点击桥梁回收
		if (!myrole.weapon) {
			Delete();
		}
	}
}
void MakeLine() {
	if (!IsDrawing) {
		IsDrawing = TRUE;
		LINE* line = GetBlock(sizeof(LINE));
		DOT* LineHead = GetBlock(sizeof(DOT));
		line->HeadDot = LineHead;
		line->next = NULL;
		AddLine(line);
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
	if (IsDrawing) {
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
	while (line != NULL) {
		if (MouseAndLine(line)) {//判断鼠标点击桥
			DeleteLine(line);
			return;//点一次删一条
		}
		line = line->next;
	}
	return;
}
void DeleteLine(LINE* line) {
	LINE* p = LineUnion;
	if (p == NULL) {
		return;
	}
	while (p->next != line) {
		p = p->next;
	}
	p->next = line->next;
	DOT* q = line->HeadDot;//删除桥里面的dot
	DOT* r = NULL;
	while (q != NULL) {
		r = q;
		q = q->next;
		free(r);
		myrole.colorvolume += VOLUMEREDUCINGSPEED;//颜料回收
	}
	free(line);//删除桥本身
	return;
}
