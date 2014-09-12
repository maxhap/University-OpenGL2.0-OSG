#pragma once

const static unsigned int csg_uiExplore=0;
const static unsigned int csg_uiFly=1;

typedef void (*DisplayFunc)(void);

typedef struct _raaCamera
{
	unsigned int m_uiMode;
	float m_fVP[4];
	float m_fVT[4];
	float m_fVR[4];
	float m_fVU[4];
	float m_fVD[4];
	float m_fNear;
	float m_fFar;
	bool m_bMotion;
	int m_aiLastMousePos[2];
	float m_afMouseDisp[2];
} raaCamera;

void camInit(raaCamera &rCam);
void camToFly(raaCamera &rCam);
void camToExplore(raaCamera &rCam, float *fvTarget, unsigned int uiSteps, DisplayFunc pFunc);
void camTravel(raaCamera &rCam, float fStep);
void camPanHori(raaCamera &rCam, float fStep);
void camPanVert(raaCamera &rCam, float fStep);
void camRot(raaCamera &rCam, float fHStep, float fVStep);
void applyCamera(raaCamera &rCam);

// use this to get the current camera matrix in a form to position objects
void getObjMat(raaCamera &rCam, float *pafMat);