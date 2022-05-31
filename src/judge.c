#include"parameter.h"
#include"gamecontrol.h"
#include"judge.h"
#include<math.h>
extern bool teleport;
extern bool IsJumping;
extern bool IsDropping;
extern double VerticalSpeed;
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
bool JumpJudgeBlock() {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y;
	double BlockX, BlockY;
	struct BLOCK* p = blockhead;
	while (p != NULL) {
		BlockX = p->x + BlockSize;
		BlockY = p->y + 2 * BlockSize;
		if (fabs(RoleX - BlockX) <= JUMPBLOCKRANGEX && RoleY - BlockY <= 0 && BlockY - RoleY <= BlockSize) {
			if (teleport)
			{
				teleport = FALSE;
				myrole.y = p->y + 2 * BlockSize;
			}
			return TRUE;
		}
		else if (fabs(RoleX - BlockX) < BlockSize+RoleWidth/2 -0.05&& BlockY - RoleY < RoleHeight + 2 * BlockSize && BlockY - RoleY>0&&IsJumping)
		{
			if (teleport)
			{
			teleport = FALSE;
			myrole.y = p->y - RoleHeight;
			}
			VerticalSpeed = -1 * fabs(VerticalSpeed);
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}
bool RightMoveJudgeBlock() {
	double RoleX = myrole.x + RoleWidth;
	double RoleY = myrole.y;
	double BlockX, BlockY;
	struct BLOCK* p = blockhead;
	while (p != NULL) {
		BlockX = p->x;
		BlockY = p->y;
		if (BlockX - RoleX >= 0 && BlockX - RoleX <= MOVERANGE && (BlockY - RoleY < RoleHeight && BlockY - RoleY >= 0 || RoleY - BlockY < 2 * BlockSize && RoleY - BlockY >= 0)) {

			if (teleport)
			{
				teleport = FALSE;
				myrole.x = p->x - RoleWidth;
			}
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}
bool LeftMoveJudgeBlock() {
	double RoleX = myrole.x;
	double RoleY = myrole.y;
	double BlockX, BlockY;
	struct BLOCK* p = blockhead;
	while (p != NULL) {
		BlockX = p->x + 2 * BlockSize;
		BlockY = p->y;
		if (RoleX - BlockX >= 0 && RoleX - BlockX <= MOVERANGE && (BlockY - RoleY < RoleHeight && BlockY - RoleY > 0 || RoleY - BlockY < 2 * BlockSize && RoleY - BlockY >= 0)) {
			if (teleport)
			{
				teleport = FALSE;
				myrole.x = p->x + 2 * BlockSize;
			}
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}
bool RoleAndEnemy(struct ENEMY enemy) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	double EnemyX = enemy.x + enemy.width / 2;
	double EnemyY = enemy.y + enemy.height / 2;
	if (enemy.live == FALSE)
		return FALSE;
	if (distance(RoleX, RoleY, RoleWidth / 2, EnemyX, EnemyY, enemy.size)) {
		return TRUE;
	}
	return FALSE;
}
bool RoleAndGoal(struct GOAL goal) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	double GoalX = goal.x + GoalSize;
	double GoalY = goal.y + GoalSize;
	if (distance(RoleX, RoleY, RoleWidth , goal.x, goal.y, GoalSize)) {
		return TRUE;
	}
	return FALSE;
}
bool EnemyAndBullet(struct ENEMY enemy, struct BULLET bullet) {
	double BulletX = bullet.x + BulletWidth / 2;
	double BulletY = bullet.y + BulletHeight / 2;
	double EnemyX = enemy.x + enemy.width / 2;
	double EnemyY = enemy.y + enemy.height / 2;
	if (distance(BulletX, BulletY, BulletSize, EnemyX, EnemyY, enemy.size)&&enemy.live==TRUE) {
		return TRUE;
	}
	return FALSE;
}
bool BulletAndGround(struct BULLET bullet) {
	struct BLOCK* p = blockhead;
	double BulletX = bullet.x + BulletWidth / 2;
	double BulletY = bullet.y + BulletHeight / 2;
	while (p != NULL) {
		double BlockX = p->x + BlockSize;
		double BlockY = p->y + BlockSize;
		if (distance(BulletX, BulletY, BulletSize, BlockX, BlockY, BlockSize)) {
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}
bool RoleAndBonus(struct BONUS bonus) {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y + RoleHeight / 2;
	double BonusX = bonus.x + BonusSize;
	double BonusY = bonus.y + BonusSize;
	if (distance(RoleX, RoleY, RoleWidth / 2, BonusX, BonusY, BonusSize)) {
		return TRUE;
	}
	return FALSE;
}
bool JumpJudgeDot() {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y;
	double DotX, DotY;
	LINE* line = LineUnion;

	while (line != NULL) {
		DOT* dot = line->HeadDot;
		while (dot != NULL) {
			DotX = dot->x + DotSize;
			DotY = dot->y + 2 * DotSize;
			 if (fabs(RoleX - DotX) < RoleWidth / 2 + DotSize && (IsJumping&&VerticalSpeed >= 0) && RoleY + RoleHeight - 0.1 > DotY - 2 * DotSize&&RoleY < DotY)
			{
			if (teleport)
			{
				teleport = FALSE;
				myrole.y = DotY - RoleHeight - 2 * DotSize + 0.1;
			}
			VerticalSpeed = -1 * fabs(VerticalSpeed);
			return TRUE;
			}
			else if (fabs(RoleX-DotX)<RoleWidth/2+DotSize&&RoleY<=DotY&&RoleY+RoleHeight>DotY&&VerticalSpeed<=0) {//TO DO :�ֱ泣̬ʱվ����ײ��������ײ
				if (teleport)
				{
					teleport = FALSE;
					myrole.y = DotY;
				}
				return TRUE;
			}
			
			dot = dot->next;
		}
		line = line->next;
	}
	return FALSE;
}
bool RightMoveJudgeDot() {
	double RoleX = myrole.x + RoleWidth;
	double RoleY = myrole.y;
	double DotX, DotY;
	LINE* line = LineUnion;
	while (line != NULL) {
		DOT* dot = line->HeadDot;
		while (dot != NULL) {
			DotX = dot->x;
			DotY = dot->y;
			if (RoleX-DotX>=0&&RoleX-DotX<=2*DotSize && (RoleY-DotY<2*DotSize&&RoleY>=DotY||DotY-RoleY<RoleHeight&&DotY>RoleY)) {
				if (teleport)
				{
					teleport = FALSE;
					myrole.x = DotX - RoleWidth;
				}
				return TRUE;
			}
			dot = dot->next;
		}
		line = line->next;
	}
	return FALSE;
}
bool LeftMoveJudgeDot() {
	double RoleX = myrole.x;
	double RoleY = myrole.y;
	double DotX, DotY;
	LINE* line = LineUnion;
	while (line != NULL) {
		DOT* dot = line->HeadDot;
		while (dot != NULL) {
			DotX = dot->x + 2 * DotSize;
			DotY = dot->y;
			if (RoleX-DotX<=0  && DotX - RoleX < 2*DotSize && (RoleY - DotY < 2 * DotSize&&RoleY >= DotY || DotY - RoleY<RoleHeight&&DotY>RoleY)) {
				if (teleport)
				{
					teleport = FALSE;
					myrole.x = dot->x + 2 * DotSize;
				}
				return TRUE;
			}
			dot = dot->next;
		}
		line = line->next;
	}
	return FALSE;
}
bool MouseAndGround() {
	struct BLOCK* p = blockhead;
	while (p != NULL) {
		double BlockX = p->x + BlockSize;
		double BlockY = p->y + BlockSize;
		if (distance(MouseX, MouseY, 0, BlockX, BlockY, BlockSize)) {
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}
bool MouseAndLine(LINE* line) {
	LINE* p = LineUnion;
	DOT* dot;
	double DotX, DotY;
	if (p == NULL) {
		return;
	}
	while (p != line) {
		p = p->next;
	}
	dot = p->HeadDot;
	while (dot != NULL) {
		DotX = dot->x + DotSize;
		DotY = dot->y + DotSize;
		if (distance(MouseX, MouseY, 0, DotX, DotY, DotSize)) {
			return TRUE;
		}
		dot = dot->next;
	}
	return FALSE;
}
bool MouseAndAllLine() {
	LINE* p = LineUnion;
	DOT* dot;
	double DotX, DotY;
	if (p == NULL) {
		return;
	}
	while (p != NULL) {
		dot = p->HeadDot;
		while (dot != NULL) {
			DotX = dot->x + DotSize;
			DotY = dot->y + DotSize;
			if (distance(MouseX, MouseY, 0, DotX, DotY, DotSize)) {
				return TRUE;
			}
			dot = dot->next;
		}
		p = p->next;
	}
	return FALSE;
}

