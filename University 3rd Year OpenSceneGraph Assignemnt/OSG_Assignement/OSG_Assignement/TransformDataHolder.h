#ifndef _TRANSDATAHOLDER_
#define _TRANSDATAHOLDER_

#include "stdafx.h"
#include <osg/Referenced>
#include <osg/MatrixTransform>

#include "LogicEngine.h"
#include <OSG_Utils/DataHolder.h>

class TransformDataHolder : public DataHolder
{

public:

	static const unsigned int NO_MOVEMENT = 0;
	static const unsigned int ROTATE_UP = 1;
	static const unsigned int ROTATE_DOWN = 2;
	static const unsigned int ROTATE_LEFT = 3;
	static const unsigned int ROTATE_RIGHT = 4;
	static const unsigned int ROTATE_AROUND_LEFT = 5;
	static const unsigned int ROTATE_AROUND_RIGHT = 6;

	static const unsigned int TRANSLATE_UP = 7;
	static const unsigned int TRANSLATE_DOWN = 8;
	static const unsigned int TRANSLATE_LEFT = 9;
	static const unsigned int TRANSLATE_RIGHT = 10;

	TransformDataHolder( osg::MatrixTransform* pMatrix, float fRotationAndMovementSpeed );
	virtual ~TransformDataHolder();

	virtual void setMovement( unsigned int uiMovement );
	virtual void update();
	virtual void serialise( stringstream& ofFile );
	virtual void setData( float x, float y, float z );

protected:

	unsigned int _uiMovement;
	osg::MatrixTransform* _pmObject;
	osg::Matrixf _mUpdater;

	float _fRotationAndMovementSpeed;
};

#endif