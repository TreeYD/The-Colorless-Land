#ifndef GAMECONTROL_H
#define GAMECONTROL_H
#define RENDERGAP 20
enum timer {
	RENDER,
	LEFTMOVING, //���ƶ�
	RIGHTMOVING, //���ƶ�
	JUMP, //��W��Ծ
	FALL, //�Ӹߵ��ε��͵���
	BULLETMAKE, //�ӵ�����
	SHOT, //�����ʱ�ķ���
	BULLETMOVE, //�����ȥ����ӵ��ƶ�
	DRAW//����timer����ûд
};//��ͬ��timerֱ��ö����
#define INITIALVERTICALSPEED 0.65
#define G 0.1
void render(int TimerID);
void KeyBoardControl(int key, int event);//�����¼�����
void MouseControl(int x, int y, int button, int event);//����¼�����
void PlayerMove(int event);//���ݲ�ͬ�ķ���event�в�ͬ����ĸı�
void BulletMake();//��bullet.liveΪFALSE���ӵ���ΪTRUE
void Shot();//��Wʱ��bullet.live��bullet.IsMovingΪTRUE,��ȷ���ӵ����ٶ�
void BulletMove();//һֱ�����ӵ�������bullet.IsMovingΪTRUE���ӵ�һֱ�˶�
double MouseX, MouseY;//ȫ�ֱ����������
double COS, SIN;//��ɫ�����λ�����߼н�
static double VerticalSpeed = INITIALVERTICALSPEED;//��ɫ�ڵ�������������ĳ��ٶ�
static double FallingSpeed = 0;//��ɫ�Ӹߴ������ش��ĳ��ٶ�
static bool IsFloating = FALSE;//��ɫ����������е�һ��flag
#endif

