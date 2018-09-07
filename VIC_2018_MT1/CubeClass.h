#ifndef cubeclass_def 
#define cubeclass_def 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>


class CubeClass
{
public:
	CubeClass(); // constructor
	void render(); // draw it
	void renderCW(); // draw it CW
	void renderCCW(); // draw it CCW

};

#endif
