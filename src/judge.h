#ifndef JUDGE_H
#define JUDGE_H
bool distance(double x1, double y1, double size1, double x2, double y2, double size2);//����������ж��뾶ȷ������
bool RoleAndGroundX(struct ROLE myrole, struct BLOCK* blockhead);//��ɫ������ˮƽ����ֱ�ж�
bool RoleAndGroundY(struct ROLE myrole, struct BLOCK* blockhead);
bool RoleAndEnemy(struct ROLE myrole, struct ENEMY enemy);//��ɫ�͵��˵���ײ�ж�
bool EnemyAndBullet(struct ENEMY enemy, struct BULLET bullet);//���˺��ӵ�����ײ�ж�
#endif
