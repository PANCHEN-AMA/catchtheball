#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <math.h>
#include <cstdlib>


using namespace std;

#define stripeImageWidth 32
#define G 0.001

GLubyte stripeImage[4 * stripeImageWidth];

static GLuint texName;
bool direction = true;
double t = 0.003;

void init(void) {
	GLfloat light_position[] = { 0., 1.5, 1.5, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0); //设置背景色为蓝色
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}


void idle(void) //空闲函数
{
	glutPostRedisplay();
}

void display(void) {
	double time,v,y,x_set,z_set;
	v,time = 0;
	y = 0.5;
	x_set = (rand()%100)/(float)100;
	z_set = (rand() % 100) / (float)100;
	for (time = 0;time < 5;time = time + 0.002)
	{
		v = G * time;
		y = y - (v + 0.5*G*t*t);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			glPushMatrix();
			glTranslatef(x_set, y, z_set);
			GLfloat mat_ambient[] = { 0.021500, 0.174500, 0.021500, 0.550000 };
			GLfloat mat_diffuse[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
			GLfloat mat_specular[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
			GLfloat mat_shininess[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			glutSolidSphere(0.5, 40, 50);    //半径为 1,40 条纬线,50 条经线
			glPopMatrix();
			glFlush();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	Sleep(50);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w,
			1.5* (GLfloat)h / (GLfloat)w, -10.0, 10.0); //创建平行视景体
	else
		glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat)
			w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) { //控制键盘输入，键盘输入a、d、//s、w时分别实现左右前后的旋转，
	switch (key) { //输入e、r的时候实现放大和缩小
	case 'a':
		glRotatef(5.0, 0.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	case 'd':
		glRotatef(-5.0, 0.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	case 's':
		gluLookAt(0, 0.01, 0.01, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
		break;
	case 'w':
		gluLookAt(0, -0.01, 0.01, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
		break;
	case 'e':
		glScalef(1.1, 1.1, 1.1);
		glutPostRedisplay();
		break;
	case 'r':
		glScalef(0.9, 0.9, 0.9);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(450, 100);
	glutCreateWindow(argv[0]);
	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
