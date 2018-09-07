/****	First 3D program : the spinning cube
	Written by Rob Cox and Hugh Fisher				****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "Axes.h"
#include "CubeClass.h"
#include "Dome.h"
#include "2DText.h"
#include "grid.h"

#pragma warning(disable : 4996) // Reduce unsafe warning messages

#define framesPerSecond 25
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame = false;

#define ESC	  27

#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99

static int	WinWidth, WinHeight;
static int	AppMenu;

static GLfloat	spinAngle;
GLfloat spinIncrement = 0.4f; // slow

static GLfloat	eyePoint[3];
static GLfloat	lookAtPoint[3];
static GLfloat	upVec[3];//
static GLfloat  fov; // field of view
static GLfloat	nearPlane;
static GLfloat	farPlane;

static GLfloat	color3[3];

static GLfloat	red[] = { 1.0, 0.0, 0.0 };
static GLfloat	blue[] = { 0.0, 1.0, 0.0 };
static GLfloat	green[] = { 0.0, 0.0, 1.0 };
static GLfloat	cyan[] = { 0.0, 1.0, 1.0 };
static GLfloat	magenta[] = { 1.0, 0.0, 1.0 };
static GLfloat	yellow[] = { 1.0, 1.0, 0.0 };
static GLfloat	black[] = { 0.0, 0.0, 0.0 };
static GLfloat	white[] = { 1.0, 1.0, 1.0 };

CubeClass *cube; // pointer to a cube object
GLUquadricObj *quadric1;
GLUquadricObj *quadric2;
GLUquadricObj *quadric3;
GLUquadricObj *quadric4;
Axes *axes; //
Grid *grid; // To help with push and scal etc
DomeClass *dome0;
DomeClass *dome1;
DomeClass *dome2;

int showGrid = true;
int showAxes = true;
int showBase1 = true;
int showBase2 = false;
int showWireFrame = true;
int showSpin = false;
int showChrisTrapani = false; // change center of spin

int hiddenDepth = false;
int hiddenCull = false;
int clockWise = false; // start CCW
GLfloat angle = 0;
GLfloat angleVar = 0;

GLfloat centerX = 10;
GLfloat centerZ = 10;

int mode = 2; // mode 2 = 2D, 3=3D , 5=Both
Text2D *drawText2d;
char dispString[255] = "2=2D (also F1-help)  3=3D  5=2D+3D";

// Function prototypes needed because they are used before its declared

bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

void CheckGL()
{
	int  err;
	char msg[256];

	err = glGetError();
	if (err != GL_NO_ERROR) {
		sprintf(msg, "OpenGL error: %s", (GLubyte*)gluErrorString(err));
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

/****		Window events		****/

static void seteyePoint()
{
	glLoadIdentity();
	gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2],
		lookAtPoint[0], lookAtPoint[1], lookAtPoint[2],
		upVec[0], upVec[1], upVec[2]);
}

