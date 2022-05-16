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
extern LINE* LineUnion;
struct ENEMY enemy[EnemyNum];
struct BULLET bullet[BulletNum];
struct BLOCK* blockhead;
bool distance(double x1, double y1, double size1, double x2, double y2, double size2) {
	double gap = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	if (gap <= size1 + size2) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
bool RoleAndGroundX(struct BLOCK* blockhead) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	while (blockhead) {
		double BlockX = blockhead->x + BlockSize / 2;
		double BlockY = blockhead->y + BlockSize / 2;
		if (distance(RoleX, RoleY, RoleWidth / 2, BlockX, BlockY, BlockSize)) {
			return TRUE;
		}
		blockhead = blockhead->next;
	}
	return FALSE;
}
bool RoleAndGroundY(struct BLOCK* blockhead) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	while (blockhead) {
		double BlockX = blockhead->x + BlockSize / 2;
		double BlockY = blockhead->y + BlockSize / 2;
		if (distance(RoleX, RoleY, RoleHeight / 2, BlockX, BlockY, BlockSize)) {
			return TRUE;
		}
		blockhead = blockhead->next;
	}
	return FALSE;
}
bool RoleAndEnemy(struct ENEMY enemy) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	double EnemyX = enemy.x + enemy.width / 2;
	double EnemyY = enemy.y + enemy.height / 2;
	if (distance(RoleX, RoleY, RoleWidth, EnemyX, EnemyY, enemy.size)) {
		return TRUE;
	}
	return FALSE;
}
bool RoleAndGoal(struct GOAL goal) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	double GoalX = goal.x + GoalSize / 2;
	double GoalY = goal.y + GoalSize / 2;
	if (distance(RoleX, RoleY, RoleWidth, goal.x, goal.y, GoalSize)) {
		return TRUE;
	}
	return FALSE;
}
bool EnemyAndBullet(struct ENEMY enemy, struct BULLET bullet) {
	double BulletX = bullet.x + BulletWidth / 2;
	double BulletY = bullet.y + BulletHeight / 2;
	double EnemyX = enemy.x + enemy.width / 2;
	double EnemyY = enemy.y + enemy.height / 2;
	if (distance(BulletX, BulletY, BulletSize, EnemyX, EnemyY, enemy.size)) {
		return TRUE;
	}
	return FALSE;
}
bool BulletAndGround(struct BULLET bullet, struct BLOCK* blockhead) {
	double BulletX = bullet.x + BulletWidth / 2;
	double BulletY = bullet.y + BulletHeight / 2;
	while (blockhead) {
		double BlockX = blockhead->x + BlockSize / 2;
		double BlockY = blockhead->y + BlockSize / 2;
		if (distance(BulletX, BulletY, BulletSize, BlockX, BlockY, BlockSize)) {
			return TRUE;
		}
		blockhead = blockhead->next;
	}
	return FALSE;
}
bool RoleAndBonus(struct BONUS bonus) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	double BonusX = bonus.x + BonusSize / 2;
	double BonusY = bonus.y + BonusSize / 2;
	if (distance(RoleX, RoleY, RoleWidth, BonusX, BonusY, BonusSize)) {
		return TRUE;
	}
	return FALSE;
}
bool RoleAndLineX() {
	LINE* line = LineUnion;
	DOT* dot = NULL;
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	while (line != NULL) {
		dot = line->HeadDot;
		while (dot != NULL) {
			if (distance(dot->x, dot->y, DotSize, RoleX, RoleY, RoleWidth)) {
				return TRUE;
			}
			dot = dot->next;
		}
		line = line->next;
	}
	return FALSE;
}
bool RoleAndLineY() {
	LINE* line = LineUnion;
	DOT* dot = NULL;
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	while (line != NULL) {
		dot = line->HeadDot;
		while (dot != NULL) {
			if (distance(dot->x, dot->y, DotSize, RoleX, RoleY, RoleHeight)) {
				return TRUE;
			}
			dot = dot->next;
		}
		line = line->next;
	}
	return FALSE;
}
bool MouseAndGround(struct BLOCK* blockhead) {
	while (blockhead) {
		double BlockX = blockhead->x + BlockSize / 2;
		double BlockY = blockhead->y + BlockSize / 2;
		if (distance(MouseX, MouseY, 0, BlockX, BlockY, BlockSize)) {
			return TRUE;
		}
		blockhead = blockhead->next;
	}
	return FALSE;
}
bool MouseAndLine(LINE* line) {
	DOT* p = line->HeadDot;
	while (p != NULL) {
		if (distance(MouseX, MouseY, 0, p->x, p->y, DotSize)) {
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}
