#include "StdAfx.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <raaMathsLibrary\raaVector.h>

#include "raaUtilities.h"

void makeGrid(unsigned long &ruiDL, float fWidth, float fDepth, unsigned int uiWSections, unsigned int uiDSections)
{
	float fHalfWidth=fWidth*0.5f;
	float fHalfDepth=fDepth*0.5f;
	float fWStep=fWidth/(float)uiWSections;
	float fDStep=fDepth/(float)uiDSections;

	if(!ruiDL) ruiDL=glGenLists(1);

	glNewList(ruiDL, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.8f, 0.8f);
	glBegin(GL_LINES);

	float fW=-fHalfWidth;
	for(unsigned int i=0;i<=uiWSections;i++, fW+=fWStep)
	{
		glVertex3f(fW, 0.0f, fHalfDepth);
		glVertex3f(fW, 0.0f, -fHalfDepth);
	}

	float fD=-fHalfDepth;
	for(unsigned int i=0;i<=uiDSections;i++, fD+=fDStep)
	{
		glVertex3f(fHalfWidth, 0.0f, fD);
		glVertex3f(-fHalfWidth, 0.0f, fD);
	}

	glEnd();
	glPopAttrib();
	glEndList();
}
