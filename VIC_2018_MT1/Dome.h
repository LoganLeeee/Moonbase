
#ifndef dome_def 
#define dome_def 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

class DomeClass
{
public:
	GLfloat x0, y0, z0;
	GLfloat x1, y1, z1;
	GLfloat x2, y2, z2;
	GLfloat x3, y3, z3;

	int slices=13;
	int stacks=4;
	GLfloat startAngle = 0;
	GLfloat delta=10; // angle in degrees
	GLfloat radius=3;
	bool debug = false;

	DomeClass(int slices, int stacks, GLfloat delta, GLfloat radius); // constructor
	DomeClass(); // constructor
	DomeClass(DomeClass *d); // copy constructor
	void render(); // draw it
	void setdebug(); // print co-ords to the console

};

// useful 2d rotate function
void Move2D(GLfloat oldx, GLfloat oldz,
	GLfloat angle, GLfloat distance, // angle in degrees
	GLfloat *resultx, GLfloat *resultz);

#endif

