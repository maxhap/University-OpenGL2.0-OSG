#include "stdafx.h"
#include "maSceneIO.h"
#include "maShapes.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void save_shape( float x, float y, float z, float fObjectType, ofstream& ofStream );
void save_object( float x, float y, float z, float fObjectType, string strFileLocation, ofstream& ofStream );
void save_textured_shape( float x, float y, float z, float fObjectType, unsigned int uiWidth, unsigned int uiHeight, string fileLocation, ofstream& ofStream );
void create_object( maStructList& list, unsigned int uiType, float x, float y, float z, string strObjFileLoation, unsigned int uiWidth, unsigned int uiHeight );

/*--------------------------------------------
Author: Max Ashton
Description: Write object data to file based on rules
			Rules: 1. ------- indicates new object, this must be at the start of all objects
				   2. depending on data type call relevant save function passing the current file buffer 
----------------------------------------------*/
void save_scene( maStructList& list, std::string strFileLocation )
{
	if( strFileLocation.compare( strFileLocation.length() - 4, 4, ".scn" ) != 0 )
	{
		strFileLocation = strFileLocation + ".scn";
	}

	ofstream ofsFile;
	ofsFile.open ( strFileLocation );

	//iterate through list setting selected objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	while( bNotEndOfList )
	{
		ofsFile << "-----------------------------------------------------" << "\n";
		
		unsigned int uiType;
		float x = 0;
		float y = 0;
		float z = 0;
		string strObjFileLoation;
		unsigned int uiWidth = 0;
		unsigned int uiHeight = 0;

		if( pCurrentNode->dataType == OBJECT ) 
		{
			uiType = ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->iObjectType;
			x = ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->pfPossitionData[0];
			y = ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->pfPossitionData[1];
			z = ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->pfPossitionData[2];			
			strObjFileLoation = ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->strFileLocation;
			uiWidth = ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->uiWidth;
			uiHeight = ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->uiHeight;
		}
		else
		{
			uiType = ( ( ModelData* ) pCurrentNode->_pvData )->basicData->iObjectType;
			x = ( ( ModelData* ) pCurrentNode->_pvData )->basicData->pfPossitionData[0];
			y = ( ( ModelData* ) pCurrentNode->_pvData )->basicData->pfPossitionData[1];
			z = ( ( ModelData* ) pCurrentNode->_pvData )->basicData->pfPossitionData[2];			
			strObjFileLoation = ( ( ModelData* ) pCurrentNode->_pvData )->basicData->strFileLocation;
		}

		if( uiType != TEXTUREDQUAD && uiType != MODEL )
		{
			save_shape( x, y, z, uiType, ofsFile );
		}
		else if( uiType == MODEL )
		{
			save_object( x, y, x, uiType, strObjFileLoation, ofsFile );
		}
		else
		{
			save_textured_shape( x, y, z, uiType, uiWidth, uiHeight, strObjFileLoation, ofsFile );
		}

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}

	ofsFile << "-----------------------------------------------------" << "\n";

	ofsFile.close();
}

/*--------------------------------------------
Author: Max Ashton
Description: save attributes for a basic shape using meaningfully identifiers followed by an =
----------------------------------------------*/
void save_shape( float x, float y, float z, float fObjectType, ofstream& ofStream )
{
	ofStream << "objecttype=" <<  fObjectType << "\n";
	ofStream << "posx=" << x << "\n";
	ofStream << "posy=" << y << "\n";
	ofStream << "posz=" << z << "\n";
}

/*--------------------------------------------
Author: Max Ashton
Description: save attributes for an object using meaningfully identifiers followed by an = ( basic shape shares similar data so call save_shape also )
----------------------------------------------*/
void save_object( float x, float y, float z, float fObjectType, string strFileLocation, ofstream& ofStream )
{
	save_shape( x, y, z, fObjectType, ofStream );
	ofStream << "filelocation=" << strFileLocation << "\n";
}

/*--------------------------------------------
Author: Max Ashton
Description: save attributes for an object using meaningfully identifiers followed by an = ( object shares similar data so call save_shape also )
----------------------------------------------*/
void save_textured_shape( float x, float y, float z, float fObjectType, unsigned int uiWidth, unsigned int uiHeight, string fileLocation, ofstream& ofStream )
{
	save_object( x, y, z, fObjectType, fileLocation, ofStream );
	ofStream << "width=" << uiWidth << "\n";
	ofStream << "height=" << uiHeight << "\n";
}

/*--------------------------------------------
Author: Max Ashton
Description: Cycle though the file and build objects based on rules
			Rules: 1. if ---- found then new object detected so create object based on already created data then start fresh
				   2. using object data type, correct function to create object is called
				   3. Using string library to compare string to gather data 
----------------------------------------------*/
void load_scene( maStructList& list, std::string strFileLocation )
{
	ifstream ifsFile( strFileLocation );

	string sLine;

	if( ifsFile.is_open() )
	{
		bool firstObject = true;

		unsigned int uiType = 0;
		float x = 0;
		float y = 0;
		float z = 0;
		string strObjFileLoation = "";
		unsigned int uiWidth = 0;
		unsigned int uiHeight = 0;


		while( ifsFile.good() )
		{
			getline( ifsFile, sLine );

			//new object

			if( sLine.compare( 0, 5, "-----" ) == 0 )
			{
				if( !firstObject )
				{
					//write data if not first
					create_object( list, uiType, x, y, z, strObjFileLoation, uiWidth, uiHeight );
				}

				firstObject = false;
			}

			if( sLine.compare( 0, 10, "objecttype" ) == 0 )
			{
				uiType = ( unsigned int ) atoi( sLine.substr( 11 ).c_str() );
			}

			if( sLine.compare( 0, 4, "posx" ) == 0 )
			{
				x = atof( sLine.substr( 5 ).c_str() );
			}

			if( sLine.compare( 0, 4, "posy" ) == 0 )
			{
				y = atof( sLine.substr( 5 ).c_str() );
			}

			if( sLine.compare( 0, 4, "posy" ) == 0 )
			{
				z = atof( sLine.substr( 5 ).c_str() );
			}

			if( sLine.compare( 0, 12, "filelocation" ) == 0 )
			{
				strObjFileLoation = sLine.substr( 13 );
			}

			if( sLine.compare( 0, 5, "width" ) == 0 )
			{
				uiWidth = ( unsigned int ) atoi( sLine.substr( 6 ).c_str() );
			}

			if( sLine.compare( 0, 6, "height" ) == 0 )
			{
				uiType = ( unsigned int ) atoi( sLine.substr( 7 ).c_str() );
			}

		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: call correct function by using the stored object type as the function identifier
----------------------------------------------*/
void create_object( maStructList& list, unsigned int uiType, float x, float y, float z, string strObjFileLoation, unsigned int uiWidth, unsigned int uiHeight )
{
	switch( uiType )
	{
		case TRIANGLE: add_triangle( list, x, y, z );break;
		case LITTEAPOT: add_lit_teapot( list, x, y, z );break;
		case LITSPHERE: add_lit_sphere( list, x, y, z );break;
		case POSSLIGHT: add_possitioned_light( list, x, y, z );break;
		case MODEL: add_model( list, x, y, z, strObjFileLoation );break;	
		case TEXTUREDQUAD: add_smiley_quad( list, x, y, z, strObjFileLoation, uiWidth, uiHeight );break;
		case CLIENTCUBE: add_client_cube( list, x, y, z ); break;
	}
}