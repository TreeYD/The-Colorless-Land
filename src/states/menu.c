#include "stateManager.h"
#include "menu.h"
#include"gamecontrol.h"
#define MAXLEVEL 1
#define TOTRANK  4
#define LOCALTIME 14
#define EACHRANK 26
#define TITLEPOSX 7
#define TITLEPOSY 7.3
#define ICONSZ 0.7
#define ANIMATIONSTAGE 800
//标题对应图标的大小
#define BTWD0 3.5
#define BTHT0 0.8
//MainMenu界面的BUTTON的尺寸
#define BTWD1 11.35
#define BTHT1 0.8
#define BTWD2 3.5
#define BTHT2 0.8
//ArchMenu界面的BUTTON的尺寸

//总共设计了三种按钮大小
//archive1.txt  10002022/05/22|(14)000000000000|(12)
bool isDelete;//是否处于删除模式

ARCHINFO archBuf[3];
string archName[3] = { "archive1.txt","archive2.txt","archive3.txt" };
struct ArchInfo* curArch;
//在stateManagerinit的时候从文件中读入
//在回到MainMenu的时候再输出到文件里面，关卡结算的时候记得更新curArch的总得分和当前关卡的得分
//由于ArchMenu和LevelMenu是同一级的，所以跳转的时候记得要pop出去
//function for MainMenu
int curStage = 1;//当前所处的关卡
char settleInfo[] = { "第1关     获得分数: 000" };//关卡结算是需要用到的字符串，给了节省每次draw生成字符的内存开支，现在此处存下来
extern struct ROLE myrole;
extern StateManager statemanager;
extern State* StageArray[StageNum];

void setMainMenu(void);
void drawMainMenu(void);
void GetBackToMainMenu(void);
void ToHelp(void);
void MainToArch(void);
void ExitGame(void);
void NewGame(void);

//function for ArchMenu
void setArchMenu(void);
void drawArchMenu(void);
void setArchButton(void);
void InitNewGame();//生成新的存档
void SetArchZero(ARCHINFO *ptr);
void ToArch(void);
void AddArch(void);
void DeleteArch(void);
void LoadLevel1(void);
void LoadLevel2(void);
void LoadLevel3(void);
void *LoadLevelList[] = { LoadLevel1,LoadLevel2,LoadLevel3 };

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


/*for pause page*/
void setPauseMenu();
void drawPauseMenu();

/*for help page*/
void setHelpMenu();
void drawHelpMenu();
void drawHelpMenuP1();
void drawHelpMenuP2();
void Goback();
void helpEvent(int key, int event);
void TurnPage();
static int page = 1;

/*function for settlement page*/
void setSettleMenu();//在setSettleMenu时直接将myrole中的信息载入curArch
void drawSettleMenu();
void ToSettle();//关卡结算时触发，由于这关不会再用到了，所以直接Pop出栈
void ToNextStage();
void ToLevel();//使用当前存档加载Level界面

State SettleMenu = {
	"SETTLEMENU",
	setSettleMenu,
	drawSettleMenu,
	NULL,
	NULL,
	uiMouseEvent
};
State PauseMenu = {
	"PAUSEMENU",
	setPauseMenu,
	drawPauseMenu,
	NULL,
	NULL,
	uiMouseEvent,
};

State HelpMenu = {
	"HELPMENU",
	setHelpMenu,
	drawHelpMenu,
	NULL,
	helpEvent,
	uiMouseEvent,
};


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

State ArchMenu = {
	"ARCHMENU",
	setArchMenu,
	drawArchMenu,
	NULL,
	NULL,
	uiMouseEvent,
};

