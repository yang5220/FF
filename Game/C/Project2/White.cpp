#include<Windows.h>
#include"White.h"
int score = 0;
int level = 0;
int K[COL][ROW];

GAME_COORD boundary;

void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}
PGAME_COORD GetBoundary()
{
	return &boundary;
}

int*GetK()
{
	return *K;
}
POINT GetPosit(int i, int j)
{
	POINT P;
	P.x = j;
	P.y = i;
	return P;
}
int GetScore()
{
	return score;
}
int GetLevel()
{
	return level;
}
void AddScore()
{
	score++;
}
void AddLevel()
{
	level++;
}

void CreateKuai()
{
	int i, j, r;
	for (i = 0; i < COL; i++)
	{
		r = rand() % ROW;
		for (j = 0; j < ROW; j++)
		{
			if (r == j)
				K[i][j] = 1;//°×¿é
			else
				K[i][j] = 0;//ºÚ¿é
		}
	}

}

bool ReInit()
{
	int i, j, r;
	////ÏÂÒ»ÅÅ±»ÉÏÒ»ÅÅ¸²¸Ç
	for (i = COL - 1; i > 0; i--)
	{
		for (j = 0; j < ROW; j++)
		{
			K[i][j] = K[i - 1][j];
		}
	}
	r = rand() % ROW;
	for (j = 0; j < ROW; j++)
	{
		if (j == r)
		{
			K[0][j] = 1;
		}
		else
			K[0][j] = 0;
	}
	return true;
}