#ifndef _MENU_H
#include "StateManager.h"
#define _MENU_H
typedef struct ArchInfo {
	int maxLevel;
	char time[12];
	int rank[8];//rank[0]存储总分，rank[1-6]分别存储1-6的分数 
}ARCHINFO;
void ReadArchFile();//从文件中读取所有存档
void WriteArchFile();//在文件中输出所有存档,已经放在了setMainMenu里面，即回到主菜单的时候会将存档输出到文件中
void LoadRank(int x, int score);//将第x关的分数score输入当前存档，在关卡结算的时候调用
void ToPause(void);
void ToSettle(void);
#endif // !MENU_H
