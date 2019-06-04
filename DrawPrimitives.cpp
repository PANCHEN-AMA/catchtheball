#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
//#include <GL/gl.h>

#include <math.h>
#include <cstdlib>
#include <Windows.h>
#include <glut.h>


/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

void drawSphere(double r, int lats, int longs) {
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r *  cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}


void solidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	glBegin(GL_LINE_LOOP);
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);

	gluCylinder(quadric, base, 0, height, slices, stacks);

	gluDeleteQuadric(quadric);
	glEnd();
}

// Added in Exercise 9 - Start *****************************************************************
void drawSnowman( bool female )
{
	glRotatef( -90, 1, 0, 0 );
	glScalef(0.03, 0.03, 0.03);

	// draw 3 white spheres
	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	drawSphere( 0.8, 10, 10 );
	glTranslatef( 0.0, 0.8, 0.0 );
	drawSphere( 0.6, 10, 10 );
	if(female)
	{
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-0.2, 0.05, 0.3);
		drawSphere( 0.32, 10, 10 );
		glTranslatef(0.4, 0, 0);
		drawSphere( 0.32, 10, 10 );
		glPopMatrix();
	}
	glTranslatef( 0.0, 0.6, 0.0 );
	drawSphere( 0.4, 10, 10 );

	// draw the eyes
	glPushMatrix();
	glColor4f( 0.0, 0.0, 0.0, 1.0 );
	glTranslatef( 0.2, 0.2, 0.2 );
	drawSphere( 0.066, 10, 10 );
	glTranslatef( 0, 0, -0.4 );
	drawSphere( 0.066, 10, 10 );
	glPopMatrix();

	// draw a nose
	glColor4f( 1.0, 0.5, 0.0, 1.0 );
	glTranslatef( 0.3, 0.0, 0.0 );
	glRotatef( 90, 0, 1, 0 );
	solidCone( 0.1, 0.3, 10, 10 );
}
// Added in Exercise 9 - End *****************************************************************
// Cube Vertex
static const GLdouble aCubeVertex[][3] = {
	{ 0.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 }
};
// Cube Face
static const int aCubeFace[][4] = {
	{ 0, 1, 2, 3 },
	{ 1, 5, 6, 2 },
	{ 5, 4, 7, 6 },
	{ 4, 0, 3, 7 },
	{ 4, 5, 1, 0 },
	{ 3, 2, 6, 7 }
};
// Cube Normal
static const GLdouble aCubeNormal[][3] = {
  { 0.0, 0.0,-1.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  {-1.0, 0.0, 0.0 },
  { 0.0,-1.0, 0.0 },
  { 0.0, 1.0, 0.0 }
};
//Cube Material
static const GLfloat aCube0Material[] = { 0.8, 0.2, 0.2, 1.0 };
static const GLfloat aCube1Material[] = { 0.2, 0.2, 0.8, 1.0 };
void drawCube(const GLfloat material[])
{
	// param
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);

	glBegin(GL_QUADS);
	for (size_t i = 0; i < 6; ++i)
	{
		glNormal3dv(aCubeNormal[i]);
		for (size_t j = 0; j < 4; ++j)
		{
			glVertex3dv(aCubeVertex[aCubeFace[i][j]]);
		}
	}
	glEnd();
}

GLuint texture[2];

