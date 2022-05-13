#ifndef JUDGE_H
#define JUDGE_H
bool distance(double x1, double y1, double size1, double x2, double y2, double size2);//根据坐标和判定半径确定距离
bool RoleAndGroundX(struct ROLE myrole, struct BLOCK* blockhead);//角色与地面分水平和竖直判定
bool RoleAndGroundY(struct ROLE myrole, struct BLOCK* blockhead);
bool RoleAndEnemy(struct ROLE myrole, struct ENEMY enemy);//角色和敌人的碰撞判定
bool EnemyAndBullet(struct ENEMY enemy, struct BULLET bullet);//敌人和子弹的碰撞判定
#endif
