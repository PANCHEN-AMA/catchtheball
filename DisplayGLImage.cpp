#include "DisplayGLImage.h"
#include "DrawPrimitives.h"
#include "DetectCollision.h"
#include "glut.h"
#include <Windows.h>
#include <CoordinateConverter.h>

extern int flag = 0;
extern float pose[16] = { 0 };
extern float startime = 0;
extern float ballx = 0, bally = 0, ballz = 0;
void InitBall();
extern float starSpeed = 0.01;
float shadowFlag = 0;
int modelLevel = 1;
void modelTransfer();
/**
 * Initialize open GL window
 */
void initGL(){
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelZoom(1.0, -1.0);
    
    // enable and set colors
    glEnable( GL_COLOR_MATERIAL );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    
    // enable and set depth parameters
    glEnable( GL_DEPTH_TEST );
    glClearDepth( 1.0 );

	// enable texture
	glEnable(GL_TEXTURE_2D);
    
    // light parameters
    GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_dif[] = {0.7, 0.7, 0.7, 1.0};
    
    // enable lighting
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	InitBall();
}


/**
 * render window image with openGL
 * @param window displaying window
 * @param img_bgr background image from camera.
 */
void display(GLFWwindow* window, const cv::Mat& img_bgr, float poseMatrices[][16], int codes_count, int width_gl, int height_gl, int& starnum) {
	glfwGetFramebufferSize(window, &width_gl, &height_gl);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, width_gl, 0.0, height_gl);
	glRasterPos2i(0, height_gl - 1);
	glDrawPixels(width_gl, height_gl, GL_BGR_EXT, GL_UNSIGNED_BYTE, img_bgr.data);
	glPopMatrix();

	glLoadIdentity();
	gluPerspective(virtual_camera_angle, ((double)width_gl / (double)height_gl), 0.01, 100);


	glEnable(GL_DEPTH_TEST);

	/* rendering openGL ----------------------------------------- */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (poseMatrices[1] != NULL) {
		glPushMatrix();
		glLoadMatrixf(poseMatrices[1]);
		glScalef(0.05f, 0.05f, 0.05f);

		// Draw field
		drawField();
		glPopMatrix();
	}

	if (poseMatrices[0] != NULL) {
		glPushMatrix();
		glLoadMatrixf(poseMatrices[0]);
		glScalef(0.05f, 0.05f, 0.05f);

		// Draw cylinder
		glColor4f(0.0, 1.0, 1.0, 1.0);
		drawCylinder(0.5, 0.1, 2, 10);
		//DrawBall();
		glPopMatrix();
	}

	/* detect collision*/
	float ball[16] = { 1,0,0,0,
					   0,1,0,0,
					   0,0,1,0,
					   ballx,bally,ballz,1 };
	DetectCollision(poseMatrices, ball, starnum, flag, pose, startime);

	/* plot ball*/
	if (ballz > 0)
	{
		ballz = ballz - starSpeed;
	}
	else
	{
		InitBall();
	}

	float modelBall[16];
	Game2ModelMatrix(modelBall, ball, poseMatrices[1]);
	if (modelBall != NULL) {
		glPushMatrix();
		glLoadMatrixf(modelBall);
		glScalef(0.05f, 0.05f, 0.05f);

		// Draw cylinder
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
		//glColor4f(0.0, 1.0, 1.0, 1.0);
		//drawCylinder(0.5, 0.1, 2, 10);
		drawSphere(0.5, 40, 50);
		glPopMatrix();
	}

	/* detect whether transfer model*/
	modelTransfer();

	if (shadowFlag == 1)
	{
		float shadow[16];
		memcpy(shadow, ball, 16 * sizeof(float));
		shadow[14] = 0.0f;
		float modelshadow[16];
		Game2ModelMatrix(modelshadow, shadow, poseMatrices[1]);
		if (modelshadow != NULL)
		{
			glPushMatrix();
			{
				glLoadMatrixf(modelshadow);
				glScalef(0.05f, 0.05f, 0.05f);
				glColor3f(0.1f, 0.1f, 0.1f);
				drawCircle();
			}
			glPopMatrix();
		}
	}

	/* print shift to restart the game */
	CleanScreen(flag, startime, starnum, poseMatrices);
}

void InitBall()
{
	ballz = 1;
	ballx = 0.1+(rand() % 80) / (float)100;
	bally = 0.1+(rand() % 80) / (float)100;
}

void modelTransfer()
{
	if ((GetAsyncKeyState(VK_UP) & 0X8000)==0X8000)
	{
		if(modelLevel<2)
			modelLevel++;
	}
	if ((GetAsyncKeyState(VK_DOWN) & 0X8000) == 0X8000)
	{
		if (modelLevel > 0)
			modelLevel--;
	}
	switch (modelLevel)
	{
	// Easy
	case(0): {
		shadowFlag = 1;
		starSpeed = 0.01;
		break;
	}
	// Medium
	case(1): {
		shadowFlag = 0;
		starSpeed = 0.01;
		break;
	}
	// Difficult
	case(2): {
		shadowFlag = 0;
		starSpeed = 0.05;
		break;
	}
	}
}
