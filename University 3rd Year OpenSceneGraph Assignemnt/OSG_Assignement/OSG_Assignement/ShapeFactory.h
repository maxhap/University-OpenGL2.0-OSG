#ifndef _SHAPEFACTORY_
#define _SHAPEFACTORY_

#include "stdafx.h"
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile> 
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <vector>

#include <random>

#include "IcoSphereGeometry.h"


class ShapeFactory
{
	ShapeFactory( const ShapeFactory& );
	ShapeFactory& operator = ( const ShapeFactory& );

public: 
	
	virtual ~ShapeFactory();

	static ShapeFactory& instance()
	{
		static ShapeFactory instance;
		return instance;
	};

	osg::Geode* getPyramid();
	osg::Geode* getSphere();
	osg::Geode* getRectangle( float fWidth, float fHeight );
	osg::Geode* getBox();
	osg::Geode* getWireframeSphere( float fRadious, osg::Vec4f vColor );

	osg::Geode* ShapeFactory::createBox1( osg::Vec3f &vHalfDim, osg::Vec4f &vColour );

protected:
	
	ShapeFactory();	

	static osg::Vec3f calculateFaceNormal( osg::Vec3f vPoint1, osg::Vec3f vPoint2, osg::Vec3f vPoint3 );

};
#endif