#ifndef _HUD_
#define  _HUD_

#include "stdafx.h"

#include <vector>

#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Drawable>
#include <osg/Vec4d>

#include "HUDCamera.h"
#include "HUDItem.h"
#include "HUDItemCallBack.h"
#include "LogicEngine.h"

using namespace std;

class LogicEngine;

class HUD : public osg::Group
{

public:

	HUD( osg::Vec4d  vWindowSize, bool bWithRobot );
	virtual ~HUD();

	HUDCamera* getCamera();

	void setSelectedItem( string sName );
	

protected:

	HUDCamera* _pHudCamera;

	vector<HUDItem*>* _pvItemList; 

	void setup( bool bWithRobot );
	void setToggleItem( string sName );
};

#endif