void MainToArch()
{
	//TO DO:load all info according to current Arch
	StatePush(&ArchMenu);
}
void ToArch() {
	StatePop(NULL);
	StatePush(&ArchMenu);
}
void AddArch(void)
{
	InitNewGame();
	CacheSorting();
	setArchButton();
	return;
}
void DeleteArch(void)
{
	isDelete = !isDelete;
	return;
}
void LoadRank(int x, int score)
{
	if (curArch == NULL)return;
	if (x > curArch->maxLevel)curArch->maxLevel = x;
	curArch->rank[0] -= curArch->rank[x] - score;
	curArch->rank[x] = score;
}
void WriteArchFile() {
	for (int i = 0; i < 3; i++) {
		FILE *fp = fopen(archName[i], "w");
		archBuf[i].rank[0] = 0;
		for (int j = 1; j <= (EACHRANK - LOCALTIME) / 2; j++)
			archBuf[i].rank[0] += archBuf[i].rank[j];
		fprintf(fp, "%d%03d%s", archBuf[i].maxLevel, archBuf[i].rank[0], archBuf[i].time);
		for (int j = 1; j <= (EACHRANK - LOCALTIME) / 2; j++) {
			fprintf(fp, "%02d", archBuf[i].rank[j]);
			printf("%d\n", archBuf[i].rank[j]);
		}
		fclose(fp);
	}
	return;
}
void ReadArchFile() {
	for (int i = 0; i < 3; i++) {
		FILE *fp = fopen(archName[i], "r");
		int cnt = MAXLEVEL;
		SetArchZero(&archBuf[i]);
		archBuf[i].maxLevel = fgetc(fp) - '0';
		while (cnt < TOTRANK) {
			archBuf[i].rank[0] *= 10;
			archBuf[i].rank[0] += fgetc(fp) - '0';
			cnt++;
		}
		while (cnt < LOCALTIME) {
			archBuf[i].time[cnt - TOTRANK] = fgetc(fp);
			cnt++;
		}
		archBuf[i].time[cnt - TOTRANK] = '\0';
		while (cnt < EACHRANK) {
			archBuf[i].rank[cnt - LOCALTIME + 1] = fgetc(fp) - '0';
			archBuf[i].rank[cnt - LOCALTIME + 1] = archBuf[i].rank[cnt - LOCALTIME + 1] * 10 + fgetc(fp) - '0';
			cnt += 2;
		}
		fclose(fp);
	}
	return;
}
void InitNewGame() {
	for (int i = 0; i < 3; i++) {
		if (archBuf[i].maxLevel > 0)continue;
		archBuf[i].maxLevel = 1;
		time_t systime = time(0);
		//产生“YYYY/MM/DD”格式的字符串
		strftime(archBuf[i].time, sizeof(archBuf[i].time), "%Y/%m/%d", localtime(&systime));
		curArch = &archBuf[i];//同时将新建存档设置为当前存档
		break;
	}
}

void SetArchZero(ARCHINFO* ptr)
{
	ptr->maxLevel = 0;
	for (int i = 0; i <= 6; i++)ptr->rank[i] = 0;
	strcpy(ptr->time, "0000/00/00");
	ptr->time[10] = '\0';
	return;
}

int animation_line = 0;
int animation_iv = 6;
double GetAnimationX(double staX, int x) {
	return staX + 0.95*ScaleXInches(x);
}
double GetAnimationY(double staY, int y) {
	int up = 110;
	int a = y / up, b = y % up;
	if (!(a & 1))return staY - 0.5*ScaleXInches(b);
	return staY - 0.5*ScaleXInches(up) + 0.5*ScaleXInches(b);

}
void setMainMenu()
{
	WriteArchFile();
	if (animation_line == -1)animation_line = 900;
}

