#pragma once

//NOTE: a matrix is assumed to be a float[16] array. This translates to a row ordered OpenGL matrix 
void matMakeIdentity(float *pM);
void matMakeTranslation(float fX, float fY, float fZ, float *pM);
void matMakeScale(float fX, float fY, float fZ, float *pM);
void matMakeXAxisRot(float fRadians, float *pM);
void matMakeYAxisRot(float fRadians, float *pM);
void matMakeZAxisRot(float fRadians, float *pM);
void matTranslate(float fX, float fY, float fZ, float *pM);
void matScale(float fX, float fY, float fZ, float *pM);
void matXAxisRot(float fRadians, float *pM);
void matYAxisRot(float fRadians, float *pM);
void matZAxisRot(float fRadians, float *pM);
void matVecMult(float *pV, float *pM, float *pVRes);
void matMatMult(float *pM0, float *pM1, float *pMRes);
void matCopy(float *pM0, float *pM1);
