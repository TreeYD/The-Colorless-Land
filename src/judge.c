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
bool JumpJudgeBlock() {
	double RoleX = myrole.x + RoleWidth / 2;
	double RoleY = myrole.y;
	double BlockX, BlockY;
	struct BLOCK* p = blockhead;
	while (p != NULL) {
		BlockX = p->x + BlockSize;
		BlockY = p->y + 2 * BlockSize;
		if (fabs(RoleX - BlockX) <= JUMPBLOCKRANGEX && fabs(RoleY - BlockY) <= JUMPBLOCKRANGEY) {
			myrole.y = BlockY;
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
		if (fabs(BlockX - RoleX) <= MOVERANGE && fabs(BlockY - RoleY) <= BlockSize) {
			myrole.x = BlockX - RoleWidth;
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
		if (fabs(BlockX - RoleX) <= MOVERANGE && fabs(BlockY - RoleY) <= BlockSize) {
			myrole.x = BlockX;
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
			if (fabs(RoleX - DotX) <= JUMPDOTRANGEX && fabs(RoleY - DotY) <= JUMPDOTRANGEY) {
				myrole.y = DotY;
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
			if (fabs(DotX - RoleX) <= MOVERANGE && fabs(DotY - RoleY) <= DotSize) {
				myrole.x = DotX - RoleWidth;
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
			if (fabs(DotX - RoleX) <= MOVERANGE && fabs(DotY - RoleY) <= DotSize) {
				myrole.x = DotX;
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

