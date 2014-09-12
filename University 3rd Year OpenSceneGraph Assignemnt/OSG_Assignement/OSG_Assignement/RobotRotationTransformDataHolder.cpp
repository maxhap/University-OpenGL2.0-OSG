#include "stdafx.h"
#include "RobotRotationTransformDataHolder.h"

RobotRotationTransformDataHolder::RobotRotationTransformDataHolder( float* pafLimits, osg::Switch* psColourSwitch, osg::MatrixTransform* pMatrix, float fRotationAndMovementSpeed )
		: TransformDataHolder( pMatrix, fRotationAndMovementSpeed )
{
	_pafLimits = pafLimits;
	_mOrigionalMatrix = ( osg::Matrixf ) pMatrix->getMatrix();
	_mPermUpdater = osg::Matrix::identity();
	_psColourSwitch = psColourSwitch;
	_psColourSwitch->ref();
}

RobotRotationTransformDataHolder::~RobotRotationTransformDataHolder()
{
	if( _psColourSwitch ) _psColourSwitch->unref();
	if( _pafLimits ) delete _pafLimits;	
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: update
Description: 1. if object moving update
			 2. if new matrix has not exceeded any of the limits then set new matrix
			 3. set material color depending on state 
----------------------------------------------------------------------------------*/
void RobotRotationTransformDataHolder::update()
{
	if( _uiMovement != NO_MOVEMENT )
	{

		osg::Matrixf mTemp = _mPermUpdater * _mUpdater;

		if( mTemp.getRotate().asVec3().x() <= _pafLimits[LIMIT_RIGHT] &&
			mTemp.getRotate().asVec3().x() >= -_pafLimits[LIMIT_LEFT] && 
			mTemp.getRotate().asVec3().y() <= _pafLimits[LIMIT_UP] &&
			mTemp.getRotate().asVec3().y() >= -_pafLimits[LIMIT_DOWN] &&
			mTemp.getRotate().asVec3().z() <= _pafLimits[LIMIT_AROUND_LEFT] &&
			mTemp.getRotate().asVec3().z() >= -_pafLimits[LIMIT_AROUND_RIGHT] )
		{
			_psColourSwitch->setSingleChildOn( 1 );

			_pmObject->setMatrix( _mOrigionalMatrix * mTemp );

			_mPermUpdater = mTemp;

			return;
		}

		_psColourSwitch->setSingleChildOn( 2 );

		return;
	}	

	_psColourSwitch->setSingleChildOn( 0 );

	return;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setMovement
Description: call setMovement in superclass		 
----------------------------------------------------------------------------------*/
void RobotRotationTransformDataHolder::setMovement( unsigned int uiMovement )
{
	//change direction rotation allowing reuse of arrow keys for specific parts
	if( _pmObject->getName().compare( "Hand3_Rotator" ) == 0 && uiMovement == ROTATE_AROUND_LEFT ) 
	{
		uiMovement = ROTATE_LEFT;
	}
	if( _pmObject->getName().compare( "Hand3_Rotator" ) == 0 && uiMovement == ROTATE_AROUND_RIGHT ) uiMovement = ROTATE_RIGHT;

	TransformDataHolder::setMovement( uiMovement );
}

void RobotRotationTransformDataHolder::serialise( stringstream& ofFile )
{
	ofFile << "<NodeType>RobotTransform</NodeType>" << "\n";
	ofFile << "<RotationX>" << _mPermUpdater.getRotate().x() <<"</RotationX>" << "\n";
	ofFile << "<RotationY>" << _mPermUpdater.getRotate().y() <<"</RotationY>" << "\n";
	ofFile << "<RotationZ>" << _mPermUpdater.getRotate().z() <<"</RotationZ>" << "\n";
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setData
Description: 1. set to pmObject matrix from parameters passed in
----------------------------------------------------------------------------------*/
void RobotRotationTransformDataHolder::setData( float x, float y, float z )
{	
	osg::Matrixf mTemp;
	mTemp.makeRotate( x, 1.0f, 0.0f, 0.0f );
	_mUpdater = _mUpdater * mTemp;
	mTemp.makeRotate( y, 0.0f, 1.0f, 0.0f );		
	_mUpdater = _mUpdater * mTemp;
	mTemp.makeRotate( z, 0.0f, 0.0f, 1.0f );
	_mUpdater = _mUpdater * mTemp;

	_pmObject->setMatrix( _mOrigionalMatrix * _mUpdater );
}

