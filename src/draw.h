#ifndef DRAW_H
#define DRAW_H
#define LINEDURATION 250  //һ����5s����ɫ��50FPS��
#define VOLUMEREDUCINGSPEED 1 //������
void DrawingRouter(double x1, double y1, int button, int event);//���������·������ݵ�ǰ������ģʽ���ã�ע�⴫��ʱ�Ѿ���x��y��λ�任ΪӢ��
void PickUpDots(void);//������DRAW��Timer�����
void DrawSavedLines(void);//������RENDER�����
#endif 