#include "stateManager.h"
#include "menu.h"
#include"gamecontrol.h"
#include "lightgui.h"
#include "helpAndPause.h"
#include"stageinit.h"
/*CurrentRank: temporary variable to represent present stage process. Waiting to be changed.*/
//archive1.txt  10002022/05/22
//空存档的内容为 0
//function for main menu
int MaxLevel;//当前到达的最远的关卡
bool isDelete;//当前是否处于删除存档的状态
BUTTON arch_button[3];
void setMainMenu(void);
void drawMainMenu(void);
void GetBackToMainMenu(void);
void ToHelp(void);
void NewGame(void);
void ExitGame(void);
string CheckArchive(void);
void NewArchive(string name);

//function for ArchiveMenu
void setArchiveMenu(void);
void drawArchiveMenu(void);
// void ToArchive(void);
// void AddArchive(void);
// void DeleteArchive(void);
// void WriteFile(string name, string content);
// string ReadFile(string name);
// void LoadLevel1(void);
// void LoadLevel2(void);
// void LoadLevel3(void);
// void *LoadLevelList[] = { LoadLevel1,LoadLevel2,LoadLevel3 };

//function for Level selection
void setLevelMenu(void);
void drawLevelMenu(void);
void LoadStage1(void);
void LoadStage2(void);
void LoadStage3(void);
void LoadStage4(void);
void LoadStage5(void);
void LoadStage6(void);
void *LoadStageList[] = { LoadStage1,LoadStage2,LoadStage3,LoadStage4,LoadStage5,LoadStage6 };
//储存LoadStage各个关卡的函数
extern State HelpMenu;
extern State* StageArray[StageNum];
State  MainMenu = {
	"MAINMENU",
	setMainMenu,
	drawMainMenu,
	NULL,
	NULL,
	uiMouseEvent,
};
State LevelMenu = {
	"LEVELMENU",
	setLevelMenu,
	drawLevelMenu,
	NULL,
	NULL,
	uiMouseEvent
};
State ArchiveMenu = {
	"ARCHIVEMENU",
	setArchiveMenu,
	drawArchiveMenu,
	NULL,
	NULL,
	uiMouseEvent,
};
void ToArchive()
{
	//TO DO:load all info according to current archive
	StatePush(&ArchiveMenu);
}


int animation_line = 0;
int animation_iv = 6;
double GetAnimationX(double staX, int x) {
	return staX + ScaleXInches(x) - 0.2;
}
double GetAnimationY(double staY, int y) {
	int up = 100;
	int a = y / up, b = y % up;
	if (a & 1)return staY - 0.5*ScaleXInches(b);
	return staY - 0.5*ScaleXInches(up) + 0.5*ScaleXInches(b);

}
void setMainMenu()
{
	if (animation_line == -1)animation_line = 900;
}

void drawMainMenu()
{
	//TO DO: to draw a simple animation when first open MainMenu
	if (animation_line <= 800 && animation_line >= 0) {
		//display logo
		DisplayClear();
		AddBitMap("logo.bmp", 4, 1, SRCCOPY);
		AddBitMap("spray.bmp", GetAnimationX(4, animation_line), GetAnimationY(1.1, animation_line), SRCCOPY);
		animation_line += animation_iv;
	}
	if (animation_line > 800) {
		double L = ScaleXInches(450);
		double H = ScaleXInches(175);
		double w = 3, h = 0.8;
		BUTTON New;
		//setButton(L, H, 0.1, w, h, "setting.bmp", "   继续游戏 ", ToArchive);
		setButton(L, H, 0.1, w, h, "setting.bmp", "   继续游戏 ", LoadStage1);
		setButton(L + w + 0.6, H, 0.1, w, h, "help.bmp", "    帮助   ", ToHelp);
		New = setButton(L, H + h + 0.4, 0.1, w, h, "new.bmp", "   新建游戏 ", NewGame);
		setButton(L + w + 0.6, H + h + 0.4, 0.1, w, h, "exit.bmp", "  退出游戏 ", ExitGame);
		New->isDisable = (CheckArchive() == "archive4.txt");
		traverseButton();
		animation_line = -1;
	}
	if (animation_line == -1) {
		AddBitMap("logo.bmp", 4, 1, SRCCOPY);
		traverseButton();
	}
}


