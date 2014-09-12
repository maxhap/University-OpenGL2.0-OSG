#ifndef _SCENEMANIPULATOR_
#define _SCENEMANIPULATOR_

#include "stdafx.h"

#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Drawable>

#include <osgManipulator/Dragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TranslateAxisDragger>

#include <OSG_Utils/NodeNameFinder.h>
#include <OSG_Utils/NodeFinder.h>

#include "ShapeFactory.h"
#include "TransformCallBack.h"
#include "GroupDataHolder.h"

class TransformDataHolder;

using namespace std;

class SceneManipulator
{
	
public:

	//scene objects
	static const unsigned int SCENEFILE = 0;
	static const unsigned int SPHERE = 1;
	static const unsigned int PHYRAMID = 2;
	static const unsigned int RECTANGLE = 3;
	static const unsigned int BOX = 4;	

	SceneManipulator();
	virtual ~SceneManipulator();	

	static void addObjectToScene( osg::Group* pnScene, unsigned int uiObject, string sName, string sTexture, osg::Matrixf mPosition );
	static void addGeometryToScene( osg::Group* pnScene, string sName, string sTexture, osg::Matrixf mPosition, osg::Geode* pnGeometry );
	static void addSDFToScene( osg::Group* pnScene, string sName, osg::Matrixf mPosition, string sFileName );
	static osg::Group* addMySDFToScene( osg::Group* pnScene, string sName, string sFileName, string sTexture );
	
protected:

	static osg::Group* buildManipulationStructure( string sName, osg::Matrixf mPosition );
	static osg::Group* buildManipulationStructureUsingMT( string sName, osg::MatrixTransform* pmtPosition );
	static osg::StateSet* loadTexture( string sName, osg::StateSet* pssToChange = NULL );
	static void addDragger( osgManipulator::Dragger* pDragger, osg::Switch* pSwitch, osg::MatrixTransform* pMatrixTransform, osg::Matrix mScale );	
	static void addSelectionStructure( string sName, osg::MatrixTransform* pMT );
};

#endif