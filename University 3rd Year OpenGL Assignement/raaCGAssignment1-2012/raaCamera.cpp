#include "StdAfx.h"

#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include <raaMathsLibrary\raaVector.h>
#include <raaMathsLibrary\raaMatrix.h>

#include "raaCamera.h"

void camInit(raaCamera &rCam)
{
	vecInitPVec(rCam.m_fVP);
	vecInitPVec(rCam.m_fVT);
	vecInitDVec(rCam.m_fVR);
	vecInitDVec(rCam.m_fVD);
	vecInitDVec(rCam.m_fVU);
	vecSet(0.0f, 1.8f, 0.0f, rCam.m_fVP);
	vecSet(0.0f, 0.0f, -1.0f, rCam.m_fVD);
	vecSet(1.0f, 0.0f, 0.0f, rCam.m_fVR);
	vecSet(0.0f, 1.0f, 0.0f, rCam.m_fVU);
	vecProject(rCam.m_fVP, rCam.m_fVD, 1.0f, rCam.m_fVT);

	rCam.m_fNear=0.3f;
	rCam.m_fFar=100.0f;	
	rCam.m_bMotion=false;
	rCam.m_uiMode=csg_uiFly;
}

void camTravel(raaCamera &rCam, float fStep)
{
	float fLen;
	float aVec[4];

	switch(rCam.m_uiMode)
	{
		case csg_uiFly:
			vecProject(rCam.m_fVP, rCam.m_fVD, -fStep, rCam.m_fVP);
			vecProject(rCam.m_fVP, rCam.m_fVD, 1.0f, rCam.m_fVT);
			break;
		default:
			vecInitDVec(aVec);
			vecSub(rCam.m_fVP, rCam.m_fVT, aVec);
			fLen=vecNormalise(aVec, aVec)+fStep;
			if(fLen<rCam.m_fNear) fLen=rCam.m_fNear;
			if(fLen>rCam.m_fFar) fLen=rCam.m_fFar;
			vecProject(rCam.m_fVT, aVec, fLen, rCam.m_fVP);
			break;
	}
}

void camPanHori(raaCamera &rCam, float fStep)
{
	float af[]={0.0f, 0.0f, 0.0f, 0.0f};
	float fLen;

	switch(rCam.m_uiMode)
	{
		case csg_uiFly:
			vecProject(rCam.m_fVP, rCam.m_fVR, fStep, rCam.m_fVP);
			vecProject(rCam.m_fVP, rCam.m_fVD, 1.0f, rCam.m_fVT);
			break;
		default:
			vecSub(rCam.m_fVT, rCam.m_fVP, af);
			fLen=vecLength(af);
			vecProject(rCam.m_fVP, rCam.m_fVR, fStep, rCam.m_fVP);
			vecSub(rCam.m_fVT, rCam.m_fVP, rCam.m_fVD);
			vecNormalise(rCam.m_fVD, rCam.m_fVD);
			vecCrossProduct(rCam.m_fVD, rCam.m_fVU, rCam.m_fVR);
			vecNormalise(rCam.m_fVR, rCam.m_fVR);
			vecProject(rCam.m_fVT, rCam.m_fVD, -fLen, rCam.m_fVP);
			break;
	}
}

void camPanVert(raaCamera &rCam, float fStep)
{
	float af[]={0.0f, 0.0f, 0.0f, 0.0f};
	float fLen;

	switch(rCam.m_uiMode)
	{
		case csg_uiFly:
			vecProject(rCam.m_fVP, rCam.m_fVU, fStep, rCam.m_fVP);
			vecProject(rCam.m_fVP, rCam.m_fVD, 1.0f, rCam.m_fVT);			
			break;
		default:
			vecSub(rCam.m_fVT, rCam.m_fVP, af);
			fLen=vecLength(af);
			vecProject(rCam.m_fVP, rCam.m_fVU, fStep, rCam.m_fVP);
			vecSub(rCam.m_fVT, rCam.m_fVP, rCam.m_fVD);
			vecNormalise(rCam.m_fVD, rCam.m_fVD);
			vecCrossProduct(rCam.m_fVR, rCam.m_fVD, rCam.m_fVU);
			vecNormalise(rCam.m_fVU, rCam.m_fVU);
			vecProject(rCam.m_fVT, rCam.m_fVD, -fLen, rCam.m_fVP);
			break;
	}
}

