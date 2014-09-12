#ifndef _MODELROBOT_
#define _MODELROBOT_

#include "stdafx.h"

#include <osg/Node>
#include <osg/Group>
#include <osg/NodeVisitor>
#include <osg/Switch>
#include <osg/StateSet>
#include <osg/Geode>
#include <osg/Vec4f>
#include <osg/Material>
#include <osg/StateSet>
#include <iostream>
#include <fstream>

#include <OSG_Utils/NodeFinder.h>

#include "SceneManipulator.h"
#include "TransformCallBack.h"

using namespace std;

class ModelRobot 
{

public:

	ModelRobot( string sSGFilePath, string sLimitsFilePath );
	virtual ~ModelRobot();

	osg::Node* getModel();
	void incrementSelection();
	void setMovement( unsigned int uiDirection );

protected:

	string _sFilePath;
	string _sLimitsFilePath;

	osg::Node* _pnRobotRoot;

	map<string, osg::MatrixTransform*>* _pmRotationNodes;

	int _uiCurrentSelectedNode;

	void loadModel();
	void parseRotationNodes();
	void buildNewRotationNodeStructure( osg::MatrixTransform* pRotationNode ); 
	void buildCallbackStrcture( float* pafLimits, osg::MatrixTransform* pRotationNode );
};

#endif