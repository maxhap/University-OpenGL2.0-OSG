#ifndef _ICOSPHERE_
#define  _ICOSPHERE_

#include "stdafx.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
#include <osgUtil/UpdateVisitor>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>
#include <vector>
#include <map>
#include <math.h>

using namespace std;

class IcoSphereGeometry 
{

public:

	IcoSphereGeometry( unsigned int uiRecursionLevel );
	virtual ~IcoSphereGeometry();

	std::vector<osg::Vec3f*>* getVertexs();
	std::vector<osg::Vec3f*>* getTriangleIndexes();

protected:

	std::vector<osg::Vec3f*>* _pvVertexs;
	std::map<string, unsigned int>* _pvIndexCash;
	std::vector<osg::Vec3f*>* _pvTriangleIndexes;

	unsigned int _uiIndex;
	unsigned int _uiRecursionLevel;

	unsigned int addVertex( osg::Vec3f* v3Point );
	unsigned int getMiddlePoint( unsigned int uiFirstIndex, unsigned int uiSecondIndex );
	void createPoints();
};

#endif