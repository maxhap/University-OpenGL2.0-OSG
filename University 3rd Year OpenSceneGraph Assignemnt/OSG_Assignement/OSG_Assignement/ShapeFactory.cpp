#include "stdafx.h"
#include "ShapeFactory.h"

ShapeFactory::ShapeFactory()
{	
}

ShapeFactory::~ShapeFactory()
{
}
	
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getPyramid
description: Build a pyramid by creating a square base and three side triangle faces
			 for each face set material colour, calculate normals using 
			 calculateNormals() and add texture points 
----------------------------------------------------------------------------------*/
osg::Geode* ShapeFactory::getPyramid()
{
	osg::Geode* pPyramidGeode = new osg::Geode();
	osg::Geometry* pyramidGeometry = new osg::Geometry();

	osg::StateSet* pStateSet = pPyramidGeode->getOrCreateStateSet();

	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

	osg::Material* pMaterial = new osg::Material(); 	
	pMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 1.0f, 1.0f, 1.0f, 1.0f ) );	
	pMaterial->setShininess( osg::Material::FRONT,  0.2f );

	pStateSet->setAttribute( pMaterial );

	pPyramidGeode->addDrawable(pyramidGeometry);

	osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
	pyramidVertices->push_back( osg::Vec3( -1, -1, -1) ); // front left
	pyramidVertices->push_back( osg::Vec3( 1, -1, -1 ) ); // front right
	pyramidVertices->push_back( osg::Vec3( 1, 1, -1 ) ); // back right
	pyramidVertices->push_back( osg::Vec3( -1, 1, -1 ) ); // back left
	pyramidVertices->push_back( osg::Vec3( 0, 0, 1 ) ); // peak

	pyramidGeometry->setVertexArray( pyramidVertices );

	osg::Vec3Array* pNorms = new osg::Vec3Array();	

	osg::DrawElementsUInt* pyramidBase =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	pyramidBase->push_back(3);
	pyramidBase->push_back(2);
	pyramidBase->push_back(1);
	pyramidBase->push_back(0);
	pyramidGeometry->addPrimitiveSet(pyramidBase);

	pNorms->push_back( calculateFaceNormal( ( *pyramidVertices )[3], ( *pyramidVertices )[2], ( *pyramidVertices )[1] ) );

	osg::DrawElementsUInt* pyramidFaceOne =
		new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceOne->push_back(0);
	pyramidFaceOne->push_back(1);
	pyramidFaceOne->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceOne);

	pNorms->push_back( calculateFaceNormal( ( *pyramidVertices )[0], ( *pyramidVertices )[1], ( *pyramidVertices )[4] ) );

	osg::DrawElementsUInt* pyramidFaceTwo =
		new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceTwo->push_back(1);
	pyramidFaceTwo->push_back(2);
	pyramidFaceTwo->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceTwo);

	pNorms->push_back( calculateFaceNormal( ( *pyramidVertices )[1], ( *pyramidVertices )[2], ( *pyramidVertices )[4] ) );

	osg::DrawElementsUInt* pyramidFaceThree =
		new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceThree->push_back(2);
	pyramidFaceThree->push_back(3);
	pyramidFaceThree->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceThree);

	pNorms->push_back( calculateFaceNormal( ( *pyramidVertices )[2], ( *pyramidVertices )[3], ( *pyramidVertices )[4] ) );

	osg::DrawElementsUInt* pyramidFaceFour =
		new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceFour->push_back(3);
	pyramidFaceFour->push_back(0);
	pyramidFaceFour->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceFour);

	pNorms->push_back( calculateFaceNormal( ( *pyramidVertices )[3], ( *pyramidVertices )[0], ( *pyramidVertices )[4] ) );

	pyramidGeometry->setNormalArray( pNorms );
	pyramidGeometry->setNormalBinding( osg::Geometry::BIND_PER_PRIMITIVE );

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 0 red
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); //index 1 green
	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) ); //index 2 blue
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 3 white
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 4 red

	pyramidGeometry->setColorArray(colors);
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::Vec2Array* texcoords = new osg::Vec2Array(5);
	(*texcoords)[0].set(0.00f,0.0f); // tex coord for vertex 0 
	(*texcoords)[1].set(0.25f,0.0f); // tex coord for vertex 1 
	(*texcoords)[2].set(0.50f,0.0f); //  ""
	(*texcoords)[3].set(0.75f,0.0f); //  "" 
	(*texcoords)[4].set(0.50f,1.0f); //  ""
	pyramidGeometry->setTexCoordArray(0,texcoords);	

	return pPyramidGeode;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getSphere
description: Using IcoSphereGeometry to generate a spheres geometry  with defined 
		     smoothness iterations, build up osg structure to represent created
			 geometry. Lighting is disabled, and a random number is generated for
			 each vertex colour
