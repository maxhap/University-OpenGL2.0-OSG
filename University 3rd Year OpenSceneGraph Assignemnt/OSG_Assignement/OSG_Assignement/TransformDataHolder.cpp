#include "stdafx.h"

TransformDataHolder::TransformDataHolder( osg::MatrixTransform* pMatrix, float fRotationAndMovementSpeed ) : DataHolder()
{
	_fRotationAndMovementSpeed = fRotationAndMovementSpeed;
	_pmObject = pMatrix;
	_pmObject->ref();
	_mUpdater = osg::Matrixf::identity();
	_uiMovement = NO_MOVEMENT;
}

TransformDataHolder::~TransformDataHolder()
{
	if( _pmObject ) _pmObject->unref();
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setMovement
Description: 1. based on movement movement is determined using a switch with Movement 
				const cases
			 2. _mUpdater Matrix set to match			 
----------------------------------------------------------------------------------*/
void TransformDataHolder::setMovement( unsigned int uiMovement )
{
	_uiMovement = uiMovement;

	switch( uiMovement )
	{
		case NO_MOVEMENT: _mUpdater = osg::Matrixf::identity(); break;
		case ROTATE_UP: _mUpdater.makeRotate( _fRotationAndMovementSpeed, 1.0f, 0.0f, 0.0f ); break;
		case ROTATE_DOWN: _mUpdater.makeRotate( -_fRotationAndMovementSpeed, 1.0f, 0.0f, 0.0f ); break;
		case ROTATE_LEFT: _mUpdater.makeRotate( -_fRotationAndMovementSpeed, 0.0f, 1.0f, 0.0f ); break;
		case ROTATE_RIGHT: _mUpdater.makeRotate( _fRotationAndMovementSpeed, 0.0f, 1.0f, 0.0f ); break;
		case ROTATE_AROUND_LEFT: _mUpdater.makeRotate( _fRotationAndMovementSpeed, 0.0f, 0.0f, 1.0f ); break;
		case ROTATE_AROUND_RIGHT: _mUpdater.makeRotate( -_fRotationAndMovementSpeed, 0.0f, 0.0f, 1.0f ); break;

		case TRANSLATE_UP: _mUpdater.makeTranslate( osg::Vec3f( 0.0f, 0.0f, _fRotationAndMovementSpeed ) ); break;
		case TRANSLATE_DOWN: _mUpdater.makeTranslate( osg::Vec3f( 0.0f, 0.0f, -_fRotationAndMovementSpeed ) ); break; 
		case TRANSLATE_LEFT: _mUpdater.makeTranslate( osg::Vec3f( -_fRotationAndMovementSpeed, 0.0f, 0.0f ) ); break;
		case TRANSLATE_RIGHT: _mUpdater.makeTranslate( osg::Vec3f( _fRotationAndMovementSpeed, 0.0f, 0.0f ) ); break;
	}
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: update
Description: 1. if object is moving it will need updating, multiply by updater matrix
			 2. then use LogicEngine::instance().updateModifiresWithModel() to update
				the dragger's
----------------------------------------------------------------------------------*/
void TransformDataHolder::update()
{
	if( _uiMovement != NO_MOVEMENT )
	{		
		_pmObject->setMatrix( _mUpdater * _pmObject->getMatrix() );
		LogicEngine::instance().updateModifiresWithModel( _pmObject );
	}	
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: serialise
Description: 1. write user to streamstring parameter as xml format
----------------------------------------------------------------------------------*/
void TransformDataHolder::serialise( stringstream& ofFile )
{
	ofFile << "<NodeType>Transform</NodeType>" << "\n";
	ofFile << "<RotationX>" << _pmObject->getMatrix().getRotate().x() <<"</RotationX>" << "\n";
	ofFile << "<RotationY>" << _pmObject->getMatrix().getRotate().y() <<"</RotationY>" << "\n";
	ofFile << "<RotationZ>" << _pmObject->getMatrix().getRotate().z() <<"</RotationZ>" << "\n";
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setData
Description: 1. set to pmObject matrix from parameters passed in
----------------------------------------------------------------------------------*/
void TransformDataHolder::setData( float x, float y, float z )
{	
	//_mUpdater.makeRotate( _fRotationAndMovementSpeed, osg::DegreesToRadians( z ), osg::DegreesToRadians( y ), osg::DegreesToRadians( z ) );		

	_pmObject->setMatrix( _pmObject->getMatrix() * _mUpdater );
}

