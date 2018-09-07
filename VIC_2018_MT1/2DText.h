#ifndef __2DtextZZ__
#define __2DtextZZ__
//2Dtext.h

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "glut.h"

#pragma warning(disable : 4996) // Reduce unsafe warning messages

/**

Bitmap font constants (use these in GLUT program).
GLUT_BITMAP_9_BY_15		
GLUT_BITMAP_8_BY_13		
GLUT_BITMAP_TIMES_ROMAN_10	
GLUT_BITMAP_TIMES_ROMAN_24	
GLUT_BITMAP_HELVETICA_10	
GLUT_BITMAP_HELVETICA_12	
GLUT_BITMAP_HELVETICA_18

Typical usage (after its created)
  glMatrixMode(GL_PROJECTION);
  glPushMatrix(); // NOTE : This preserves the Projection matrix for future use 
  glLoadIdentity();
  gluOrtho2D(0.0, (GLfloat)width, 0.0, (GLfloat)height);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity(); // load 'no' transformation so the text works 

  glColor3f(1, 1, 1);
  text2D->setFontStyle(GLUT_BITMAP_HELVETICA_12);
  text2D->drawstr(10,30,"Test String %d for Text2D Class",2);
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();  // NOTE : This restores the Projection matrix  
  glMatrixMode(GL_MODELVIEW); // if needed 
*/

class Text2D 
{
protected:
	
GLvoid *font_style;

public:

	void drawstr(GLuint x, GLuint y, char* format, ...);
    void setFontStyle(void *f);
	void SetUp2D(GLfloat ww, GLfloat hh);
	Text2D();
};


#endif