void drawMainMenu()
{
	//TO DO: to draw a simple animation when first open MainMenu
	if (animation_line <= ANIMATIONSTAGE && animation_line >= 0) {
		//display logo
		DisplayClear();
		AddBitMap("logo.bmp", 3.5, 6, SRCCOPY);
		AddBitMap("spray.bmp", GetAnimationX(3.5, animation_line), GetAnimationY(6.1, animation_line), SRCCOPY);
		animation_line += animation_iv;
	}
	if (animation_line > 800) {
		double L = ScaleXInches(420);
		double H = ScaleXInches(250);
		BUTTON newGame;
		setButton(L, H, 0.1, BTWD0, BTHT0, "setting.bmp", "   继续游戏 ", MainToArch);
		setButton(L + BTWD0 + 0.4, H, 0.1, BTWD0, BTHT0, "help.bmp", "    帮助   ", ToHelp);
		setButton(L + BTWD0 + 0.4, H + BTHT0 + 0.2, 0.1, BTWD0, BTHT0, "exit.bmp", "  退出游戏 ", ExitGame);
		newGame = setButton(L, H + BTHT0 + 0.2, 0.1, BTWD0, BTHT0, "new.bmp", "   新建游戏 ", NewGame);
		newGame->isDisable = (archBuf[2].maxLevel);
		traverseButton();
		animation_line = -1;
	}
	if (animation_line == -1) {
		AddBitMap("logo.bmp", 3.5, 6, SRCCOPY);
		AddBitMap("spray.bmp", GetAnimationX(3.5, ANIMATIONSTAGE), GetAnimationY(6.1, ANIMATIONSTAGE), SRCCOPY);
		traverseButton();
	}
}

void GetBackToMainMenu()
{
	StatePop("MAINMENU");
}

void ExitGame()
{
	StateFree();
	FreeConsole();
	exit(0);
}

void NewGame(void)
{
	InitNewGame();
	StatePush(&LevelMenu);
}

void setArchMenu(void)
{
	isDelete = 0;

	setArchButton();

}
BUTTON add, del;
void drawArchMenu(void)
{
	AddZoomBitMap("archive.bmp", TITLEPOSX, TITLEPOSY - 0.15, ICONSZ, ICONSZ, SRCCOPY);
	setLabel(TITLEPOSX + ICONSZ, TITLEPOSY, 50, " 存档");
	add->isDisable = isDelete || (archBuf[2].maxLevel);
	double staX = 2.2, staY = 5.9 + BTHT1 + 0.25, W = BTWD1 + 0.2, H = -0.05;
	for (int i = 0; i < 3; i++) {
		if (!archBuf[i].maxLevel)break;
		staY -= BTHT1 + 0.25;
		H += BTHT1 + 0.25;
	}
	if (isDelete&&H > 0) {
		SetPenColor("TextGrey");
		drawBox(staX, staY, 0, W, H);
	}
	traverseButton();
	return;
}

void setArchButton(void)
{
	char i;
	double staX = 2.3, staY = 6;

	for (i = 0; i < 3; i++) {
		if (!archBuf[i].maxLevel)break;
		char info0[15], info1[85];
		sprintf(info0, "archive%d.bmp", i + 1);
		sprintf(info1, "      关卡进度 %d           得分 %03d           日期 %s   ",
			(archBuf[i].maxLevel<=6? archBuf[i].maxLevel :6), archBuf[i].rank[0], archBuf[i].time);
		setButton(staX, staY, 0.1, BTWD1, BTHT1, CopyString(info0), CopyString(info1), LoadLevelList[i]);
		staY -= BTHT1 + 0.25;
	}
	add = setButton(staX, staY, 0.1, BTWD2, BTHT2, "addarchive.bmp", "", AddArch);
	del = setButton(staX + BTWD2 + 0.45, staY, 0.1, BTWD2, BTHT2, "deletearchive.bmp", "", DeleteArch);
	setButton(staX + 2 * BTWD2 + 0.9, staY, 0.1, BTWD2, BTHT2, "menu.bmp", " 主菜单", GetBackToMainMenu);
	return;
}

