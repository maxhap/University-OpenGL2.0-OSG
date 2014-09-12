#ifndef _RobotRotationTransformDataHolder1_
#define _RobotRotationTransformDataHolder1_

#include "stdafx.h"

#include <osg/Referenced>
#include <osg/MatrixTransform>

#include <OSG_Utils/DataHolder.h>

class RobotRotationTransformDataHolder : public TransformDataHolder
{

public:

	static const unsigned int LIMIT_LEFT = 0;
	static const unsigned int LIMIT_RIGHT = 1;
	static const unsigned int LIMIT_UP = 2;
	static const unsigned int LIMIT_DOWN = 3;
	static const unsigned int LIMIT_AROUND_LEFT = 4;
	static const unsigned int LIMIT_AROUND_RIGHT = 5;

	RobotRotationTransformDataHolder( float* pafLimits, osg::Switch* psColourSwitch, osg::MatrixTransform* pMatrix, float fRotationAndMovementSpeed );
	virtual ~RobotRotationTransformDataHolder();

	virtual void update();
	virtual void setMovement( unsigned int uiMovement );
	virtual void serialise( stringstream& ofFile );
	virtual void setData( float x, float y, float z );

protected:

	float* _pafLimits;
	osg::Matrixf _mOrigionalMatrix;
	osg::Matrixf _mPermUpdater;
	osg::Switch* _psColourSwitch;

};

#endif