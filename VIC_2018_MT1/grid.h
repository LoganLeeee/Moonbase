#ifndef __grid_ZZ__
#define __grid_ZZ__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

class Grid 
{
protected:

GLfloat xsize;
GLfloat zsize;
int xsteps;
int zsteps;

public:


void setUp(GLfloat xsizeZ, GLfloat zsizeZ, int xstepsZ, int zstepsZ);

int render();
int renderCW();
int renderLines(); // 
Grid();
~Grid();

};


#endif
