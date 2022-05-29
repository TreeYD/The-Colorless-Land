#ifndef GAMECONTROL_H
#define GAMECONTROL_H
#define RENDERGAP 20
#define DRAWGAP 10
#include "parameter.h"
enum timer {
	RENDER,
	LEFTMOVING, //���ƶ�
	RIGHTMOVING, //���ƶ�
	JUMP, //��W��Ծ
	FALL, //�Ӹߵ��ε��͵���
	JUDGE,//�ƶ����������ռ���͵��˵���ײ
	BULLETMAKE, //�ӵ�����
	SHOT, //�����ʱ�ķ���
	BULLETMOVE, //�����ȥ����ӵ��ƶ�
	DRAW,//����timer����ûд
	HP,//monitor life status
};//��ͬ��timerֱ��ö����
#define INITIALVERTICALSPEED 0.15
#define G 0.01
#define VOLUMEREDUCINGSPEED 0.1 
#define ColorBonusVolume 10
void StartAutoTimer();
void CancelControlTimer();
void render(int TimerID);
void KeyBoardControl(int key, int event);//�����¼�����
void MouseControl(int x, int y, int button, int event);//����¼�����
void PlayerMove(int event);//���ݲ�ͬ�ķ���event�в�ͬ����ĸı�
void BonusJudge();//�жϵõ��ռ���ĺ���
void EnemyJudge();//�ж��������ײ��Ѫ�ĺ���
void GoalJudge();
void BulletMake();//��bullet.liveΪFALSE���ӵ���ΪTRUE
void Shot();//��Wʱ��bullet.live��bullet.IsMovingΪTRUE,��ȷ���ӵ����ٶ�
void BulletMove();//һֱ�����ӵ�������bullet.IsMovingΪTRUE���ӵ�һֱ�˶�
void MakeLine();
void PickUpDots();
void AddLine(LINE* NewLine);
void AddDot(DOT* NewDot);
void Delete();
void DeleteLine(LINE* line);
void CacheLineSorting(LINE* line);
void recycleInk(LINE* line);
void HPMonitor(void);
double MouseX, MouseY;//ȫ�ֱ����������
double COS, SIN;//��ɫ�����λ�����߼н�
static double VerticalSpeed = INITIALVERTICALSPEED;//��ɫ�ڵ�������������ĳ��ٶ�
static double FallingSpeed = 0;//��ɫ�Ӹߴ������ش��ĳ��ٶ�

#endif

