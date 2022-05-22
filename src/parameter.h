#ifndef PARAMETER_H
#define PARAMETER_H

#define GraphicsWindowWidth 16//���ڴ�С
#define GraphicsWindowHeight 9
#define RoleSpeed 0.2//��ɫ�ƶ��ٶ�
#define RoleWidth 0.5//��ɫ��С
#define RoleHeight 0.75
#define InitialHP 100
#define InitialColorVolume 10
#define MaxColorVolume 100
#define EnemySpeed 0.02//�����ƶ��ٶ�
#define EnemyNum 10//һ���ؿ��е��˵��������
#define BulletSpeed 0.3//�ӵ��ƶ��ٶ�
#define BulletWidth 0.04//�ӵ���С
#define BulletHeight 0.04
#define BulletSize 0.02//�ӵ��ж��뾶
#define BulletNum 100//�ӵ��������
#define BlockSize 0.25
#define BonusNum 30
#define BonusSize 0.15
#define GoalSize 0.15
#define DotSize 0.1
#define StageNum 20
#include"genlib.h"
enum DIRECTION
{
	LEFT = -1,
	RIGHT = 1//ö�ٷ�������
};
struct ROLE
{
	double x;
	double y;
	int direction;//��ɫ�ƶ�����
	int mark;//�÷�
	double colorvolume;//��������
	bool live;
	bool weapon;//TRUEΪǹFALSEΪ����
	int HP;
	bool IsMoving;
};
struct ROLE myrole;
struct ENEMY
{
	double x;
	double y;
	double width;//��ͬ���˲�ͬ��С
	double height;
	double size;
	double moverange;//�ƶ���Χ
	double nowrange;
	int direction;//������һ�����������˶����漰�˶�����
	int kind;//��������
	bool live;
	int HP;//��ͬ����Ѫ����ͬ����Ҫ������ͬ������ÿ�ι���HP--���о�BOSSӦ�ÿ��ԷŽ�������
};
struct ENEMY enemy[EnemyNum];//����������洢
struct BULLET
{
	double x;
	double y;
	double SpeedX;//�������λ�úͽ�ɫλ��ȷ���нǣ�Ȼ���ٶ������ֽ�һ��
	double SpeedY;
	bool live;
	bool IsMoving;
};
struct BULLET bullet[BulletNum];//�ӵ�������洢
struct BLOCK {
	double x;
	double y;
	struct BLOCK* next;
};
struct BLOCK* blockhead;//�������������洢
struct BONUS {
	double x;
	double y;
	bool IsColor;//�ռ������� һ�ּӷ�һ�ּ�����
	bool live;
};
struct BONUS bonus[BonusNum];
struct GOAL {
	double x;
	double y;
	bool live;
};
struct GOAL NowGoal;
typedef struct dot {
	double x, y;
	struct dot* next;
}DOT;
typedef struct line {
	DOT* HeadDot;
	struct line* next;
}LINE;
#endif//old