void GetBackToMainMenu()
{
	StatePop("MAINMENU");//or StatePop(NULL)
}

void NewGame()
{
	DisplayClear();
	string name = CheckArchive();
	NewArchive(name);
	StatePush(&LevelMenu);
}

void ExitGame()
{
	StateFree();
	FreeConsole();
	exit(0);
}

string CheckArchive()//返回空存档的文件名
{
	string name;
	char i;
	for (i = '1'; i <= '3'; i++) {
		name = Concat("archive", Concat(CharToString(i), ".txt"));
		FILE* fp = fopen(name, "r");
		if (fgetc(fp) == '0') {//空文档的存档开头字符为'0'
			fclose(fp);
			return name;
		}
		fclose(fp);
	}
	return "archive4.txt";//返回的文件名为”archive4.txt“即表明存档已满
}

void NewArchive(string name)
{
	if (name == "archive4.txt")return;
	FILE *fp = fopen(name, "w");
	SYSTEMTIME systime;
	GetLocalTime(&systime);//本地时间
	fprintf(fp, "1000%04d/%02d/%02d", systime.wYear, systime.wMonth, systime.wDay);
	fclose(fp);
	MaxLevel = 1;
}

void setArchiveMenu(void)
{
	return;
}

void drawArchiveMenu(void)
{
	return;
}

void setLevelMenu(void)
{
	DisplayClear();
	double L = ScaleXInches(660);
	double H = ScaleXInches(675);
	double w = 0.6;
	double h = 0.6;
	AddZoomBitMap("flag.bmp", L, H - 0.1, w, h, SRCCOPY);
	setLabel(L + w, H, 50, "关卡");
	int i = 1, j = 1;
	double staX = 4.6, staY = 3.85;
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 3; i++) {
			int num = 4 - j * 3 + i;
			if (num <= MaxLevel)setButton(staX + i * 2.4, staY + j * 1.5, 0.1, 1.75, 1, "", IntegerToString(num), LoadStageList[num - 1]);
			else {
				BUTTON new_button = setButton(staX + i * 2.4, staY + j * 1.5, 0.1, 1.75, 1, "lock.bmp", "", LoadStageList[num - 1]);
				new_button->isDisable = 1;//使得未开放的关卡没有点击效果
			}
		}
	}
	setButton(staX + 0.27, staY - 1.5, 0.1, 2.5, 0.8, "archive.bmp", " 存档", ToArchive);
	setButton(staX + 3.27, staY - 1.5, 0.1, 2.5, 0.8, "menu.bmp", " 主菜单", GetBackToMainMenu);
	traverseButton();
	return;
}

void drawLevelMenu(void)
{
	double L = ScaleXInches(660);
	double H = ScaleXInches(675);
	double w = 0.6;
	double h = 0.6;
	AddZoomBitMap("flag.bmp", L, H - 0.1, w, h, SRCCOPY);
	setLabel(L + w, H, 50, "关卡");
	traverseButton();
	return;
}


void ToHelp(void)
{
	StatePush(&HelpMenu);
}
void LoadStage1(void)
{

	StatePush(StageArray[0]);
}
void LoadStage2(void)
{
	/*
	StatePush(&StateArray[1]);
	*/
}
void LoadStage3(void)
{
	/*
	StatePush(&StateArray[2]);
	*/
}
void LoadStage4(void)
{
	/*
	StatePush(&StateArray[3]);
	*/
}
void LoadStage5(void)
{
	/*
	StatePush(&StateArray[4]);
	*/
}
void LoadStage6(void)
{
	/*
	StatePush(&StateArray[5]);
	*/
}
