#include "CoordinateConverter.h"
#include "DrawPrimitives.h"
#include "DisplayGLImage.h"
#include <math.h>
#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
#include <iostream>
#include <Windows.h>

//void DetectCollision(float poseMatrices[][16], float ball[16], int starnum)
void DetectCollision(float mark[][16], float ball[16], int &starnum, int &flag, float pose[16], float& startime)
{
	if (flag==0)
	{
		float gameMark[16];
		//float gameBall[16];
		Model2GameMatrix(gameMark, mark[0], mark[1]);
		//Model2GameMatrix(gameBall, mark[0], mark[1]);
		if (gameMark[12]>=ball[12]-0.2 && gameMark[12]<=ball[12]+0.2 && gameMark[13] >= ball[13]-0.2 && gameMark[13] <= ball[13]+0.2 &&
			gameMark[14]>=ball[14]-0.2 && gameMark[14]<=ball[14]+0.2)
		{
			startime = 0;
			flag = 1;
			memcpy(pose, mark[0], 16 * sizeof(float));
			//memcpy(pose, gameMark, 16 * sizeof(float));
			InitBall();
			//std::cout << ball[15] << std::endl;
		}
	}
	else
	{
		startime++;
		if (startime < 20)
		{
			//std::cout << startime << std::endl;
			glPushMatrix();
			{
				glLoadMatrixf(pose);
				glScalef(startime * 0.01f, startime * 0.01f, startime * 0.01f);
				float x = 0.05f * startime;
				float y = 0.05f * startime * startime;
				float z = -0.05f * startime;
				glTranslatef(x, y, 0.0f);
				glRotatef((float)startime * 100.f, 0.f, 0.f, 1.f);
				switch (starnum % 7)
				{
				case(1): glColor3f(1.0, 0.0, 0.0); break; //red
				case(2): glColor3f(0.0, 1.0, 0.0); break; //green
				case(3): glColor3f(0.0, 0.0, 1.0); break; //blue
				case(4): glColor3f(1.0, 1.0, 0.0); break; //yellow
				case(5): glColor3f(1.0, 0.0, 1.0); break; //pink
				case(6): glColor3f(0.0, 1.0, 1.0); break; //cyan-blue
				case(7): glColor3f(1.0, 1.0, 1.0); break; //white
				}
				drawstar();
			}
			glPopMatrix();
		}
		else
		{
			startime = 0;
			flag = 0;
			starnum++;
		}
	}	
}

void CleanScreen(int &flag, float &startime, int &starnum,float poseMatrices[][16])
{
	int a = GetKeyState(VK_SHIFT);
	if (a < 0)
	{
		flag = 0;
		startime = 0;
		starnum = 0;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 16; j++)
				poseMatrices[i][j] = NULL;
		initGL();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
