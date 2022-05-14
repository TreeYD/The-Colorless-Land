#ifndef PARAMETER_H
#define PARAMETER_H

#define GraphicsWindowWidth 16//窗口大小
#define GraphicsWindowHeight 9
#define RoleSpeed 0.5//角色移动速度
#define RoleWidth 0.5//角色大小
#define RoleHeight 1
#define EnemySpeed 0.5//敌人移动速度
#define EnemyNum 10//一个关卡中敌人的最大数量
#define BulletSpeed 3//子弹移动速度
#define BulletWidth 0.01//子弹大小
#define BulletHeight 0.002
#define BulletSize 0.005//子弹判定半径
#define BulletNum 100//子弹最大数量
#define BlockSize 0.5
#define DotSize 0.5
enum DIRECTION
{
	LEFT, RIGHT//枚举方向类型
};
struct ROLE
{
	double x;
	double y;
	int direction;//角色移动方向
	int mark;//得分
	int colorvolume;//颜料容量
	bool live;
	bool weapon;//TRUE为枪FALSE为画笔
	int HP;
};
struct ROLE myrole;
struct ENEMY
{
	double x;
	double y;
	double width;//不同敌人不同大小
	double height;
	double size;
	int direction;//敌人在一个区域往复运动，涉及运动方向
	int kind;//敌人种类
	bool live;
	int HP;//不同敌人血量不同，需要攻击不同次数，每次攻击HP--，感觉BOSS应该可以放进里面来
	struct ENEMY* next;
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
typedef struct dot {
	double x, y;
	struct dot* next;
}DOT;
typedef struct line {
	DOT* HeadDot;
	struct line* next;
}LINE;
#endif