----------------------------------------------------------------------------------*/
osg::Geode* ShapeFactory::getSphere()
{	
	IcoSphereGeometry pIsg( 2 );

	osg::Geode* pSphereGeode = new osg::Geode();
	osg::Geometry* pSphereGeometry = new osg::Geometry();

	pSphereGeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	pSphereGeode->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF ); 

	pSphereGeode->addDrawable( pSphereGeometry );

	osg::Vec3Array* pSphereVertices = new osg::Vec3Array;

	int count = 0; 

	for( std::vector<osg::Vec3f*>::iterator i = pIsg.getVertexs()->begin(); i != pIsg.getVertexs()->end(); i++ )
	{
		pSphereVertices->push_back( osg::Vec3f( ( *i )->x(), ( *i )->y(), ( *i )->z() ) );		
	}

	pSphereGeometry->setVertexArray( pSphereVertices );

	count = 0; 

	for( std::vector<osg::Vec3f*>::iterator i = pIsg.getTriangleIndexes()->begin(); i != pIsg.getTriangleIndexes()->end(); i++ )
	{
		osg::DrawElementsUInt* pSphereFace = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
		pSphereFace->push_back( ( *i )->x() );	
		pSphereFace->push_back( ( *i )->y() );
		pSphereFace->push_back( ( *i )->z() );

		pSphereGeometry->addPrimitiveSet( pSphereFace );		

		count++;
	}

	//Declare and load an array of Vec4 elements to store colors.

	osg::Vec4Array* colors = new osg::Vec4Array;

	for( int i = 0; i < count; i++ )
	{
		float fColour_1 = ( rand() % 10 ) / 10.0f;
		float fColour_2 = ( rand() % 10 ) / 10.0f;
		float fColour_3 = ( rand() % 10 ) / 10.0f;

		colors->push_back(osg::Vec4( fColour_1, fColour_2, fColour_3, 1.0f) );
	}

	pSphereGeometry->setColorArray( colors );
	pSphereGeometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

	return pSphereGeode;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getRectangle
description: Create a singe quad with texture points, normals and material
----------------------------------------------------------------------------------*/
osg::Geode* ShapeFactory::getRectangle( float fWidth, float fHeight )
{
	osg::Geode* pRectangleGeode = new osg::Geode();
	osg::Geometry* pRectangleGeometry = new osg::Geometry();

	osg::StateSet* pStateSet = pRectangleGeode->getOrCreateStateSet();

	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

	osg::Material* pMaterial = new osg::Material(); 	
	pMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 1.0f, 1.0f, 1.0f, 1.0f ) );	
	pMaterial->setShininess( osg::Material::FRONT,  0.2f );

	pStateSet->setAttribute( pMaterial );

	//pRectangleGeode->addDrawable( pRectangleGeometry );
	pRectangleGeode->addDrawable( new osg::ShapeDrawable( new osg::Box( osg::Vec3f(), 1.0f, 1.0f, 0.01f ) ) );

	osg::Vec3Array* pRectangleVertices = new osg::Vec3Array;
	pRectangleVertices->push_back( osg::Vec3( -fWidth, fHeight, 0.0f ) ); // bottom left
	pRectangleVertices->push_back( osg::Vec3( fWidth, fHeight, 0.0f) ); // bottom right
	pRectangleVertices->push_back( osg::Vec3( fWidth, -fHeight, 0.0f ) ); // top right
	pRectangleVertices->push_back( osg::Vec3( -fWidth, -fHeight, 0.0f ) ); // top left

	pRectangleGeometry->setVertexArray( pRectangleVertices );

	osg::DrawElementsUInt* puiPyramidBase =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	puiPyramidBase->push_back(3);
	puiPyramidBase->push_back(2);
	puiPyramidBase->push_back(1);
	puiPyramidBase->push_back(0);
	pRectangleGeometry->addPrimitiveSet(puiPyramidBase);


	osg::Vec4Array* paColors = new osg::Vec4Array;
	paColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 0 white
	paColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 1 white
	paColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 2 white
	paColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 3 white

	pRectangleGeometry->setColorArray(paColors);
	pRectangleGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::Vec2Array* paTexcoords = new osg::Vec2Array(5);
	(*paTexcoords)[0].set( 0.0f, 1.0f );
	(*paTexcoords)[1].set( 0.0f,0.0f ); 
	(*paTexcoords)[3].set( 1.0f,1.0f ); 
	(*paTexcoords)[2].set( 1.0f,0.0f ); 
	pRectangleGeometry->setTexCoordArray(0,paTexcoords);

	return pRectangleGeode;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getBox
