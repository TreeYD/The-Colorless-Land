#ifndef JUDGE_H
#define JUDGE_H
#define JUMPBLOCKRANGEX 0.27
#define JUMPBLOCKRANGEY 0.1
#define JUMPDOTRANGEX 0.12
#define JUMPDOTRANGEY 0.05
#define MOVERANGE 0.15
bool distance(double x1, double y1, double size1, double x2, double y2, double size2);//根据坐标和判定半径确定距离
bool JumpJudgeBlock();
bool RightMoveJudgeBlock();
bool LeftMoveJudgeBlock();
bool RoleAndEnemy(struct ENEMY enemy);//角色和敌人的碰撞判定
bool RoleAndGoal(struct GOAL goal);
bool EnemyAndBullet(struct ENEMY enemy, struct BULLET bullet);//敌人和子弹的碰撞判定
bool BulletAndGround(struct BULLET bullet);
bool RoleAndBonus(struct BONUS bonus);//角色与收集物的判定
bool JumpJudgeDot();
bool RightMoveJudgeDot();
bool LeftMoveJudgeDot();
bool MouseAndGround();//鼠标位置和地面表面的判定，画图时用
bool MouseAndLine(LINE*line);//右键回收桥梁使用
bool MouseAndAllLine();
#endif
