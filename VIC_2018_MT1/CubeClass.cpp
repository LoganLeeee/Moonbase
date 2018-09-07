/****		Cube in points-polygons (polyhedron) form	****/

#include "CubeClass.h"

static GLfloat Verts[8][3] = {
  { -0.5,  0.5, -0.5 }, /* 0 left top rear */
  {  0.5,  0.5, -0.5 },	/* 1 right top rear */
  {  0.5, -0.5, -0.5 },	/* 2 right bottom rear */
  { -0.5, -0.5, -0.5 },	/* 3 left bottom rear */
  { -0.5,  0.5,  0.5 },	/* 4 left top front */
  {  0.5,  0.5,  0.5 },	/* 5 right top front */
  {  0.5, -0.5,  0.5 },	/* 6 right bottom front */
  { -0.5, -0.5,  0.5 }	/* 7 left bottom front */
};

static GLuint Faces[6][4] = {
  4, 5, 6, 7,	/* front */
  5, 1, 2, 6,	/* right */
  0, 4, 7, 3,	/* left */
  4, 0, 1, 5,	/* top */
  7, 6, 2, 3,	/* bottom */
  1, 0, 3, 2	/* rear */
};

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  glColor3f(0.0, 1.0, 1.0);
//  glLineWidth(3);


CubeClass::CubeClass()
{
}

void CubeClass::renderCW()
{
  
  /* Draw cube in traditional OpenGL style */
  glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++) 
		{
		for (int vert = 0; vert < 4; vert ++)
			glVertex3fv(Verts[Faces[face][vert]]);
		}

  glEnd();
}

void CubeClass::renderCCW()
{
  
  /* Draw cube in traditional OpenGL style */
  glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++) 
		{
		for (int vert = 3; vert >= 0; vert--)
			glVertex3fv(Verts[Faces[face][vert]]);
		}

  glEnd();
}

void CubeClass::render()
{
	renderCCW();
}