void setLevelMenu(void)
{
	int i = 1, j = 1;
	double staX = 4.6, staY = 4.2;
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 3; i++) {
			int num = 4 - j * 3 + i;
			if (num <= curArch->maxLevel)setButton(staX + i * 2.4, staY + j * 1.5, 0.1, 1.75, 1, "", IntegerToString(num), LoadStageList[num - 1]);
			else {
				BUTTON newLevel = setButton(staX + i * 2.4, staY + j * 1.5, 0.1, 1.75, 1, "lock.bmp", "", LoadStageList[num - 1]);
				newLevel->isDisable = 1;//使得未开放的关卡没有点击效果
			}
		}
	}
	setButton(staX + 0.45, staY - 1.5, 0.1, 2.5, 0.8, "archive.bmp", " 存档", ToArch);
	setButton(staX + 3.45, staY - 1.5, 0.1, 2.5, 0.8, "menu.bmp", " 主菜单", GetBackToMainMenu);
	traverseButton();
	return;
}

void drawLevelMenu(void)
{
	AddZoomBitMap("flag.bmp", TITLEPOSX, TITLEPOSY - 0.1, ICONSZ, ICONSZ, SRCCOPY);
	setLabel(TITLEPOSX + ICONSZ, TITLEPOSY, 50, " 关卡");
	traverseButton();
	return;
}

void LoadLevel1(void)
{
	if (isDelete) {
		archBuf[0] = archBuf[1];
		archBuf[1] = archBuf[2];
		SetArchZero(&archBuf[2]);
		CacheSorting();
		setArchButton();
		isDelete = 0;
	}
	else {
		curArch = &archBuf[0];
		StatePop(NULL);//Pop the ArchMenu
		StatePush(&LevelMenu);
	}

}
void LoadLevel2(void)
{
	if (isDelete) {
		archBuf[1] = archBuf[2];
		SetArchZero(&archBuf[2]);
		CacheSorting();
		setArchButton();
		isDelete = 0;
	}
	else {
		curArch = &archBuf[1];
		StatePop(NULL);
		StatePush(&LevelMenu);
	}
}
void LoadLevel3(void)
{
	if (isDelete) {
		SetArchZero(&archBuf[2]);
		CacheSorting();
		setArchButton();
		isDelete = 0;
	}
	else {
		curArch = &archBuf[2];
		StatePop(NULL);
		StatePush(&LevelMenu);
	}
}

void ToHelp(void)
{
	StatePush(&HelpMenu);
}
void LoadStage1(void)
{
	curStage = 1;
	StatePush(StageArray[0]);

}
void LoadStage2(void)
{
	curStage = 2;
	StatePush(StageArray[1]);
}
void LoadStage3(void)
{
	curStage = 3;
	StatePush(StageArray[2]);
}
void LoadStage4(void)
{
	curStage = 4;
	StatePush(StageArray[3]);
}
void LoadStage5(void)
{
	curStage = 5;
	StatePush(StageArray[4]);
}
void LoadStage6(void)
{
	curStage = 6;
	StatePush(StageArray[5]);
}

void setPauseMenu()
{
	double x = GetWindowWidth();
	double y = GetWindowHeight();
	setButton(x / 2 - 0.75, y / 2 - 0.15, 0.1, 1.5, 0.4, "", "继续游戏", Goback);
	setButton(x / 2 - 0.75, y / 2 - 0.7, 0.1, 1.5, 0.4, "", "查看帮助", ToHelp);
	setButton(x / 2 - 0.75, y / 2 - 1.25, 0.1, 1.5, 0.4, "", "回到主菜单", GetBackToMainMenu);
}

