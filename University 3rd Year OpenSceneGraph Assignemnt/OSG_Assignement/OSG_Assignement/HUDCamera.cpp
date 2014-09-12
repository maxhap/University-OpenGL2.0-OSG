#include "stdafx.h"
#include "HUDCamera.h"

HUDCamera::HUDCamera( string sName, osg::Vec4d vOrthMatrixValues )
{
	_sName = sName;
	_vOrthMatrixValues = vOrthMatrixValues;

	setup();
}

HUDCamera::~HUDCamera()
{

}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setup
Description: set inherited setProjectionMatrixAsOrtho2D to constructor parameter
			 ,set POST_RENDER so item alway appears in front
----------------------------------------------------------------------------------*/
void HUDCamera::setup()
{
	this->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	this->setProjectionMatrixAsOrtho2D( _vOrthMatrixValues.x(), _vOrthMatrixValues.y(), _vOrthMatrixValues.z(), 
									    _vOrthMatrixValues.w() );
	this->setViewMatrix( osg::Matrix::identity() );
	this->setRenderOrder( osg::Camera::POST_RENDER );
	this->setClearMask( GL_DEPTH_BUFFER_BIT );
}
