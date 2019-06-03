#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
//#include <GL/gl.h>

#include <math.h>

void drawSphere(double r, int lats, int longs);
void solidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
void drawSnowman(bool female);
void drawCube(const GLfloat material[]);
void drawField();
void drawCylinder(double rad, double len, int axis, int slices);
void drawstar(void);
void drawCircle();

