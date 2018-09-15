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
	font_style = f;
}

Text2D::Text2D()
{
	//setFontStyle(GLUT_BITMAP_TIMES_ROMAN_24);
	setFontStyle(GLUT_BITMAP_HELVETICA_12);
}

void Text2D::SetUp2D(GLfloat ww, GLfloat hh) // ww=width hh=height
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat)ww, 0.0, (GLfloat)hh);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRasterPos2f(200, 40);
	glPixelZoom(1, 1);
}
static GLfloat	eyePoint[3];

void Text2D::draw2DStuff(GLfloat eyePoint[3], GLfloat lookAtPoint[3], GLfloat upVec[3], bool CM,bool autoMove,double hsize)
{

	Text2D *drawText2d;
	glColor3f(1, 1, 0);
	glRasterPos2f(30, 60);
	drawText2d = new Text2D();
	int sl = 550;//startline
	int d = 20;

	drawText2d->drawstr(10, sl -= d, "%s %8.2f,%8.2f,%8.2f, ", "EyePoint   ", eyePoint[0], eyePoint[1], eyePoint[2]);
	drawText2d->drawstr(10, sl -= d, "%s %8.2f,%8.2f,%8.2f, ", "lookAtPoint", lookAtPoint[0], lookAtPoint[1], lookAtPoint[2]);
	drawText2d->drawstr(10, sl -= d, "%s %8.2f,%8.2f,%8.2f, ", "upVec      ", upVec[0], upVec[1], upVec[2]);
	drawText2d->drawstr(10, sl -= d, "%s %8.2f, ", "Size   ", hsize);

	drawText2d->drawstr(10, sl -= d, "%s", "0 - Control mode switch----" );
	if (CM)	drawText2d->drawstr(180, sl  , "%s", "Control Mode On   ");
	else	drawText2d->drawstr(180, sl  , "%s", "Control Mode Off  ");	

	//drawText2d->drawstr(10, sl -= d, "%s,%d", "0 - autodrive", autoMove);
	glColor3f(1, 1, 0);
	drawText2d->drawstr(10, sl -= 2 * d, "%s", "2 - 2D mode and help", 30);
	drawText2d->drawstr(10, sl -= d, "%s", "3 - 3D mode");
	drawText2d->drawstr(10, sl -= d, "%s", "5 - 2D & 3D mode");
	drawText2d->drawstr(10, sl -= d, "%s", "g - grid");
	drawText2d->drawstr(10, sl -= d, "%s", "a - axes");
	drawText2d->drawstr(10, sl -= d, "%s", "w - Wireframe");
	drawText2d->drawstr(10, sl -= d, "%s", "o - Space Base one");
	drawText2d->drawstr(10, sl -= d, "%s", "t - space Base two");
	drawText2d->drawstr(10, sl -= d, "%s", "l - Show simulated terrain");
	drawText2d->drawstr(10, sl -= d, "%s", "k - Turn on/off lights");
	drawText2d->drawstr(10, sl -= d, "%s", "F1 - This help");
	drawText2d->drawstr(10, sl -= d, "%s", "Arrows and pgup/pgdown comma and dot move eyepos");

	drawText2d->drawstr(10, sl -= d, "%s", "Space = start / stop spin");
	drawText2d->drawstr(10, sl -= d, "%s", "F2 - restore start view");

	drawText2d->drawstr(10, sl -= d, "%s", "d - depth buffer");
	drawText2d->drawstr(10, sl -= d, "%s", "c - cull hidden  @-swaps CW/CCW");
	drawText2d->drawstr(10, sl -= d, "%s", "/ - spin speed");
	drawText2d->drawstr(10, sl -= d, "%s", "+,- and = sets angle variable");
	drawText2d->drawstr(10, sl -= d, "%s", "[ and ] sets field of view");

}

// end