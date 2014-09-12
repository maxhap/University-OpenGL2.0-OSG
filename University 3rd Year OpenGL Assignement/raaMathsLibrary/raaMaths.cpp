#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include "raaMaths.h"

void mathsInit()
{
	srand((unsigned int)time(0));
}

float degToRad(float fDeg)
{
	return (fDeg/180.0f)*csg_fPI;
}

float radToDeg(float fRad)
{
	return (fRad/csg_fPI)*180.0f;
}

float randFloat(float fMin, float fMax)
{
	return (((float)rand()/(float)RAND_MAX)*(fMax-fMin))+fMin;
}
