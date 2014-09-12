#ifndef _HUDCamer_
#define _HUDCamer_

#include "stdafx.h"

#include <osg\Camera>
#include <osg\Vec4d>

using namespace std;

class HUDCamera : public osg::Camera
{

public: 

	HUDCamera( string sName, osg::Vec4d vOrthMatrixValues );
	virtual ~HUDCamera();

protected:

	osg::Vec4d _vOrthMatrixValues;
	string _sName;

	void setup();
};

#endif