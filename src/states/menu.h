#ifndef _MENU_H
#include "StateManager.h"
#define _MENU_H
typedef struct ArchInfo {
	int maxLevel;
	char time[12];
	int rank[8];//rank[0]�洢�ܷ֣�rank[1-6]�ֱ�洢1-6�ķ��� 
}ARCHINFO;
void ReadArchFile();//���ļ��ж�ȡ���д浵
void WriteArchFile();//���ļ���������д浵,�Ѿ�������setMainMenu���棬���ص����˵���ʱ��Ὣ�浵������ļ���
void LoadRank(int x, int score);//����x�صķ���score���뵱ǰ�浵���ڹؿ������ʱ�����
void ToPause(void);
void ToSettle(void);
#endif // !MENU_H
