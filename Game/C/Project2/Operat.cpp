#include<Windows.h>
#include"White.h"

#define TIMER_ID 12340
#define ONELEVELUP   5//升级所需的分数
DWORD dwTimerElapse;
DOUBLE dbLevelSpeedupRatio;//升级后游戏加快的系数


HBITMAP hbmpNote;
char note[10][15] = { "note0.bmp","note1.bmp","note2.bmp","note3.bmp","note4.bmp","note5.bmp","note6.bmp","note7.bmp"
,"note8.bmp","note9.bmp" };


void PlayMusic()
{
	PlaySound("姚远 - 她说 (钢琴版) [mqms2].wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}



bool SpeedUp(HWND hwnd)
{
	dwTimerElapse = (DWORD)(dwTimerElapse * dbLevelSpeedupRatio);
	KillTimer(hwnd, TIMER_ID);
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	return true;
}

void OnTimer(HWND hwnd)
{
	ReInit();
}

void OnKeyDown(DWORD vk, HWND hwnd)//判断按下的块是否是白块，并将信息返回
{

	int flag = 0;
	switch (vk)
	{
	case'A':
		if (*(GetK() + 16) == Click_W)
		{
			flag = 1;
		}
		break;
	case'S':
		if (*(GetK() + 17) == Click_W)
		{
			flag = 1;
		}
		break;
	case'D':
		if (*(GetK() + 18) == Click_W)
		{
			flag = 1;
		}
		break;
	case'F':
		if (*(GetK() + 19) == Click_W)
		{
			flag = 1;
		}
		break;

	default:
		flag = 2;
		break;
	}
	if (flag == 1)//按下白块
	{
		AddScore();
		if (GetScore() >= Score)//赢得游戏
		{
			PlaySound("s5.wav", NULL, SND_FILENAME | SND_ASYNC);
			KillTimer(hwnd, TIMER_ID);
			MessageBox(0, "You Win!", "You Win", 0);
			ExitProcess(0);
		}

		if (GetScore() % ONELEVELUP == 0)
		{
			AddLevel();
			SpeedUp(hwnd);
		}
	}
	else if (flag == 0)
	{
		PlaySound("s8.wav", NULL, SND_FILENAME | SND_ASYNC);
		KillTimer(hwnd, TIMER_ID);
		MessageBox(0, "Game Over", "Game Over", 0);
		ExitProcess(0);
	}
}
HBITMAP getNote1(HWND hwnd)
{


	int s;
	s = rand() % 10;

	hbmpNote = (HBITMAP)LoadImage(NULL, note[s],
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


	if (hbmpNote == NULL)
	{
		MessageBox(hwnd, "bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}
	return hbmpNote;
}

void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y)
{
	FILETIME ft;

	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);

	dwTimerElapse = dwInitTimerElapse;
	dbLevelSpeedupRatio = level_speedup_ratio;
	SetBoundary(boundary_x, boundary_y);
	CreateKuai();
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	PlayMusic();

}