void drawPauseMenu()
{
	string pause = "PAUSE";
	statemanager.stack[statemanager.top - 1]->draw();
	MovePen(4, 6);
	SetPenColor("white");
	StartFilledRegion(0);
	DrawLine(8, 0);
	DrawLine(0, -3);
	DrawLine(-8, 0);
	DrawLine(0, 3);
	EndFilledRegion();
	SetPenColor("black");
	MovePen(4, 6);
	DrawLine(8, 0);
	DrawLine(0, -3);
	DrawLine(-8, 0);
	DrawLine(0, 3);
	int regPointSize = GetPointSize();
	SetPointSize(regPointSize * 2.5);
	SetStyle(1);
	double width = TextStringWidth(pause);
	MovePen(GetWindowWidth() / 2 - width / 2, 5.5);
	DrawTextString(pause);
	SetPointSize(regPointSize);
	SetStyle(0);
	traverseButton();

}


void setHelpMenu()
{
	double x = GetWindowWidth();
	double y = GetWindowHeight();
	page = 1;
	setButton(x / 2 - 2.4, y / 4 - 1.5, 0.1, 1.8, 0.6, "", "返回", Goback);
	setButton(x / 2 + 0.6, y / 4 - 1.5, 0.1, 1.8, 0.6, "NextPage.bmp", "", TurnPage);

}

void drawHelpMenu()
{
	switch (page)
	{
	case 1:drawHelpMenuP1(); break;
	case 2:drawHelpMenuP2(); break;
	}
}
void drawHelpMenuP1()
{
	const string helpsP1[2][5] = { "W:跳跃","A/D:向左/右运动","F:切换模式","鼠标左键：绘制图线/向当前方向射击","鼠标右键：绘图模式下回收墨水","ESC：退出关卡回到主菜单","P：暂停游戏","H：帮助页面","←/→：（帮助页面）上一页/下一页" ,"" };
	int regPointSize = GetPointSize();
	SetPointSize(regPointSize * 2.5);
	SetStyle(1);
	double width = TextStringWidth("操作提示");
	MovePen(GetWindowWidth() / 2 - width / 2, 7);
	DrawTextString("操作提示");
	SetPointSize(regPointSize * 2);
	SetStyle(0);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (i == 1 && j == 4)
				continue;
			MovePen(1.5 + 7 * i, 6 - j * 1);
			DrawTextString(helpsP1[i][j]);
		}
	}
	SetPointSize(regPointSize);
	double x = GetWindowWidth() / 2 + 0.2;
	double y = GetWindowHeight() / 4;
	double w = 0.15, h = 0.3;
	traverseButton();
}

void Goback()
{
	StatePop(NULL);
}

void helpEvent(int key, int event)
{
	if ((page == 1 && key == VK_RIGHT && event == KEY_DOWN) || (page == 2 && key == VK_LEFT && event == KEY_DOWN))
	{
		TurnPage();
		return;
	}

}


