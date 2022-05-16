#ifndef _lightgui_h
#define _lightgui_h



void drawBox(double x, double y, double r,double w, double h);
//take (x,y) as the bottom left corner , draw a box with width "w" ,height "h" ,and radius "r"
void setButton(double x, double y, double r,double w, double h, string icon, string text, void *func);
//draw a button, and put a icon and text in the center of the box, the height of the font is 1/3 of the box height
void setLabel(double x, double y, int fontSize, string text); //e.g. fontSize 5 means the font height is 5 px 
//draw a label, fontSize means the pixel height of the text
void setSeekbar(double x, double y, double w, double h, double *Obj);
//draw a seekbar, w means the length of seekbar, h means the height of seekbar, Obj means the address of the controlled variable
void setTextbox(double x, double y, double r, double w, double h,string hint,int maxLength);
/*draw a textbox, 
w means the width of textbox, 
h means the height of textbox,
the font size is 1/3 of the box height,
the maxlength means the maxlength you can input*/
//and before input there will a hint in color "Grey" in the box
void InitGUI();
//init GUI, including define some colors, register some events, etc.
void freeButton();
// free all the button, make it disable
void freeTextbox();
// free all the textbox, make it disable
void freeSeekbar();
// free all the seekbar, make it disable
void CacheSorting(void);
void traverseButton(void);
void uiMouseEvent(int x, int y, int button, int event);
#endif