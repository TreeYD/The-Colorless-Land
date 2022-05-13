#ifndef PARAMETER_H
#define PARAMETER_H

#define GraphicsWindowWidth 16//���ڴ�С
#define GraphicsWindowHeight 9
#define RoleSpeed 0.5//��ɫ�ƶ��ٶ�
#define RoleWidth 0.5//��ɫ��С
#define RoleHeight 1
#define EnemySpeed 0.5//�����ƶ��ٶ�
#define EnemyNum 10//һ���ؿ��е��˵��������
#define BulletSpeed 3//�ӵ��ƶ��ٶ�
#define BulletWidth 0.01//�ӵ���С
#define BulletHeight 0.002
#define BulletSize 0.005//�ӵ��ж��뾶
#define BulletNum 100//�ӵ��������
#define BlockSize 0.5
enum DIRECTION
{
	LEFT, RIGHT//ö�ٷ�������
};
struct ROLE
{
	double x;
	double y;
	int direction;//��ɫ�ƶ�����
	int mark;//�÷�
	int colorvolume;//��������
	bool live;
	bool weapon;//TRUEΪǹFALSEΪ����
	int HP;
};
struct ROLE myrole;
struct ENEMY
{
	double x;
	double y;
	double width;//��ͬ���˲�ͬ��С
	double height;
	double size;
	int direction;//������һ�����������˶����漰�˶�����
	int kind;//��������
	bool live;
	int HP;//��ͬ����Ѫ����ͬ����Ҫ������ͬ������ÿ�ι���HP--���о�BOSSӦ�ÿ��ԷŽ�������
	struct ENEMY* next;
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
#endif