static void setProjection3D()
{
	GLfloat aspect;
	/* Set projection Mode */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspect = (float)WinWidth / (float)WinHeight;
	gluPerspective(fov, aspect, nearPlane, farPlane);
	/* Back to normal */
	glMatrixMode(GL_MODELVIEW);
	seteyePoint();

	if (hiddenDepth)
	{
		// Now do z buffer
		glEnable(GL_DEPTH_TEST);
		glDepthRange(nearPlane, farPlane);
		glClearDepth(farPlane);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	if (hiddenCull)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		if (clockWise) glFrontFace(GL_CW);
		else glFrontFace(GL_CCW);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

static void setProjection2D()
{
	//GLfloat aspect;
	/* Set projection Mode */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//aspect = (float)WinWidth / (float)WinHeight;
	glOrtho(0, WinWidth, 0, WinHeight, -1.0f, 1.0f);
	/* Back to normal but identity*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void drawCube()
{
	glPushMatrix();
	cube->render();
	glPopMatrix();
}

void drawCylinder()
{
	glPushMatrix();
	gluCylinder(quadric1, 0.5, 0.5, 1, 17, 4);
	glPopMatrix();
}

void drawDisk(GLfloat innerRadius) // inner raduis 0 = full disk 0.5 = no disk or circle 圆环
{
	glPushMatrix();
	gluDisk(quadric3, innerRadius, 0.5, 17, 4);
	glPopMatrix();
}

void drawCone(GLfloat smallEndRadius) //  raduis 0 = Cone  0.5 = Cylinder  圆锥
{
	glPushMatrix();
	glTranslatef(0, 0, -0.5);
	gluCylinder(quadric4, 0.5, smallEndRadius, 1, 17, 4);
	glPopMatrix();
}

void drawSphere() //球
{
	glPushMatrix();
	gluSphere(quadric2, 0.5, 13, 13);
	glPopMatrix();
}

void drawDome()
{
	//int i;
	glPushMatrix();

	glPopMatrix();
}

void drawConector(int conectorlong = 4)
{
	//int conectorlong = 4;

	glPushMatrix();						 	//Conector tunnel
	//glColor3f(0.6, 0.5, 1);
	/*glTranslatef(0, 0, 0);*/
	glScalef(1, 1, conectorlong);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();							//Conector door
	glColor3f(1, 0.5, 0.4);
	drawDisk(0.0);
	glTranslatef(0, 0, conectorlong);
	drawDisk(0.0);
	glPopMatrix();
}

void drawDomeAndConector(GLfloat angle)
{
	glPushMatrix();

	glColor3f(0.6, 0.5, 0.4);

	glTranslatef(0, 1, 0);
	glRotatef(angle, 0, 1, 0);
	glRotatef(180, 1, 0, 0);
	dome0->render();
	glTranslatef(0, 1, -5.5);
	glColor3f(0.6, 0.5, 1);
	drawConector();
	glPopMatrix();
}
void drawBigDome()			//组合大碉堡
{
	//int i;
	glPushMatrix();	
	drawDomeAndConector(90);//小

	glTranslatef(9, 3, 0);
	glRotatef(180, 1, 0, 0);
	glColor3f(0.4, 0.4, 0.4);
	dome1->render();		//大
	glPopMatrix();
}

void drawBase1()
{
	glScalef(.5, .5, .5);
	glPushMatrix();
	glTranslatef(3, 0, 10);
	drawDomeAndConector(0);//小2

	glTranslatef(0, 0, -7);
	drawDomeAndConector(0);//小1

	glPopMatrix();
	glTranslatef(15, 0, 20);
	drawBigDome();
	double d = 10;

	glTranslatef(0, 0, -d);

	drawBigDome();
	glTranslatef(9, 0,  3);
	glColor3f(0.0, 1, 0.0);
	drawConector(d - 6);
	glPushMatrix();

	glPopMatrix();

	//glPushMatrix();
	//glColor3f(0.6, 0.5, 0.4);
	//glTranslatef(2, 10, 10);
	//glScalef(2, 2, 2);
	////drawSphere();
	////drawCube();
	//drawCylinder();
	//drawDisk(0.2);

	//glPopMatrix();
}

void drawBase2()
{
	//glPushMatrix();
	//glColor3f(0.6, 0.5, 0.4);
	//glTranslatef(2, 1, 10);
	//glRotatef(180, 1, 0, 0);
	//dome0->render();
	//glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);

	glTranslatef(10, 3, 10);
	glRotatef(180, 1, 0, 0);
	dome1->render();
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(5, 1, 0);
	//glRotatef(180, 1, 0, 0);
	//glColor3f(0.3, 0.3, 0.8);
	//dome2->render();
	//glPopMatrix();
}

static void drawWorld()
{
	if (showWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}
	glPushMatrix();

	if (showSpin)
	{
		glPushMatrix();
		glTranslatef(centerX, 0, centerZ); // move origin
		glRotatef(spinAngle, 0, 1, 0);
		glTranslatef(-centerX, 0, -centerZ); // move origin back
	}
	if (showGrid)
	{
		glColor3f(0.5, 0.5, 0);
		grid->render();
	}

	if (showAxes)
	{
		axes->render();
	}

	//glColor3fv(color3);

	if (showBase1)
	{
		drawBase1();
	}
	if (showBase2)
	{
		drawBase2();
	}
	if (showSpin) glPopMatrix();

	glPopMatrix();
}

void draw2DStuff()
{
	glColor3f(1, 1, 0);
	glRasterPos2f(30, 60);

	//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	//drawText2d->drawstr(30, 30, "%s (%d)",dispString,45);

	drawText2d->drawstr(10, 530, "%s %8.2f,%8.2f,%8.2f, ", "EyePoint", eyePoint[0], eyePoint[1], eyePoint[2]);

	drawText2d->drawstr(10, 500, "%s", "2 - 2D mode and help");
	drawText2d->drawstr(10, 470, "%s", "3 - 3D mode");
	drawText2d->drawstr(10, 440, "%s", "5 - 2D & 3D mode");
	drawText2d->drawstr(10, 410, "%s", "g - grid");
	drawText2d->drawstr(10, 380, "%s", "a - axes");
	drawText2d->drawstr(10, 350, "%s", "w - Wireframe");
	drawText2d->drawstr(10, 320, "%s", "o - Space Base one");
	drawText2d->drawstr(10, 290, "%s", "t - space Base two");
	drawText2d->drawstr(10, 260, "%s", "F1 - This help");
	drawText2d->drawstr(10, 230, "%s", "Arrows and pgup/pgdown comma and dot move eyepos");
	drawText2d->drawstr(10, 200, "%s", "Space = start / stop spin");
	drawText2d->drawstr(10, 170, "%s", "F2 - restore start view");

	drawText2d->drawstr(10, 150, "%s", "d - depth buffer");
	drawText2d->drawstr(10, 130, "%s", "c - cull hidden  @-swaps CW/CCW");
	drawText2d->drawstr(10, 110, "%s", "/ - spin speed");
	drawText2d->drawstr(10, 90, "%s", "+,- and = sets angle variable");
}

static void display()
{
	if (hiddenDepth) { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	else { glClear(GL_COLOR_BUFFER_BIT); }

	if (mode == 3 || mode == 5)
	{
		setProjection3D();
		seteyePoint();
		/****       mouse control      ****/
		glRotatef(xrot, 1.0f, 0.0f, 0.0f);
		glRotatef(yrot, 0.0f, 1.0f, 0.0f);
		/****       mouse control      ****/
		drawWorld();
	}

	if (mode == 2 || mode == 5)
	{
		setProjection2D();
		draw2DStuff();
	}
	/* Check everything OK and update screen */
	CheckGL();
	glutSwapBuffers();
}

static void resize(int width, int height)
{
	/* Save for event handlers */
	WinWidth = width;
	WinHeight = height;

	/* Reset view in window. */
	glViewport(0, 0, WinWidth, WinHeight);
}

/****		User events		****/

static void menuChoice(int item)
{
	switch (item) {
	case cmdRed:
		color3[0] = 1; // R
		color3[1] = 0; // G
		color3[2] = 0; // B
		break;
	case cmdGreen:
		color3[0] = 0; // R
		color3[1] = 1; // G
		color3[2] = 0; // B
		break;
	case cmdExit:
		exit(0);
		break;
	default:
		break;
	}
}

/* In most GUI systems we would write just one event handler
   for all kinds of keystrokes. In GLUT, we need one for the
   standard ASCII keys and one for special cursor or function
   style keys that vary from system to system. Because the
   GLUT special key code range overlaps with ASCII lowercase,
   it isn't safe to use the same function for both.        */

static void asciiKey(unsigned char key, int x, int y)
{
	if (key == ESC)
		menuChoice(cmdExit);

	if (key == '2') { mode = 2; strcpy(dispString, "2D Mode"); }
	if (key == '3') { mode = 3; strcpy(dispString, "3D Mode"); }
	if (key == '5') { mode = 5; strcpy(dispString, "2D+3D Mode"); }
	if (key == 'g') { showGrid = !showGrid; }
	if (key == 'a') { showAxes = !showAxes; }
	if (key == 'o') { showBase1 = !showBase1; }
	if (key == 't') { showBase2 = !showBase2; }
	if (key == 'w') { showWireFrame = !showWireFrame; }
	if (key == ' ') { showSpin = !showSpin; }

	if (key == 'd') { hiddenDepth = !hiddenDepth; }
	if (key == 'c') { hiddenCull = !hiddenCull; }
	if (key == '@') { clockWise = !clockWise; }
	if (key == '/') { spinIncrement = spinIncrement + 1; if (spinIncrement > 4) { spinIncrement = 0.4f; } }
	//if (key =='j') {showChrisTrapani=!showChrisTrapani;}

	if (key == '=') { if (angle < 50) angle = 90; else angle = 0; }
	if (key == '+') { angle = angle + 10; angle = (GLfloat)((int)angle % 360); }
	if (key == '-') { angle = angle - 10; angle = (GLfloat)((int)angle % 360); }

	if (key == '.') { eyePoint[0] = eyePoint[0] + 2; eyePoint[2] = eyePoint[2] + 2; }
	if (key == ',') { eyePoint[0] = eyePoint[0] - 2; eyePoint[2] = eyePoint[2] - 2; }
}

void setStartValues()
{
	eyePoint[0] = 12.0;
	eyePoint[1] = 12.0;
	eyePoint[2] = 26.0;

	lookAtPoint[0] = 0;
	lookAtPoint[1] = 0;
	lookAtPoint[2] = 0;

	upVec[0] = 0;
	upVec[1] = 1;
	upVec[2] = 0;

	fov = 60; // field of view
	nearPlane = 0.5;
	farPlane = 90;

	spinAngle = 0.0;

	xrot = 0.0f;
	yrot = 0.0f;
}

static void specialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_F1) { mode = 2; }

	if (key == GLUT_KEY_F2)
	{
		setStartValues();
	}

	if (key == GLUT_KEY_LEFT) eyePoint[0] = eyePoint[0] + 2;
	if (key == GLUT_KEY_RIGHT) eyePoint[0] = eyePoint[0] - 2;
	if (key == GLUT_KEY_UP) eyePoint[2] = eyePoint[2] - 2;
	if (key == GLUT_KEY_DOWN) eyePoint[2] = eyePoint[2] + 2;
	if (key == GLUT_KEY_PAGE_UP) eyePoint[1] = eyePoint[1] + 2;
	if (key == GLUT_KEY_PAGE_DOWN) eyePoint[1] = eyePoint[1] - 2;
}

void exitFunction(void)
{
	delete cube;
	delete axes;
	delete drawText2d;
	delete grid;
	delete dome0;
	delete dome1;
	delete dome2;

	gluDeleteQuadric(quadric1);
	gluDeleteQuadric(quadric2);
	gluDeleteQuadric(quadric3);
	gluDeleteQuadric(quadric4);
	//  gluDeleteQuadric(quadric2);
}

/****       mouse control      ****/

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;

		//xdiff = x - eyePoint[0];
		xdiff = x - yrot;
		//ydiff = -y + eyePoint[1];
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		xrot = y + ydiff;
		yrot = x - xdiff;
		//eyePoint[1] = y +  ydiff;
		//eyePoint[0] = x - xdiff;
		//eyePoint[2] = x - xdiff;

		//glutPostRedisplay();
	}
}