description: Create a four perpendicular quad with texture points, normals and material
----------------------------------------------------------------------------------*/
osg::Geode* ShapeFactory::getBox()
{
	float fDim = 1.0f;

	// make a vertex based shape
	osg::Geode* pGeode=new osg::Geode();
	osg::Geometry* pGeom=new osg::Geometry();	

	osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();

	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );


	osg::Material* pMaterial = new osg::Material(); 	
	pMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 1.0f, 0.0f, 0.0f, 1.0f ) );	
	pMaterial->setShininess( osg::Material::FRONT,  0.2f );

	pStateSet->setAttribute( pMaterial );

	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box( osg::Vec3f(), 1 ) ) );
	//pGeode->addDrawable( pGeom );
	
	float fHalfDim = fDim*0.5f;
	 
	osg::Vec3Array *pVerts=new osg::Vec3Array();
	pVerts->push_back(osg::Vec3(-fHalfDim, -fHalfDim, -fHalfDim));
	pVerts->push_back(osg::Vec3(-fHalfDim, fHalfDim, -fHalfDim));
	pVerts->push_back(osg::Vec3(fHalfDim, fHalfDim, -fHalfDim));
	pVerts->push_back(osg::Vec3(fHalfDim, -fHalfDim, -fHalfDim));

	pVerts->push_back(osg::Vec3(-fHalfDim, -fHalfDim, fHalfDim));
	pVerts->push_back(osg::Vec3(fHalfDim, -fHalfDim, fHalfDim));
	pVerts->push_back(osg::Vec3(fHalfDim, fHalfDim, fHalfDim));
	pVerts->push_back(osg::Vec3(-fHalfDim, fHalfDim, fHalfDim));

	pGeom->setVertexArray(pVerts);

	osg::DrawElementsUByte *pIndex=new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS, 4);

	pIndex->push_back(0);
	pIndex->push_back(1);
	pIndex->push_back(2);
	pIndex->push_back(3);

	pIndex->push_back(0);
	pIndex->push_back(3);
	pIndex->push_back(5);
	pIndex->push_back(4);

	pIndex->push_back(3);
	pIndex->push_back(2);
	pIndex->push_back(6);
	pIndex->push_back(5);

	pIndex->push_back(2);
	pIndex->push_back(1);
	pIndex->push_back(7);
	pIndex->push_back(6);

	pIndex->push_back(1);
	pIndex->push_back(0);
	pIndex->push_back(4);
	pIndex->push_back(7);

	pIndex->push_back(4);
	pIndex->push_back(5);
	pIndex->push_back(6);
	pIndex->push_back(7);

	pGeom->addPrimitiveSet(pIndex);

	osg::Vec3Array *pNorms=new osg::Vec3Array();
	pNorms->push_back(osg::Vec3(0.0f, 0.0f, -1.0f));
	pNorms->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	pNorms->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	pNorms->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
	pNorms->push_back(osg::Vec3(-1.0f, 0.0f, 0.0f));
	pNorms->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	pGeom->setNormalArray(pNorms);
	pGeom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);
	
	osg::Vec4Array *pCols=new osg::Vec4Array();
	pCols->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pCols->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	pCols->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	pCols->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	pCols->push_back(osg::Vec4(1.0f, 0.0f, 1.0f, 1.0f));
	pCols->push_back(osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f));
	pCols->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pCols->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

	pGeom->setColorArray(pCols);
	pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	return pGeode;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: calculateFaceNormal
description: For each vertex on the triangle calculate direction vector then find
			 its perpendicular direction as a vector
----------------------------------------------------------------------------------*/
osg::Vec3f ShapeFactory::calculateFaceNormal( osg::Vec3f vPoint1, osg::Vec3f vPoint2, osg::Vec3f vPoint3 )
{
	osg::Vec3 vU = vPoint2 - vPoint1;
	osg::Vec3 vV = vPoint3 - vPoint1;		

	osg::Vec3 vNormal( ( vU.y() * vV.z() ) - ( vU.z() * vV.y() ),
					   ( vU.z() * vV.x() ) - ( vU.x() * vV.z() ),
					   ( vU.x() * vV.y() ) - ( vU.y() * vV.x() ) );

	return vNormal;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getWireframeSphere
description: use osg to create a sphere with parameter radius, then use the passed
		     in colour variable to set material value of State Set
----------------------------------------------------------------------------------*/
osg::Geode* ShapeFactory::getWireframeSphere( float fRadious, osg::Vec4f vColor )
{
	// make a vertex based shape
	osg::Geode* pGeode=new osg::Geode();
	osg::Geometry* pGeom=new osg::Geometry();	

	osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();

	/// Set polygon mode to GL_LINE
	osg::PolygonMode *pm = new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
	
	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	pStateSet->setAttributeAndModes( pm, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	
	osg::Material* pMaterial = new osg::Material(); 	
	pMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, vColor );	
	pMaterial->setAmbient( osg::Material::FRONT_AND_BACK, vColor );	
	pMaterial->setShininess( osg::Material::FRONT,  0.2f );

	pStateSet->setAttribute( pMaterial );

	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( osg::Vec3f(), fRadious ) ) );

	return pGeode;
}



