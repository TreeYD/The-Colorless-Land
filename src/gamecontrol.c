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
#include"stateManager.h"
#include"judge.h"
#include<math.h>
extern struct ROLE myrole;
extern void(*stateRender)(void);
struct ENEMY enemy[EnemyNum];
struct BULLET bullet[BulletNum];
struct BLOCK* blockhead;
static void ScreenRender(void);
void render(int TimerID)//��ʱ���ص�����
{
	startTimer(FALL, RENDERGAP);//FALL��Timer��Ҫһֱ���ţ���Ϊ��Ҫһֱ�жϣ�����Ҫ����������
	startTimer(BULLETMAKE, RENDERGAP);//�ӵ��Ĳ��ϲ���
	startTimer(BULLETMOVE, RENDERGAP);//�ӵ��˶���Timer��Ҫһֱ����
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
	case BULLETMAKE:
		BulletMake();
	case SHOT:
		Shot();
		break;
	case BULLETMOVE:
		BulletMove();
		break;
	case DRAW://����д
		break;
	case RENDER:
		ScreenRender();
		break;
	}
	return;
}
void KeyBoardControl(int key, int event) {//������Ϣ�ص�����
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
		if (myrole.x >= 0 && !RoleAndGroundX(myrole, blockhead)) {//���ڱ߽��ж��͵����ϰ��ж�
			myrole.x -= RoleSpeed;
		}
		break;
	case RIGHTMOVING:
		myrole.direction = RIGHT;
		if (myrole.x <= GraphicsWindowWidth - RoleWidth && !RoleAndGroundX(myrole, blockhead)) {//���ڱ߽��ж��͵����ϰ��ж�
			myrole.x += RoleSpeed;
		}
		break;
	case JUMP:
		if (!IsFloating) {
			VerticalSpeed = INITIALVERTICALSPEED;
			IsFloating = TRUE;
			myrole.y += VerticalSpeed;
			VerticalSpeed -= G;
		}
		else if (RoleAndGroundY(myrole, blockhead)) {
			VerticalSpeed = INITIALVERTICALSPEED;
			IsFloating = FALSE;
			cancelTimer(JUMP);
		}
		break;
	case FALL://FALL��Timer��Ҫһֱ���ţ���Ϊ��Ҫһֱ�жϣ�����Ҫ����������
		if (!RoleAndGroundY(myrole, blockhead) && !IsFloating) {
			//�ж�˵������һ��ֱ�������ж�û����block�Ӵ��ڶ�û������Ծ�У��Ϳ�ʼ���䣬�ڶ���������ӻ���ְ�W����ʱ���������ٶ�һ��һ��
			FallingSpeed = 0;
			myrole.y -= FallingSpeed;
			FallingSpeed += G;
		}
		if (RoleAndGroundY(myrole, blockhead)) {
			FallingSpeed = 0;
		}
		break;
	default:
		break;
	}
	return;
}
void BulletMake() {//�ӵ�����
	int i;
	for (i = 0; i < BulletNum; i++) {
		if (!bullet[i].live) {
			bullet[i].live = TRUE;
		}
	}
	return;
}
void Shot() {//��W����ʱ���õĺ���
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
void BulletMove() {//�ӵ������ȥ�Ժ��Զ��˶��ĺ���
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
			}
		}
	}
	return;
}
void MouseControl(int x, int y, int button, int event) {//�����Ϣ�ص�����
	MouseX = ScaleXInches(x);
	MouseY = ScaleYInches(y);
	COS = (MouseX - myrole.x) / sqrt(pow(MouseX - myrole.x, 2) + pow(MouseY - myrole.y, 2));
	SIN = (MouseY - myrole.y) / sqrt(pow(MouseX - myrole.x, 2) + pow(MouseY - myrole.y, 2));
	if (button == VK_LBUTTON) {
		if (event == BUTTON_DOWN) {
			if (myrole.weapon) {//��ǹ�����
				startTimer(SHOT, RENDERGAP);
			}
			else {//�ñʵ����
				startTimer(DRAW, RENDERGAP);
			}
		}
		if (event == BUTTON_UP) {
			if (myrole.weapon) {
				cancelTimer(SHOT);
			}
			else {
				cancelTimer(DRAW);
			}
		}
	}
	else if (button == VK_RBUTTON)
	{
		if(event==BUTTON_DOWN)
		myrole.weapon = !myrole.weapon;
	}
	
	return;
}

void ScreenRender(void)
{
	DisplayClear();
	if (stateRender != NULL)
	{
		stateRender();
	}
}