GLuint LoadTextureRAW(const char * filename)
{
	GLuint texture;
	int width, height;
	unsigned char * data;
	FILE * file;
	fopen_s(&file, filename, "rb");
	if (file == NULL) return 0;
	width = 1024;
	height = 512;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void DisplayWall(GLuint texture) {

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	for (int i = 0; i < 12; i++)
	{
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(i*0.4, 0.0, 0.2);
		glTexCoord2f(2.0, 0.0); glVertex3f((i+1)*0.4, 0.0, 0.2);
		glTexCoord2f(2.0, 1.0); glVertex3f((i+1)*0.4, 0.2, 0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(i*0.4, 0.2, 0.2);
	}
	glNormal3d(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(4.8, 0.0, 0.2);
	glTexCoord2f(1.0, 0.0); glVertex3f(5.0, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 0.2, 0.2);
	glTexCoord2f(0.0, 1.0); glVertex3f(4.8, 0.2, 0.2);
	
	for (int i = 0; i < 12; i++)
	{
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2f(2.0, 0.0); glVertex3f(i*0.4, 0.0, 0.0);
		glTexCoord2f(2.0, 1.0); glVertex3f(i*0.4, 0.2, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f((i + 1)*0.4, 0.2, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f((i + 1)*0.4, 0.0, 0.0);
	}
	glNormal3d(0.0, 0.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(4.8, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(4.8, 0.2, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(5.0, 0.2, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(5.0, 0.0, 0.0);

	for (int i = 0; i < 12; i++)
	{
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2f(2.0, 0.0); glVertex3f(i*0.4, 0.0, 0.0);
		glTexCoord2f(2.0, 1.0); glVertex3f(i*0.4, 0.2, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f((i + 1)*0.4, 0.2, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f((i + 1)*0.4, 0.0, 0.0);
	}
	glNormal3d(0.0, 0.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(4.8, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(4.8, 0.2, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(5.0, 0.2, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(5.0, 0.0, 0.0);

	for (int i = 0; i < 12; i++)
	{
		glNormal3d(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(i*0.4, 0.2, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(i*0.4, 0.2, 0.2);
		glTexCoord2f(2.0, 0.0); glVertex3f((i + 1)*0.4, 0.2, 0.2);
		glTexCoord2f(2.0, 1.0); glVertex3f((i + 1)*0.4, 0.2, 0.0);
	}
	glNormal3d(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(4.8, 0.2, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(4.8, 0.2, 0.2);
	glTexCoord2f(1.0, 0.0); glVertex3f(5.0, 0.2, 0.2);
	glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 0.2, 0.0);

	for (int i = 0; i < 12; i++)
	{
		glNormal3d(0.0, -1.0, 0.0);
		glTexCoord2f(2.0, 1.0); glVertex3f(i*0.4, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f((i + 1)*0.4, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f((i + 1)*0.4, 0.0, 0.2);
		glTexCoord2f(2.0, 0.0); glVertex3f(i*0.4, 0.0, 0.2);
	}
	glNormal3d(0.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(4.8, 0.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(5.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(5.0, 0.0, 0.2);
	glTexCoord2f(1.0, 0.0); glVertex3f(4.8, 0.0, 0.2);

	glNormal3d(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(5.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 0.2, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(5.0, 0.2, 0.2);
	glTexCoord2f(0.0, 0.0); glVertex3f(5.0, 0.0, 0.2);

	glNormal3d(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.2, 0.2);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.2, 0.0);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DisplayBottom(GLuint texture) {

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glNormal3d(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(5.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 5.0, 0.0);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawField() {
	texture[0] = LoadTextureRAW("bark.raw");
	texture[1] = LoadTextureRAW("aerial.raw");

	glPushMatrix();

	// Side1
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	DisplayWall(texture[0]);
	glPopMatrix();

	// Side2
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glRotated(90, 0.0, 0.0, 1.0);
	DisplayWall(texture[0]);
	glPopMatrix();

	// Side3
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslated(5.0, 0.0, 0.0);
	glRotated(90, 0.0, 0.0, 1.0);
	DisplayWall(texture[0]);
	glPopMatrix();

	// Side4
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslated(0.0, 4.8, 0.0);
	DisplayWall(texture[0]);
	glPopMatrix();

	// Bottom
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	DisplayBottom(texture[1]);
	glPopMatrix();

	glPopMatrix();

}

void drawCylinder(double rad, double len, int axis, int slices)
{
	GLUquadricObj *qobj;
	qobj = gluNewQuadric();

	glPushMatrix();
	switch (axis) {
	case 0:
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -0.5*len);
		break;
	case 1:
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glTranslatef(0.0, 0.0, -0.5*len);
		break;
	case 2:
		glTranslatef(0.0, 0.0, -0.5*len);
		break;
	default:
		glTranslatef(0.0, 0.0, -0.5*len);
	}

	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluCylinder(qobj, rad, rad, len, slices, slices);

	glPushMatrix();
	glRotatef(180.0, 1.0, 0.0, 0.0);
	gluDisk(qobj, 0.0, rad, slices, slices);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, len);
	gluDisk(qobj, 0.0, rad, slices, slices);
	glPopMatrix();

	glPopMatrix();
}

void drawstar(void)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, -0.382f, 0.f);
	glVertex3f(0.951f, 0.309f, 0.f);
	glVertex3f(-0.951f, 0.309f, 0.f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -0.382f, 0.f);
	glVertex3f(-0.588f, -0.809f, 0.f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -0.382f, 0.f);
	glVertex3f(0.588f, -0.809f, 0.f);
	glEnd();
}

const int n = 100;
const GLfloat R = 0.5f;
const GLfloat pi = 3.1415926536f;

void drawCircle()
{
	int i = 0;
	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++)
	{
		glVertex2f(R * cos(2 * pi / n * i), R * sin(2 * pi / n * i));
	}
	glEnd();
}

