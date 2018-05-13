#include <iostream>
#include <cmath>
#include <ctime>
#include "Timer.h"
#include <string.h>
#include <gl\glut.h>

#define ORTHO_WIDTH 0

char g_strFPS[9] = "FPS:0000";
int g_ifps;
int g_icount = 0;
double g_ckStart = -1;
double g_felapse;
double g_fprev;

Timer g_Timer;

extern void onFrameMove(float delta);

/*
unsigned int g_iScore;
extern char g_strScore[] = "0000000";
*/

void DrawText(float x, float y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);  // GLUT_BITMAP_TIMES_ROMAN_24
	}
}

void DisplayFSP(int ifps)
{
	int i;
	
	glColor3f(1.0, 1.0, 1.0);
	i = ifps/1000;
	g_strFPS[4] = i+48;
	ifps = ifps - i*1000;
	i = ifps/100;
	g_strFPS[5] = i+48;
	ifps = ifps - i*100;
	g_strFPS[6] = ifps/10+48;
	g_strFPS[7] = ifps%10+48;
//	glColor3f(0.5, 0.8, 0.5);
	DrawText(0, ORTHO_WIDTH, g_strFPS);	// 在指定的位置處，顯示指定的字串內容
}

//void DisplayScore(unsigned int iScore)
//{
//	int i, j;
//	unsigned int t = 1000000;
//	for( j = 0 ; j < 7 ; j++ ) {
//		i = iScore/t;
//		g_strScore[j] = i+48;
//		iScore = iScore - i*t;
//		t = t / 10;
//	}
//	glColor3f(0.91, 0.15, 0.3);
//	DrawText(32, ORTHO_WIDTH-3, g_strScore);
//}


void IdleProcess()
{
	float delta;
	double ckNow; 
	g_icount++;
	if( g_ckStart == -1 ) {
		g_fprev = g_ckStart = g_Timer.getElapsedTimeInMilliSec(); //clock(); //開始計時
		onFrameMove(0);
	}
	else {
		ckNow = g_Timer.getElapsedTimeInMilliSec();
		g_felapse = (ckNow - g_ckStart);
		if( g_felapse >= 1000.0 ) {
			g_ifps = g_icount;
			g_icount = 0;
			g_ckStart += g_felapse;
		}
		delta = (float)((ckNow - g_fprev)/1000.0); // 計算間隔的時間
		g_fprev = ckNow; // 記錄這次的時間，當成前一次的時間
		onFrameMove(delta);
//		printf("%d\n",g_ifps);
	}
//	glutPostRedisplay(); // 呼叫 Rendering 更新螢幕
}