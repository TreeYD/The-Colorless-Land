#ifndef DRAW_H
#define DRAW_H
#define LINEDURATION 250  //一条线5s后褪色（50FPS）
#define VOLUMEREDUCINGSPEED 1 //随便填的
void DrawingRouter(double x1, double y1, int button, int event);//建议在鼠标路由里根据当前鼠标操作模式调用，注意传参时已经把x，y单位变换为英寸
void PickUpDots(void);//建议在DRAW的Timer里调用
void DrawSavedLines(void);//建议在RENDER里调用
#endif 