osg::Geode* ShapeFactory::createBox1( osg::Vec3f &vHalfDim, osg::Vec4f &vColour )
{

	// make a vertex based shape
	osg::Geode* pGeode=new osg::Geode();


	osg::Geometry *pGeom=new osg::Geometry();

	pGeode->addDrawable( pGeom );

	// set the colour
	osg::Vec4Array *avColour = new osg::Vec4Array();
	avColour->push_back( vColour );
	pGeom->setColorArray( avColour );
	pGeom->setColorBinding( osg::Geometry::BIND_OVERALL );

	// define vertecies
	osg::Vec3Array *pVerts=new osg::Vec3Array();
	pVerts->push_back(osg::Vec3(-vHalfDim.x(), -vHalfDim.y(), -vHalfDim.z()));
	pVerts->push_back(osg::Vec3(-vHalfDim.x(), vHalfDim.y(), -vHalfDim.z()));
	pVerts->push_back(osg::Vec3(vHalfDim.x(), vHalfDim.y(), -vHalfDim.z()));
	pVerts->push_back(osg::Vec3(vHalfDim.x(), -vHalfDim.y(), -vHalfDim.z()));

	pVerts->push_back(osg::Vec3(-vHalfDim.x(), -vHalfDim.y(), vHalfDim.z()));
	pVerts->push_back(osg::Vec3(vHalfDim.x(), -vHalfDim.y(), vHalfDim.z()));
	pVerts->push_back(osg::Vec3(vHalfDim.x(), vHalfDim.y(), vHalfDim.z()));
	pVerts->push_back(osg::Vec3(-vHalfDim.x(), vHalfDim.y(), vHalfDim.z()));
	pGeom->setVertexArray(pVerts);

	// front
	osg::DrawElementsUByte *pFaceFront=new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS, 0);
	pFaceFront->push_back(0); 
	pFaceFront->push_back(1);
	pFaceFront->push_back(2);
	pFaceFront->push_back(3);
	pGeom->addPrimitiveSet( pFaceFront );
	// bottom
	osg::DrawElementsUByte *pFaceBottom=new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS, 0);
	pFaceBottom->push_back(0); 
	pFaceBottom->push_back(3);
	pFaceBottom->push_back(5);
	pFaceBottom->push_back(4);
	pGeom->addPrimitiveSet( pFaceBottom );
	// right
	osg::DrawElementsUByte *pFaceRight=new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS, 0);
	pFaceRight->push_back(3); 
	pFaceRight->push_back(2);
	pFaceRight->push_back(6);
	pFaceRight->push_back(5);
	pGeom->addPrimitiveSet( pFaceRight );
	// top
	osg::DrawElementsUByte *pFaceTop=new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS, 0);
	pFaceTop->push_back(2); 
	pFaceTop->push_back(1);
	pFaceTop->push_back(7);
	pFaceTop->push_back(6);
	pGeom->addPrimitiveSet( pFaceTop );
	// left
	osg::DrawElementsUByte *pFaceLeft=new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS, 0);
	pFaceLeft->push_back(1); 
	pFaceLeft->push_back(0);
	pFaceLeft->push_back(4);
	pFaceLeft->push_back(7);
	pGeom->addPrimitiveSet( pFaceLeft );
	// back
	osg::DrawElementsUByte *pFaceBack=new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS, 0);
	pFaceBack->push_back(4); 
	pFaceBack->push_back(5);
	pFaceBack->push_back(6);
	pFaceBack->push_back(7);
	pGeom->addPrimitiveSet( pFaceBack );


	// define normals
	osg::Vec3Array *pNorms=new osg::Vec3Array();
	pNorms->push_back(osg::Vec3(0.0f, 0.0f, -1.0f));
	pNorms->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	pNorms->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	pNorms->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
	pNorms->push_back(osg::Vec3(-1.0f, 0.0f, 0.0f));
	pNorms->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	pGeom->setNormalArray(pNorms);
	pGeom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);


	// define texture co-ordinates
	osg::Vec2Array *pTex = new osg::Vec2Array(8);
	(*pTex)[0].set( 0.0, 0.0 );
	(*pTex)[1].set( 0.0, 1.0 );
	(*pTex)[2].set( 1.0, 1.0 );
	(*pTex)[3].set( 1.0, 0.0 );

	(*pTex)[4].set( 1.0, 0.0 );
	(*pTex)[5].set( 0.0, 0.0 );
	(*pTex)[6].set( 0.0, 1.0 );
	(*pTex)[7].set( 1.0, 1.0 );
	pGeom->setTexCoordArray( 0, pTex );


	return pGeode;
}
