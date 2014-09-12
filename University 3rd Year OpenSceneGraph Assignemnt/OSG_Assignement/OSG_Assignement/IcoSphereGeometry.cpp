#include "stdafx.h"
#include "IcoSphereGeometry.h"

IcoSphereGeometry::IcoSphereGeometry( unsigned int uiRecursionLevel )
{
	_uiRecursionLevel = uiRecursionLevel;
	_uiIndex = 0;

	_pvVertexs = new std::vector<osg::Vec3f*>();
	_pvIndexCash = new std::map<string, unsigned int>();
	_pvTriangleIndexes = new std::vector<osg::Vec3f*>();	

	createPoints();
}

IcoSphereGeometry::~IcoSphereGeometry()
{
	if( _pvVertexs ) delete _pvVertexs;
	if( _pvIndexCash ) delete _pvIndexCash;
	if( _pvTriangleIndexes ) delete _pvTriangleIndexes;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: createPoints
Description: create the initial points using 4 squares each set perpendiculars to each
		     other
----------------------------------------------------------------------------------*/
void IcoSphereGeometry::createPoints()
{
	// create 12 vertices of a icosahedron
	float ft = ( 1.0 + sqrt( 5.0 ) ) / 2.0;

	addVertex( new osg::Vec3f( -1.0f,  ft,  0.0f ) );
	addVertex( new osg::Vec3f( 1.0f,  ft, 0.0f ) );
	addVertex( new osg::Vec3f( -1.0f, -ft,  0.0f ) );
	addVertex( new osg::Vec3f( 1.0f, -ft,  0.0f ) );

	addVertex( new osg::Vec3f( 0.0f, -1.0f,  ft ) );
	addVertex( new osg::Vec3f( 0.0f,  1.0f,  ft ) );
	addVertex( new osg::Vec3f( 0.0f, -1.0f, -ft ) );
	addVertex( new osg::Vec3f( 0.0f,  1.0f, -ft ) );

	addVertex( new osg::Vec3f( ft,  0.0f, -1.0f ) );
	addVertex( new osg::Vec3f( ft,  0.0f,  1.0f ) );
	addVertex( new osg::Vec3f( -ft,  0.0f, -1.0f ) );
	addVertex( new osg::Vec3f( -ft,  0.0f,  1.0f ) );

	// 5 faces around point 0
	_pvTriangleIndexes->push_back( new osg::Vec3f( 0, 11, 5 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 0, 5, 1 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 0, 1, 7 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 0, 7, 10) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 0, 10, 11 ) );

	// 5 adjacent faces
	_pvTriangleIndexes->push_back( new osg::Vec3f( 1, 5, 9 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 5, 11, 4 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 11, 10, 2 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 10, 7, 6 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 7, 1, 8 ) );

	// 5 faces around point 3
	_pvTriangleIndexes->push_back( new osg::Vec3f( 3, 9, 4 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 3, 4, 2 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 3, 2, 6 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 3, 6, 8 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 3, 8, 9 ) );

	// 5 adjacent faces
	_pvTriangleIndexes->push_back( new osg::Vec3f( 4, 9, 5 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 2, 4, 11 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 6, 2, 10 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 8, 6, 7 ) );
	_pvTriangleIndexes->push_back( new osg::Vec3f( 9, 8, 1 ) );

	// refine triangles
	for ( unsigned int i = 0; i < _uiRecursionLevel; i++ )
	{
		std::vector<osg::Vec3f*>* pvNewFaces = new std::vector<osg::Vec3f*>();
	
		for( std::vector<osg::Vec3f*>::iterator i = _pvTriangleIndexes->begin(); i != _pvTriangleIndexes->end(); i++ )
		{
			// replace triangle by 4 triangles
			int a = getMiddlePoint( ( *i )->x(), ( *i )->y() );				
			int b = getMiddlePoint( ( *i )->y() , ( *i )->z() );
			int c = getMiddlePoint( ( *i )->z() , ( *i )->x() );

			pvNewFaces->push_back( new osg::Vec3f( ( *i )->x(), a, c ) );
			pvNewFaces->push_back( new osg::Vec3f( ( *i )->y(), b, a ) );
			pvNewFaces->push_back( new osg::Vec3f( ( *i )->z(), c, b ) );
			pvNewFaces->push_back( new osg::Vec3f( a, b, c ) );
		}

		_pvTriangleIndexes->empty();
		delete _pvTriangleIndexes;
		_pvTriangleIndexes = 0;
		_pvTriangleIndexes = pvNewFaces;
	}
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addVertex
Description: add vertex to mesh, fix position to be on unit sphere, return index	
----------------------------------------------------------------------------------*/
unsigned int IcoSphereGeometry::addVertex( osg::Vec3f* v3Point )
{
	// add vertex to mesh, fix position to be on unit sphere, return index	
	
	float fLength = sqrt( v3Point->x() * v3Point->x() + v3Point->y() * v3Point->y() + v3Point->z() * v3Point->z() );
	
	osg::Vec3f* vNormalized = new osg::Vec3f( v3Point->x() / fLength, v3Point->y() / fLength, v3Point->z() / fLength );
	
	_pvVertexs->push_back( vNormalized );

	unsigned int uiRet = _uiIndex;

	_uiIndex++;

	delete v3Point;

	return uiRet;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getMiddlePoint
Description: used when splitting one triangle into three, find the middle point and
			 then add it to the vertex list. return new points index, cashing
			 vector used to reused points on shared faces
----------------------------------------------------------------------------------*/
unsigned int IcoSphereGeometry::getMiddlePoint( unsigned int uiFirstIndex, unsigned int uiSecondIndex )
{
	bool bFirstSmaller = uiFirstIndex < uiSecondIndex;
	unsigned int iSmallerIndex = ( bFirstSmaller ) ? uiFirstIndex : uiSecondIndex;
	unsigned int iGreaterIndex = ( bFirstSmaller ) ? uiSecondIndex : uiFirstIndex;

	ostringstream stringStream;
	stringStream << iSmallerIndex << '-' << iGreaterIndex;

	string ikey = stringStream.str();

	map<string, unsigned int>::iterator fi = _pvIndexCash->find( ikey );

	if( fi != _pvIndexCash->end() )
	{
		return ( *_pvIndexCash )[ikey];
	}

	// not in cache, calculate it
	osg::Vec3f* vFirst = ( *_pvVertexs )[uiFirstIndex];
	osg::Vec3f* vSecond = ( *_pvVertexs )[uiSecondIndex];
	osg::Vec3f* vHalfPoint = new osg::Vec3f( ( ( vFirst->x() + vSecond->x() ) / 2.0f ), 
									    ( ( vFirst->y() + vSecond->y() ) / 2.0f ),
									    ( ( vFirst->z() + vSecond->z() ) / 2.0f ) ); 

	unsigned int uiIndex = addVertex( vHalfPoint );

	//store in cash
	( *_pvIndexCash )[ikey] = uiIndex;

	return uiIndex;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getVertexs
Description: return vector list of vertexes	
----------------------------------------------------------------------------------*/
std::vector<osg::Vec3f*>* IcoSphereGeometry::getVertexs()
{
	return _pvVertexs;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getTriangleIndexes
Description:  return vector list of indexes 	
----------------------------------------------------------------------------------*/
std::vector<osg::Vec3f*>* IcoSphereGeometry::getTriangleIndexes()
{
	return _pvTriangleIndexes;
}

