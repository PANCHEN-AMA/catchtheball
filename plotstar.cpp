#define GLFW_INCLUDE_GLU
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glut.h>
#include <glm/glm.hpp>

#include "DrawPrimitives.h"

/* program & OpenGL initialization */
void init(int argc, char *argv[])
{
	// enable and set colors
	glEnable( GL_COLOR_MATERIAL );
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	// enable and set depth parameters
	glEnable( GL_DEPTH_TEST );

	glClearDepth( 1.0 );
}

void display(GLFWwindow* window) {

	float ratio;
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;

	glViewport(0, 0, width, height);

	// clear buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	gluPerspective( 30, ((double)width/(double)height), 0.01, 100 );
	
	// move to origin
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// move the object backwards
	glTranslatef( 0.0, 0.0, -10.0 );

	// move the object in a fancy way
	const float t = (float) glfwGetTime() * 2.0f;
	const float n = 2.0f;
	//glTranslatef( 1.5f*sin(n*t), 1.5f*cos(n*t), 0.0f );
	glScalef(t*0.05f, t*0.05f, t*0.05f);
	float x = (0.2f + 0.2f * n * t) * cos(n * t);
	float y = (0.2f + 0.2f * n * t) * sin(n * t);
	glTranslatef(x, y, 0.0f);

	// rotate the object 
	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	

	//glColor3f(0.0, 0.0, 0.0);  -- > black
	//glColor3f(1.0, 0.0, 0.0);  -- > red
	//glColor3f(0.0, 1.0, 0.0);  -- > green
	//glColor3f(0.0, 0.0, 1.0);  -- > blue
	//glColor3f(1.0, 1.0, 0.0);  -- > yellow
	//glColor3f(1.0, 0.0, 1.0);  -- > pink
	//glColor3f(0.0, 1.0, 1.0);  -- > cyan-blue
	//glColor3f(1.0, 1.0, 1.0);  -- > white
	

	// draw a triangle and a red sphere
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, -0.382f, 0.f);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(0.951f, 0.309f, 0.f);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(-0.951f, 0.309f, 0.f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, -0.382f, 0.f);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(-0.588f, -0.809f, 0.f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, -0.382f, 0.f);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(0.588f, -0.809f, 0.f);
	glEnd();

	//glColor4f( 1.0, 0.0, 0.0, 1.0 );
	//drawSphere(0.5,10,10);
	//drawSnowman(1);


}

void reshape( GLFWwindow* window, int width, int height ) {

	// set a whole-window viewport
	glViewport( 0, 0, (GLsizei)width, (GLsizei)height );

	// create a perspective projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 30, ((GLfloat)width/(GLfloat)height), 0.01, 100 );

	// invalidate display
	//glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// initialize the window system
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1024, 768, "Exercise 6 - Open GL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	// Set callback functions for GLFW
	glfwSetFramebufferSizeCallback(window, reshape);

	glfwMakeContextCurrent(window);
	glfwSwapInterval( 1 );
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// initialize the GL library
	init(argc, argv);

	/* Loop until the user closes the window */
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		/* Render here */
		display(window);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

