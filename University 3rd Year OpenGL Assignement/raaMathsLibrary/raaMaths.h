#pragma once

const static float csg_fPI=3.14159265f;
const static unsigned int csg_uiRandPrecision=100000;

void mathsInit();

float degToRad(float fDeg);
float radToDeg(float fRad);

float randFloat(float fMin=0.0f, float fMax=1.0f);
