#ifndef GAMECONTROL_H
#define GAMECONTROL_H
#define RENDERGAP 20
#define DRAWGAP 10
#define JUDGEGAP 500
#include "parameter.h"
enum timer {
	RENDER,
	LEFTMOVING, //左移动
	RIGHTMOVING, //右移动
	JUMP, //按W跳跃
	FALL, //从高地形到低地形
	JUDGE,//移动过程中与收集物和敌人的碰撞
	BULLETMAKE, //子弹生成
	SHOT, //点左键时的发射
	BULLETMOVE, //发射出去后的子弹移动
	DRAW//画笔timer，还没写
};//不同的timer直接枚举了
#define INITIALVERTICALSPEED 0.06
#define G 0.01
#define VOLUMEREDUCINGSPEED 1 //随便填的
void StartAutoTimer();
void CancelControlTimer();
void render(int TimerID);
void KeyBoardControl(int key, int event);//键盘事件处理
void MouseControl(int x, int y, int button, int event);//鼠标事件处理
void PlayerMove(int event);//根据不同的方向event有不同坐标的改变
void BonusJudge();//判断得到收集物的函数
void EnemyJudge();//判断与敌人碰撞掉血的函数
void GoalJudge();
void BulletMake();//将bullet.live为FALSE的子弹改为TRUE
void Shot();//按W时让bullet.live和bullet.IsMoving为TRUE,并确定子弹的速度
void BulletMove();//一直遍历子弹数组让bullet.IsMoving为TRUE的子弹一直运动
void MakeLine();
void PickUpDots();
void AddLine(LINE* NewLine);
void AddDot(DOT* NewDot);
void Delete();
void DeleteLine(LINE* line);
void CacheLineSorting(LINE* line);
void recycleInk(LINE* line);
double MouseX, MouseY;//全局变量鼠标坐标
double COS, SIN;//角色与鼠标位置连线夹角
static double VerticalSpeed = INITIALVERTICALSPEED;//角色在地面上向上跳起的初速度
static double FallingSpeed = 0;//角色从高处掉到地处的初速度
static bool IsJumping = FALSE;
static bool IsDropping = FALSE;
static bool IsDrawing = FALSE;
#endif

