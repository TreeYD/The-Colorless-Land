#ifndef STAGEINIT_H
#define STAGEINIT_H
void StageSetting();
//��ʼ��һ�йؿ�
void RoleSetting(double x, double y);
//���ý�ɫ�ĳ�ʼλ��Ϊ(x,y)

void EnemySetting(int i, double x, double y, double range, int direction, int kind);
//����������洢 �õ�i�����˵ĳ�ʼλ����(x,y) ��������Ϊkind(��1��2��3����) ��ʼ�˶�����Ϊdirecion(LEFT RIGHT) �˶���Χ��range(�����˶�)

void BonusSetting(int i, double x, double y, bool IsColor);
//�������� ������BonusSettingX
//�����������õ����ռ��� �õ�i���ռ�����(x,y)λ�� IsColorΪTRUEʱ�������ռ��� ��֮Ϊ�ӷ��ռ���

void BonusSettingX(int i, double StartX, double StartY, bool IsColor, double gap, int number);
//����ʼ��(startX,startY)��ʼ ���������������number��IsColor����ռ��� ÿ���ռ���֮��ľ�����gap ��һ�����ŵ��ǵ�i���ռ��� 
//IsColorΪTRUEʱ�������ռ��� ��֮Ϊ�ӷ��ռ���

void BlockSetting(double X, double Y);
//�������� ������BlockSettingX��BlockSettingY

void BlockSettingX(double StartX, double StartY, int number);
//����ʼ��(StartX,StartY)��ʼ ����������������number��block

void GoalSetting(double x, double y);
//�����յ������

void StageClear();
void BlockClear();
void LineClear();
void EnemyClear();
void BonusClear();
void BulletClear();
void GoalClear();
//��չؿ�״̬�ĺ��� ����state��destroy��
void InitSetting();
//���ڰ��趨�õ�StageInit��ֵ������ָ������
void StageInit1();
void StageInit2();
void StageInit3();
void StageInit4();
void StageInit5();
void StageInit6();
//����state��init��
void SettingMisc(void);
void FreeAllStages();
//called when exit
#endif // !STAGE_H
