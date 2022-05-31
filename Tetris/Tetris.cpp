#include<stdio.h>
#include <windows.h>
#include"CC212SGL.h"
#include<time.h>
#include <conio.h>
#include<Mmsystem.h>
#pragma comment(lib,"CC212SGL.lib")
#include<stdlib.h>

#define R  4 

//Global================

int field[288] = { 0 };
int map[288] = { 0 };
int next[64] = { 0 };
int row = 0, down = 0;

//updating the field & next=====================

void fupdate()
{
	for (int i = 0; i < 23; i++)
		for (int j = 0; j < 12; j++)
			field[i * 12 + j] = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			next[i * 8 + j] = 0;
}

//restart function======================

void maprestart()
{
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 12; j++)
			map[i * 12 + j] = 0;
}

//collision detection function=============

bool allow(int x, int y)
{
	for (int i = 1 + down; i < 5 + down; i++)
		for (int j = 5 + row; j < 9 + row; j++)
			if (field[i * 12 + j] != 0 && map[(i + y) * 12 + (j + x)] != 0)
				return false;
	return true;
}

//Rotation function=================

void rotate(int a[])
{
	for (int i = 0; i < R / 2; i++)
	{
		for (int j = i; j < R - i - 1; j++)
		{
			int t = a[i * 4 + j];
			a[i * 4 + j] = a[(R - 1 - j) * 4 + i];
			a[(R - 1 - j) * 4 + i] = a[(R - 1 - i) * 4 + (R - 1 - j)];
			a[(R - 1 - i) * 4 + (R - 1 - j)] = a[j * 4 + (R - 1 - i)];
			a[j * 4 + (R - 1 - i)] = t;
		}
	}
}


//updating map function========copy field into map==========

void copy()
{
	for (int i = 1 + down; i < 5 + down; i++)
		for (int j = 5 + row; j < 9 + row; j++)
			if (field[i * 12 + j] != 0)
				map[i * 12 + j] = field[i * 12 + j];
}

//===============loss function==================

bool loss()
{
	for (int j = 1; j < 11; j++)
		if (map[2 * 12 + j] != 0)
			return true;
	return false;
}

//================Win functions====================

void ShiftDown()
{
	for (int i = 22; i > 1; i--)
		for (int j = 1; j < 12; j++)
			if (map[i * 12 + j] == -1)
			{
				for (int start = i; start > 1; start--)
					map[start * 12 + j] = map[(start - 1) * 12 + j];
				i++;
			}
}
int  win()
{
	int count = 0;
	bool flag = false;
	for (int i = 22; i > 0; i--)
	{
		for (int j = 1; j < 12; j++)
		{
			if (map[i * 12 + j] == 0)
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			flag = false;
		}
		else
		{
			for (int k = 1; k < 12; k++)
				map[i * 12 + k] = -1;
			count++;
		}
	}
	ShiftDown();
	return count;
}



