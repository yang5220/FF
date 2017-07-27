#pragma once
#include<Windows.h>
#pragma comment(lib,"winmm.lib")
#define ROW         4
#define COL         5
#define Click_W      1
#define Click_B     0
#define Click_Error 2
#define Score       30

typedef struct _GAME_COORD
{
	int x;
	int y;
}GAME_COORD, *PGAME_COORD;

HBITMAP getNote1(HWND);
void PlayMusic();
void AddLevel();
void AddScore();
int GetScore();
int GetLevel();


void SetBoundary(int x, int y);
PGAME_COORD GetBoundary();

int* GetK();
POINT GetPosit(int, int);

void CreateKuai();//初始化黑白块位置
bool ReInit();//点击一次后更新黑白块位
bool SpeedUp(HWND hwnd);//升级后加快游戏速度

void OnKeyDown(DWORD vk, HWND hwnd);//每按下一个键并判断后
									//int BlcOrWht(int);//返回按键信息

									//计时器到时后，根据按键信息进行下一步处理		  
void OnTimer(HWND hwnd);

void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y);

