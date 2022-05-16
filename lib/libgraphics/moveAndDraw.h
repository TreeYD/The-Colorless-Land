#ifndef MOVEANDDRAW_H

#define MOVEANDDRAW_H

typedef struct dot {
	double x, y;
	struct dot *next;
}DOT;
typedef struct line {
	DOT* headDot;
	int duration;
	struct line* next;
}LINE;


#define RENDERGAP 20
/*ID for different kinds of timer*/
#define RENDER 0
#define RIGHTMOVING 1
#define LEFTMOVING 2
#define JUMP 3
#define PICKUPDOTS 4
#define FALL 5//falling event for the character, haven't decided how to deal with that yet...
/*vertical movement parameters*/
#define INITIALVERTICALSPEED 0.65
#define G 0.1
/*pics for player
*may change later but anyway...
*/
#define PICSTILL "PICSTILL.bmp"
#define PICRIGHT1 "PICRIGHT1.bmp"
#define PICRIGHT2 "PICRIGHT2.bmp"
#define PICLEFT1 "PICLEFT1.bmp"
#define PICLEFT2 "PICLEFT2.bmp"
/*parameters for drawing*/
#define LINEDURATION 250 //each line fades out after 5 seconds (when the FPS is 50)
#define VOLUMEREDUCINGSPEED 1 //in testing environment,need to change when published
/*TESTING VAIRABLES*/
#define TESTY 4


void move(int key, int event);//deal with keyboard movement
void render(int TimerID);//act as a timer router, 5 timers in present: 1:refresh the screen; 2:move left;3:move right;4:jump(vertical movement);5:pick up dots
void playerMove(int event);
void playerJump(void);
void playerFall(void);//not finished yet.
void mouseRouter(int x, int y, int button, int event);//a router, drawing mode only for now, may add more patterns later
void PickUpDots(void);//pick up dots of each line
void AddLine(LINE* newLine);//add a LINE unit to the LINE linklist
void AddDot(DOT *newDot);//add a DOT unit to the LINE linklist
void DrawSavedLines(void);//render all the existing lines, called when the screen refreshed
void FreeDotList(LINE*line);//free all the dots in a line
void FreeFirstLine(void);//free a line unit, called when a line's duration is reduced to 0
char * currPic;//contains the current illustration for the character
double mouseX, mouseY;//contains the current position of the mouse
static double verticalSpeed;//the vertical speed of the character, reinitialized each time when the jump event starts
static bool isFloating = FALSE;//to represent whether the character is up in the air
static double xspeed = 0.1;//horizontal speed of the character.

#endif