void drawHelpMenuP2()
{
	int regPointSize = GetPointSize();
	SetPointSize(regPointSize * 2.5);
	SetStyle(1);
	double width = TextStringWidth("图标解释");
	MovePen(GetWindowWidth() / 2 - width / 2, 7);
	DrawTextString("图标解释");
	SetPointSize(regPointSize * 1.5);
	SetStyle(0);
	NewSetPenColor(40, 85, 242);
	StartFilledRegion(1);
	MovePen(GetWindowWidth() / 2 - 1, 5.5);
	DrawLine(2, 0);
	DrawLine(0, -0.2);
	DrawLine(-2, 0);
	DrawLine(0, 0.2);
	EndFilledRegion();
	MovePen(GetWindowWidth() / 2 - 1.4 - TextStringWidth("INK:"), 5.4 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("INK:");
	MovePen(GetWindowWidth() / 2 + 1.2, 5.4 - GetFontAscent() / 2);
	DrawTextString("当前剩余墨水量");
	NewSetPenColor(242, 11, 25);
	StartFilledRegion(1);
	MovePen(GetWindowWidth() / 2 - 1, 6);
	DrawLine(2, 0);
	DrawLine(0, -0.2);
	DrawLine(-2, 0);
	DrawLine(0, 0.2);
	EndFilledRegion();
	MovePen(GetWindowWidth() / 2 - 1.4 - TextStringWidth("HP:"), 5.9 - GetFontAscent() / 2);
	NewSetPenColor(0, 0, 0);
	DrawTextString("HP:");
	MovePen(GetWindowWidth() / 2 + 1.2, 5.9 - GetFontAscent() / 2);
	DrawTextString("当前剩余生命值");
	AddZoomBitMap("shoot.bmp", GetWindowWidth() / 2 - 0.4, 4, 0.8, 0.8, SRCCOPY);
	AddZoomBitMap("pen.bmp", GetWindowWidth() / 2 - 2.2, 4, 0.8, 0.8, SRCCOPY);
	AddZoomBitMap("ghost.bmp", GetWindowWidth() / 2 + 1.4, 4, 0.8, 0.8, SRCCOPY);
	SetPointSize(regPointSize);
	MovePen(GetWindowWidth() / 2 - TextStringWidth("射击模式") / 2, 3.2);
	DrawTextString("射击模式");
	MovePen(GetWindowWidth() / 2 - 1.8 - TextStringWidth("绘画模式") / 2, 3.2);
	DrawTextString("绘画模式");
	MovePen(GetWindowWidth() / 2 + 1.8 - TextStringWidth("敌人，可通过射击击破") / 2, 3.2);
	DrawTextString("敌人，可通过射击击破");
	traverseButton();
}

void TurnPage()
{
	double x = GetWindowWidth();
	double y = GetWindowHeight();
	if (page == 1)
	{
		CacheSorting();
		setButton(x / 2 - 2.4, y / 4 - 1.5, 0.1, 1.8, 0.6, "", "返回", Goback);
		setButton(x / 2 + 0.6, y / 4 - 1.5, 0.1, 1.8, 0.6, "PrevPage.bmp", "", TurnPage);
		page = 2;
	}
	else
	{
		CacheSorting();
		setButton(x / 2 - 2.4, y / 4 - 1.5, 0.1, 1.8, 0.6, "", "返回", Goback);
		setButton(x / 2 + 0.6, y / 4 - 1.5, 0.1, 1.8, 0.6, "NextPage.bmp", "", TurnPage);
		page = 1;
	}
	return;
}


void ToPause()
{
	StatePush(&PauseMenu);
}
void setSettleMenu()
{
	double staX = 2.3, staY = 4;
	LoadRank(curStage, myrole.mark);
	BUTTON Next=setButton(staX, staY, 0.1, BTWD2, BTHT2, "continue.bmp", "下一关", ToNextStage);
	if (myrole.live == FALSE||curStage==6)
		Next->isDisable = TRUE;
	setButton(staX + BTWD2 + 0.45, staY, 0.1, BTWD2, BTHT2, "flag.bmp", "关卡", ToLevel);
	setButton(staX + 2 * BTWD2 + 0.9, staY, 0.1, BTWD2, BTHT2, "menu.bmp", " 主菜单", GetBackToMainMenu);
	settleInfo[2] = curStage + '0';
	settleInfo[20] = myrole.mark / 100 + '0';
	settleInfo[21] = myrole.mark % 100 / 10 + '0';
	settleInfo[22] = myrole.mark % 10 + '0';
}
void drawSettleMenu()
{
	setLabel(TITLEPOSX - 0.3, TITLEPOSY, 50, "关卡结算");
	setLabel(5.5, 6, 40, settleInfo);
	traverseButton();
}
void ToSettle()
{
	if (strcmp(StateTop()->name, "MAINMENU") != 0)StatePop("MAINMENU");//POP掉除了MAINMENU以外的所有东西，包括当前关卡
	StatePush(&SettleMenu);
}
void ToNextStage()
{
	curStage++;
	StatePop(NULL);
	StatePush(StageArray[curStage - 1]);
}
void ToLevel()
{
	//curArch = &archBuf[0];
	StatePop(NULL);
	StatePush(&LevelMenu);
}