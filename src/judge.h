#ifndef JUDGE_H
#define JUDGE_H
#define JUMPBLOCKRANGEX 0.4
#define JUMPBLOCKRANGEY 0.1
#define JUMPDOTRANGEX 0.4
#define JUMPDOTRANGEY 0.05
#define MOVERANGE 0.15
bool distance(double x1, double y1, double size1, double x2, double y2, double size2);//����������ж��뾶ȷ������
bool JumpJudgeBlock();
bool RightMoveJudgeBlock();
bool LeftMoveJudgeBlock();
bool RoleAndEnemy(struct ENEMY enemy);//��ɫ�͵��˵���ײ�ж�
bool RoleAndGoal(struct GOAL goal);
bool EnemyAndBullet(struct ENEMY enemy, struct BULLET bullet);//���˺��ӵ�����ײ�ж�
bool BulletAndGround(struct BULLET bullet);
bool RoleAndBonus(struct BONUS bonus);//��ɫ���ռ�����ж�
bool JumpJudgeDot();
bool RightMoveJudgeDot();
bool LeftMoveJudgeDot();
bool MouseAndGround();//���λ�ú͵��������ж�����ͼʱ��
bool MouseAndLine(LINE*line);//�Ҽ���������ʹ��
bool MouseAndAllLine();
#endif
