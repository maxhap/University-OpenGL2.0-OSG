#pragma once

// NOTE: a vector is assumed to be a float[4] array

void vecInitPVec(float *pV); // point vector
void vecInitDVec(float *pV); // direction vector
void vecInitCVec(float *pV); // colour vector
void vecInit(float *pV); // generic vector
void vecAdd(float *pV0, float *pV1, float *pVRes);
void vecSub(float *pV0, float *pV1, float *pVRes);
void vecScalarProduct(float *pV0, float fS, float *pVRes);
void vecVectorProduct(float *pV0, float *pV1, float *pVRes);
float vecDotProduct(float *pV0, float *pV1);
void vecCrossProduct(float *pV0, float *pV1, float *pVRes);
float vecLength(float *pV);
float vecNormalise(float *pV0, float *pVRes);
void vecProject(float *pVStart, float *pVDir, float fLen, float *pVRes);
void vecSet(float fX, float fY, float fZ, float *pV);