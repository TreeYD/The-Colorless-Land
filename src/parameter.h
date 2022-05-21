#ifndef PARAMETER_H
#define PARAMETER_H

#define GraphicsWindowWidth 16//窗口大小
#define GraphicsWindowHeight 9
#define StageNum 20
#define RoleSpeed 0.2//角色移动速度
#define RoleWidth 0.5//角色大小
#define RoleHeight 0.75
#define InitialHP 100
#define InitialColorVolume 10
#define MaxColorVolume 100
#define EnemySpeed 0.02//敌人移动速度
#define EnemyNum 10//一个关卡中敌人的最大数量
#define BulletSpeed 0.3//子弹移动速度
#define BulletWidth 0.01//子弹大小
#define BulletHeight 0.01
#define BulletSize 0.005//子弹判定半径
#define BulletNum 100//子弹最大数量
#define BlockSize 0.25
#define BonusNum 30
#define BonusSize 0.15
#define GoalSize 0.15
#define DotSize 0.1
#include"genlib.h"
enum DIRECTION
{
	LEFT = -1,
	RIGHT = 1//枚举方向类型
};
struct ROLE
{
	double x;
	double y;
	int direction;//角色移动方向
	int mark;//得分
	double colorvolume;//颜料容量
	bool live;
	bool weapon;//TRUE为枪FALSE为画笔
	int HP;
	bool IsMoving;
};
struct ROLE myrole;
struct ENEMY
{
	double x;
	double y;
	double width;//不同敌人不同大小
	double height;
	double size;
	double moverange;//移动范围
	double nowrange;
	int direction;//敌人在一个区域往复运动，涉及运动方向
	int kind;//敌人种类
	bool live;
	int HP;//不同敌人血量不同，需要攻击不同次数，每次攻击HP--，感觉BOSS应该可以放进里面来
};
struct ENEMY enemy[EnemyNum];//敌人用数组存储
struct BULLET
{
	double x;
	double y;
	double SpeedX;//根据鼠标位置和角色位置确定夹角，然后将速度正交分解一下
	double SpeedY;
	bool live;
	bool IsMoving;
};
struct BULLET bullet[BulletNum];//子弹用数组存储
struct BLOCK {
	double x;
	double y;
	struct BLOCK* next;
};
struct BLOCK* blockhead;//地面表面用链表存储
struct BONUS {
	double x;
	double y;
	bool IsColor;//收集物两种 一种加分一种加颜料
	bool live;
};
struct BONUS bonus[BonusNum];
struct GOAL {
	double x;
	double y;
	bool live;
};
struct GOAL NowGoal;
struct STAGE {
	int num;
	struct STAGE* next;
};
typedef struct dot {
	double x, y;
	struct dot* next;
}DOT;
typedef struct line {
	DOT* HeadDot;
	struct line* next;
}LINE;
#endif//old
