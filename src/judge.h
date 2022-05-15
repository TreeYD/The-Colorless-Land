#ifndef JUDGE_H
#define JUDGE_H
bool distance(double x1, double y1, double size1, double x2, double y2, double size2);//根据坐标和判定半径确定距离
bool RoleAndGroundX(struct BLOCK* blockhead);//角色与地面分水平和竖直判定
bool RoleAndGroundY(struct BLOCK* blockhead);
bool RoleAndEnemy(struct ENEMY enemy);//角色和敌人的碰撞判定
bool RoleAndGoal(struct GOAL goal);
bool EnemyAndBullet(struct ENEMY enemy, struct BULLET bullet);//敌人和子弹的碰撞判定
bool RoleAndBonus(struct BONUS bonus);//角色与收集物的判定
bool RoleAndLineX();//角色与所画桥的判定
bool RoleAndLineY();
bool MouseAndGround(struct BLOCK* blockhead);//鼠标位置和地面表面的判定，画图时用
bool MouseAndLine(struct LINE* line);//右键回收桥梁使用
#endif