/****		Startup			****/

static void initGraphics(void)
{
	/* Black background */
	glClearColor(0, 0, 0, 0);

	/* colour green */
	color3[0] = 0;
	color3[0] = 1;
	color3[0] = 0;

	glEnable(GL_DEPTH_TEST);

	/* Needed for vertex arrays */
	//glEnableClientState(GL_VERTEX_ARRAY);

	/* Popup menu attached to right mouse button */
	AppMenu = glutCreateMenu(menuChoice);
	glutSetMenu(AppMenu);
	glutAddMenuEntry("Red", cmdRed);
	glutAddMenuEntry("Green", cmdGreen);
	glutAddMenuEntry("----", 0);
	glutAddMenuEntry("Exit", cmdExit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Start values */
	setStartValues();

	menuChoice(cmdGreen);

	cube = new CubeClass();
	dome0 = new DomeClass();
	dome1 = new DomeClass(19, 10, 9, 4);
	dome2 = new DomeClass(17, 12, 2, 5);
	dome2->startAngle = 10;
	axes = new Axes();
	drawText2d = new Text2D();
	grid = new Grid();
	grid->setUp(20, 20, 10, 10);
	quadric1 = gluNewQuadric();
	quadric2 = gluNewQuadric();
	quadric3 = gluNewQuadric();
	quadric4 = gluNewQuadric();

	atexit(exitFunction);
}

void timer(int value)
{
	timeForNextFrame = true;
	glutTimerFunc(timerMSecs, timer, 0);
}

/****		Main control		****/

static void idleRoutine(void)
{
	if (timeForNextFrame)
	{
		spinAngle += spinIncrement;
		if (spinAngle > 360.0)
			spinAngle -= 360.0;
		glutPostRedisplay();
		timeForNextFrame = false;

		if (angleVar < angle)
		{
			angleVar = angleVar + 1;
			if (angleVar > angle) angleVar = angle;
		}

		if (angleVar > angle)
		{
			angleVar = angleVar - 1;
			if (angleVar < angle) angleVar = angle;
		}
	}
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	glutInitWindowSize(550, 550);
	glutInitWindowPosition(100, 75);
	glutCreateWindow("MoonBase");

	initGraphics();

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutKeyboardFunc(asciiKey);
	glutSpecialFunc(specialKey);

	glutIdleFunc(idleRoutine);
	glutTimerFunc(timerMSecs, timer, 0);

	glutMainLoop();
	/* Should never get here, but keeps compiler happy */
	return 0;
}