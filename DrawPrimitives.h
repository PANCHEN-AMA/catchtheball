
#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
//#include <GL/gl.h>

#include <math.h>


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

static void drawCube(const GLfloat material[])
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

void drawField() {
	glPushMatrix();

	// Side1
	glPushMatrix();
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glScaled(5.0, 0.2, 0.2);
	drawCube(aCube0Material);
	glPopMatrix();

	// Side2
	glPushMatrix();
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glRotated(90, 0.0, 0.0, 1.0);
	glScaled(5.0, 0.2, 0.2);
	drawCube(aCube0Material);
	glPopMatrix();

	// Side3
	glPushMatrix();
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glTranslated(5.0, 0.0, 0.0);
	glRotated(90, 0.0, 0.0, 1.0);
	glScaled(5.0, 0.2, 0.2);
	drawCube(aCube0Material);
	glPopMatrix();

	// Side4
	glPushMatrix();
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glTranslated(0.0, 5.0, 0.0);
	glScaled(5.0, 0.2, 0.2);
	drawCube(aCube0Material);
	glPopMatrix();

	// Bottom
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glScaled(5.0, 5.0, 0.1);
	glTranslated(0.0, 0.0, -1.0);
	drawCube(aCube1Material);
	glPopMatrix();

	glPopMatrix();

}