void camRot(raaCamera &rCam, float fHStep, float fVStep)
{
	float af[]={0.0f, 0.0f, 0.0f, 0.0f};
	float fLen;

	switch(rCam.m_uiMode)
	{
		case csg_uiFly:
			vecProject(af, rCam.m_fVR, fHStep, af);
			vecAdd(rCam.m_fVD, af,rCam.m_fVD);
			vecNormalise(rCam.m_fVD, rCam.m_fVD);
			vecProject(rCam.m_fVP, rCam.m_fVD, 1.0f, rCam.m_fVT);
			vecCrossProduct(rCam.m_fVD, rCam.m_fVU, rCam.m_fVR);
			vecNormalise(rCam.m_fVR, rCam.m_fVR);
			vecProject(af, rCam.m_fVU, fVStep, af);
			vecAdd(rCam.m_fVD, af, rCam.m_fVD);
			vecNormalise(rCam.m_fVD, rCam.m_fVD);
			vecProject(rCam.m_fVP, rCam.m_fVD, 1.0f, rCam.m_fVT);
			vecCrossProduct(rCam.m_fVR, rCam.m_fVD, rCam.m_fVU);
			vecNormalise(rCam.m_fVU, rCam.m_fVU);
			break;
		default:
			vecSub(rCam.m_fVT, rCam.m_fVP, af);
			fLen=vecLength(af);
			vecProject(rCam.m_fVP, rCam.m_fVR, fHStep, rCam.m_fVP);
			vecSub(rCam.m_fVT, rCam.m_fVP, rCam.m_fVD);
			vecNormalise(rCam.m_fVD, rCam.m_fVD);
			vecCrossProduct(rCam.m_fVD, rCam.m_fVU, rCam.m_fVR);
			vecNormalise(rCam.m_fVR, rCam.m_fVR);
			vecProject(rCam.m_fVP, rCam.m_fVU, fVStep, rCam.m_fVP);
			vecSub(rCam.m_fVT, rCam.m_fVP, rCam.m_fVD);
			vecNormalise(rCam.m_fVD, rCam.m_fVD);
			vecCrossProduct(rCam.m_fVR, rCam.m_fVD, rCam.m_fVU);
			vecNormalise(rCam.m_fVU, rCam.m_fVU);
			vecProject(rCam.m_fVT, rCam.m_fVD, -fLen, rCam.m_fVP);
			break;
	}
}

void applyCamera(raaCamera &rCam)
{
	gluLookAt(rCam.m_fVP[0], rCam.m_fVP[1], rCam.m_fVP[2], rCam.m_fVT[0], rCam.m_fVT[1], rCam.m_fVT[2], rCam.m_fVU[0], rCam.m_fVU[1], rCam.m_fVU[2]); 
}

void getObjMat( raaCamera &rCam, float *pafMat)
{
	float mR[16], mT[16];

	matMakeIdentity(mR);
	matMakeIdentity(mT);

	mR[0]=rCam.m_fVR[0];
	mR[1]=rCam.m_fVR[1];
	mR[2]=rCam.m_fVR[2];
	mR[4]=rCam.m_fVU[0];
	mR[5]=rCam.m_fVU[1];
	mR[6]=rCam.m_fVU[2];
	mR[8]=-rCam.m_fVD[0];
	mR[9]=-rCam.m_fVD[1];
	mR[10]=-rCam.m_fVD[2];
	mT[12]=rCam.m_fVP[0];
	mT[13]=rCam.m_fVP[1];
	mT[14]=rCam.m_fVP[2];

	matMatMult(mR, mT, pafMat);
}

void camToFly( raaCamera &rCam )
{
	rCam.m_uiMode=csg_uiFly;
	vecProject(rCam.m_fVP, rCam.m_fVD, 1.0f, rCam.m_fVT);
}

void camToExplore( raaCamera &rCam, float *pvTarget, unsigned int uiSteps, DisplayFunc pFunc )
{
	float afOldTarget[4];
	float afPath[4], afNormal[4];
	
	if(pvTarget && pFunc)
	{
		rCam.m_uiMode=csg_uiExplore;

		vecInitDVec(afPath);
		vecInitDVec(afNormal);

		afNormal[1]=1.0f;
		afOldTarget[0]=rCam.m_fVT[0];
		afOldTarget[1]=rCam.m_fVT[1];
		afOldTarget[2]=rCam.m_fVT[2];
		afOldTarget[3]=rCam.m_fVT[3];

		vecSub(pvTarget, afOldTarget, afPath);

		for(unsigned int i=0;i<uiSteps;i++)
		{
			float fStep=((float)i)/(float)uiSteps;			
			vecProject(afOldTarget, afPath, fStep, rCam.m_fVT);
			vecSub(rCam.m_fVT, rCam.m_fVP, rCam.m_fVD);
			vecNormalise(rCam.m_fVD, rCam.m_fVD);
			vecCrossProduct(rCam.m_fVD, afNormal, rCam.m_fVR);
			vecNormalise(rCam.m_fVR, rCam.m_fVR);
			vecCrossProduct(rCam.m_fVR, rCam.m_fVD, rCam.m_fVU);
			vecNormalise(rCam.m_fVU, rCam.m_fVU);
			pFunc();
		}
		vecSet(pvTarget[0], pvTarget[1], pvTarget[2], rCam.m_fVT);
	}
}

