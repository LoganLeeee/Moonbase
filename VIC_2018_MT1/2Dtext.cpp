// 2Dtext.cpp

#include "2Dtext.h"

/**
This routine displays a string on the glut screen in 2d Using a printf like arangement.
Warning 255 character maximum;
*/
void Text2D::drawstr(GLuint x, GLuint y, char* format, ...)
{
    va_list args;
    char buffer[255], *s;
    
	va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    glRasterPos2i(x, y);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(font_style, *s);
}

void Text2D::setFontStyle(void *f)
{
font_style=f;
}

Text2D::Text2D()
{
	//setFontStyle(GLUT_BITMAP_TIMES_ROMAN_24);
	setFontStyle(GLUT_BITMAP_HELVETICA_18);
}

void Text2D::SetUp2D(GLfloat ww, GLfloat hh) // ww=width hh=height
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLfloat)ww, 0.0, (GLfloat)hh);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glRasterPos2f(200,40);
  glPixelZoom(1,1);
}

// end


