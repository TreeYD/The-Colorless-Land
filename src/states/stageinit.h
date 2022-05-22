#ifndef STAGEINIT_H
#define STAGEINIT_H
void StageSetting();
//初始化一切关卡
void RoleSetting(double x, double y);
//设置角色的初始位置为(x,y)

void EnemySetting(int i, double x, double y, double range, int direction, int kind);
//敌人用数组存储 让第i个敌人的初始位置在(x,y) 敌人种类为kind(分1，2，3三类) 起始运动方向为direcion(LEFT RIGHT) 运动范围是range(往复运动)

void BonusSetting(int i, double x, double y, bool IsColor);
//辅助函数 被用于BonusSettingX
//或者用于设置单个收集物 让第i个收集物在(x,y)位置 IsColor为TRUE时是颜料收集物 反之为加分收集物

void BonusSettingX(int i, double StartX, double StartY, bool IsColor, double gap, int number);
//从起始点(startX,startY)开始 横向从左向右铺排number个IsColor类的收集物 每个收集物之间的距离是gap 第一个铺排的是第i号收集物 
//IsColor为TRUE时是颜料收集物 反之为加分收集物

void BlockSetting(double X, double Y);
//辅助函数 被用于BlockSettingX和BlockSettingY

void BlockSettingX(double StartX, double StartY, int number);
//从起始点(StartX,StartY)开始 横向自左向右铺排number个block

void GoalSetting(double x, double y);
//设置终点的坐标

void StageClear();
void BlockClear();
void LineClear();
void EnemyClear();
void BonusClear();
void BulletClear();
void GoalClear();
//清空关卡状态的函数 放在state的destroy里
void InitSetting();
//用于把设定好的StageInit赋值给函数指针数组
void StageInit1();
void StageInit2();
void StageInit3();
void StageInit4();
void StageInit5();
void StageInit6();
//放在state的init里
void SettingMisc(void);
void FreeAllStages();
//called when exit
#endif // !STAGE_H