int main()
{
	//setup graphic library========
	CC212SGL g;
	g.setup(1);
	g.setFullScreenMode();
	g.hideCursor();

	//time variable============

	int time_start = 0;
	int time = 0;

	//setting axises===================================

	int default_x = (g.getWindowWidth() / 2) - 220, default_y = 100;    //start of axises of the game
	int x = default_x, y = default_y;
	int dxnext = (g.getWindowWidth() / 2) + 250, dynext = 220;          //start of axises of next tetromino
	int xnext = dxnext, ynext = dynext;

	//uploading images=================================

	int limit = g.loadImage("material\\limit.png");
	int tetro_1 = g.loadImage("material\\1.png");
	int tetro_2 = g.loadImage("material\\2.png");
	int tetro_3 = g.loadImage("material\\3.png");
	int tetro_4 = g.loadImage("material\\4.png");
	int tetro_5 = g.loadImage("material\\5.png");
	int tetro_6 = g.loadImage("material\\6.png");
	int tetro_7 = g.loadImage("material\\7.png");
	int piece[7] = { tetro_1,tetro_2,tetro_3,tetro_4,tetro_5,tetro_6,tetro_7 };


	//random piece=====
	int n, random = rand() % 7;

	//initializing tetrominoes================================

	int mino_1[16] = { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 };		//shape S
	int mino_2[16] = { 0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0 };		//shape I
	int mino_3[16] = { 0,3,0,0,0,3,0,0,0,3,3,0,0,0,0,0 };		//shape L
	int mino_4[16] = { 0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0 };		//shape O
	int mino_5[16] = { 0,0,5,0,0,5,5,0,0,0,5,0,0,0,0,0 };		//shape T
	int mino_6[16] = { 0,0,6,0,0,0,6,0,0,6,6,0,0,0,0,0 };		//shape J
	int mino_7[16] = { 0,0,0,0,7,7,0,0,0,7,7,0,0,0,0,0 };		//shape Z
	int* wpiece[7] = { mino_1,mino_2,mino_3,mino_4,mino_5 ,mino_6,mino_7 };
	int* tetro;
	int* ntetro;

	//flags========

	bool exist = true, update = false, pstop = false, select = true, stop = false, force = true, sloss = true, simpact = true;

	//declare displayed strings=======================
	int level = 1;
	char lev[50];
	int score = 0;
	int scount = 0;
	char sc[50];
	char controls[100] = "  CONTROLS\n\nW A S D - move\nR - rotate\nP - restart";
	char snext[20] = "Next";

	//Soundtrack===================
	mciSendString(L"open material\\soundtrack.mp3 type mpegvideo", NULL, 0, 0);
	mciSendString(L"play material\\soundtrack.mp3 repeat", NULL, 0, 0);


	while (1)//during the game 
	{
		time_start = clock();

		//game logic================= initialize ================================
		if (!stop)
		{
			if (select)
			{
				//Select random piece=========
				n = random;
				random = rand() % 7;
				select = false;
			}

			if (!pstop && exist)
			{
				//Puts the random in the field & updates its location================= 
				for (int w = 0; w < 7; w++)
				{
					if (random == w)
					{
						ntetro = wpiece[w];
						for (int i = 2; i < 6 + down; i++)
							for (int j = 2; j < 6; j++)
								next[i * 8 + j] = ntetro[(i - 2) * 4 + (j - 2)];
					}
					//puts the next piece===============
					if (n == w)
					{
						tetro = wpiece[w];
						for (int i = 1 + down; i < 5 + down; i++)
							for (int j = 5 + row; j < 9 + row; j++)
								field[i * 12 + j] = tetro[(i - 1 - down) * 4 + (j - 5 - row)];
					}
				}
			}
			else
			{
				//reset the location of the new piece==================
				pstop = false;
				down = 0;
				row = 0;
			}

			//set borders============for map & next=============

			for (int i = 0; i < 24; i++)
				for (int j = 0; j < 12; j++)
					if (i == 0 || i == 23 || j == 0 || j == 11)
						map[i * 12 + j] = 11;

			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					if (i == 0 || i == 7 || j == 0 || j == 7)
						next[i * 8 + j] = 11;

		}

		//===================================game rendering================================================

		g.beginDraw();

		//drawing the map and the field(the moving piece)=====================

		for (int i = 0; i < 24; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				x += 32;
				if (map[i * 12 + j] == 11)
					g.drawImage(limit, x, y, RGB(255, 255, 255));
				for (int k = 0; k < 7; k++)
					if (map[i * 12 + j] == k + 1)
						g.drawImage(piece[k], x, y, RGB(255, 255, 255));
					else if (field[i * 12 + j] == k + 1)
						g.drawImage(piece[k], x, y, RGB(255, 255, 255));
			}
			y += 32;
			x = default_x;//to start new line in x axis
		}
		y = default_y;//to start new line in y axis 

		//drawing the next piece=============================

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				xnext += 32;
				if (next[i * 8 + j] == 11)
					g.drawImage(limit, xnext, ynext, RGB(255, 255, 255));
				for (int k = 0; k < 7; k++)
					if (next[i * 8 + j] == k + 1)
						g.drawImage(piece[k], xnext, ynext, RGB(255, 255, 255));
			}
			ynext += 32;
			xnext = dxnext;//to start new line in x axis
		}
		ynext = dynext;//to start new line in y axis 

		//Draw score & Controls & level & loss========================

		g.setFontSizeAndBoldness(42, 700);
		g.setDrawingColor(YELLOW);
		sprintf(sc, "SCORE:\n   %d ", score);
		g.drawText(default_x - 250, default_y + 100, sc);
		g.drawText(dxnext + 50, dynext + 400, controls);
		g.drawText(dxnext + 120, dynext - 50, snext);
		sprintf(lev, "Level: %d", level);
		g.drawText(default_x - 250, default_y + 300, lev);
		if (stop)
		{
			g.setFontSizeAndBoldness(60, 700);
			g.setDrawingColor(WHITE);
			g.drawText(default_x + 70, default_y + 250, "Game Over!!\n  Continue?\n      (Y/N)");
		}

		g.endDraw();

		//game logic update============================================

		if (kbhit())
		{
			if (GetAsyncKeyState('A'))
			{
				if (allow(-1, 0))
				{
					row--;
					exist = true;
					update = true;
				}
			}
			else if (GetAsyncKeyState('D'))
			{
				if (allow(1, 0)) {
					row++;
					exist = true;
					update = true;
				}
			}
			else if (GetAsyncKeyState('R'))
			{
				for (int i = 0; i < 7; i++)
					if (n == i)
						rotate(wpiece[i]);
			}
			else if ((GetAsyncKeyState('S') && !stop) || force)
			{
				force = false;
				if (allow(0, 1))
				{
					down++;
					exist = true;
					update = true;
					force = false;
				}
				else
				{
					if (simpact)
					{
						mciSendString(L"close material\\impact.mp3", NULL, 0, 0);
						mciSendString(L"play material\\impact.mp3", NULL, 0, 0);
					}
					copy();// copy in the map before check for winning==========
					scount = win();
					if (scount > 0)
					{
						mciSendString(L"close material\\winning.mp3", NULL, 0, 0);
						mciSendString(L"play material\\winning.mp3", NULL, 0, 0);
						score += 100 * scount;
					}
					if (loss() && sloss)
					{
						mciSendString(L"close material\\soundtrack.mp3", NULL, 0, 0);
						mciSendString(L"play material\\losing.mp3", NULL, 0, 0);
						stop = true;
						exist = false;
						sloss = false;
						simpact = false;
					}
					pstop = true;
					select = true;
				}
			}
			else if (GetAsyncKeyState('Y') && stop)
			{
				mciSendString(L"play material\\soundtrack.mp3 repeat", NULL, 0, 0);
				mciSendString(L"close material\\losing.mp3", NULL, 0, 0);
				fupdate();
				maprestart();
				pstop = false;
				stop = false;
				score = 0;
				sloss = true;
				simpact = true;
				level = 1;
			}
			else if (GetAsyncKeyState('N') && stop)
			{
				break;
			}
			else if (GetAsyncKeyState('P'))
			{
				fupdate();
				maprestart();
				pstop = false;
				exist = false;
				score = 0;
				random = rand() % 7;
				level = 1;
			}
			if (update)
			{
				fupdate();
				update = false;
			}
		}


		//game timing=================================================

		while ((clock() / (float)CLOCKS_PER_SEC * 1000.0f) - (time_start / (float)CLOCKS_PER_SEC * 1000.0f) < 100);
		if (time > 10)
		{
			force = true;
			time = 0;
		}
		if (score >= 500 * level)
			level++;
		time += level;
	}
	return 0;
}