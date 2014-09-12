#include "stdafx.h"
#include "maLoadObject.h"
#include "maLinkedList.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "raaBMPReader.h"

using namespace std;


unsigned int uiVertecieCount = 0;
unsigned int uiTextureCount = 0;
unsigned int uiNormalCount = 0;
unsigned int uiFaceCount = 0;

ModelData* load_object_file( std::string sFileLocation )
{
	ModelData* pmdModelData = new ModelData();

	uiVertecieCount = 0;
	uiTextureCount = 0;
	uiNormalCount = 0;
	uiFaceCount = 0;

	float** pfVertecies;
	float** pfTextures;
	float** pfNormals;

	unsigned int uiVerteciePosition = 0;
	unsigned int uiTexturePosition = 0;
	unsigned int uiNormalPosition = 0;
	unsigned int uiFacePosition = 0;

	ifstream ifsFile( sFileLocation );

	string sLine;

	if( ifsFile.is_open() )
	{
		//cont number or vertices textures and normals
		while( ifsFile.good() )
		{
			getline( ifsFile, sLine );
			
			if( sLine.compare( 0, 2, "v " ) == 0 )
			{
				uiVertecieCount++;
			}

			if( sLine.compare( 0, 2, "vt" ) == 0 )
			{
				uiTextureCount++;
			}

			if( sLine.compare( 0, 2, "vn" ) == 0 )
			{
				uiNormalCount++;
			}

			if( sLine.compare( 0, 2, "f " ) == 0 )
			{
				uiFaceCount++;
			}
		}

		//ifsFile.clear();
		//ifsFile.seekg(0, ios::beg);

		ifsFile.close();
	}

	//build arrays
	pfVertecies = new float*[uiVertecieCount];

	for( unsigned int i = 0; i < uiVertecieCount; ++i )
	{
		pfVertecies[i] = new float[4];
	}

	pfTextures = new float*[uiTextureCount];

	for( unsigned int i = 0; i < uiTextureCount; ++i )
	{
		pfTextures[i] = new float[4];
	}

	pfNormals = new float*[uiNormalCount];

	for( unsigned int i = 0; i < uiNormalCount; ++i )
	{
		pfNormals[i] = new float[4];
	}

	//create face array now as we know its size
	pmdModelData->pmfFaces = new ModelFace[uiFaceCount];
	
	ifsFile.open( sFileLocation );

	if( ifsFile.is_open() )
	{
		//read vertices textures and normals
		while( ifsFile.good() )
		{
			getline( ifsFile, sLine );

			if( sLine.compare( 0, 2, "v " ) == 0 )
			{
				put_floats( pfVertecies[uiVerteciePosition], sLine );
				uiVerteciePosition++;
			}

			if( sLine.compare( 0, 2, "vt" ) == 0 )
			{
				put_floats( pfTextures[uiTexturePosition], sLine );
				uiTexturePosition++;
			}

			if( sLine.compare( 0, 2, "vn" ) == 0 )
			{
				put_floats( pfNormals[uiNormalPosition], sLine );
				uiNormalPosition++;
			}

			if( sLine.compare( 0, 2, "f " ) == 0 )
			{
				//build face
				build_face( &pmdModelData->pmfFaces[uiFacePosition], sLine );
				uiFacePosition++;
			}
		}
	}

	ifsFile.close();

	//create data structure
	pmdModelData->uiNoOfFaces = uiFaceCount;
	pmdModelData->pfVertexData = pfVertecies;
	pmdModelData->pfNormalData = pfNormals;
	pmdModelData->pfTextureData = pfTextures;

	cout << "\n";
	//print_face_data(  &pmdModelData->pmfFaces[0] );

	return pmdModelData;
}

int get_index_of( string line, string indexString, unsigned int uiStartPoss )
{
	int uiPoss = uiStartPoss;

	for( unsigned int i = uiStartPoss; i < line.length(); i ++ )
	{
		if( line.compare( i, indexString.length(), indexString ) ==  0 )
		{
			break;
		}

		uiPoss++;
	}
	
	return uiPoss;
}

void put_floats( float* fVals, string sLine )
{	
	//remove v vn vt
	sLine = sLine.substr( 2 );

	istringstream issStream( sLine ) ;
	string sValue;
	
	int iPoss = 0;

	while( issStream >> sValue )
	{
		if( iPoss < 3 )
		{
			fVals[iPoss] = ( float ) atof( sValue.c_str() );
			iPoss++;
		}
	}

	fVals[3] = 0.0f; 
}

void build_face( ModelFace* pface, string sFaceData )
{
	pface->pfVerticies = new int[4]; 
	pface->pfMaterialValues = new int[4]; 
	pface->pfNormals = new int[4]; 

	//remove f
	sFaceData = sFaceData.substr( 2 );

	istringstream issStream( sFaceData ) ;
	string sValue;

	int iPoss = 0;
	int noOfNormals = 0;
	int noOfTextures = 0;

	while( issStream >> sValue )
	{
		//should not proceed past 4 vertecies
		if( iPoss < 4 )
		{
			//vertex 
			int iStringPossition = 0;

			//if( uiVertecieCount != 0 )
			{
				string substring = sValue.substr( 0, get_index_of( sValue, "/", iStringPossition ) );
				pface->pfVerticies[iPoss] = atoi( substring.c_str() ) ;

 				iStringPossition += substring.length() +1;//+1 for /
			}		

			//texture 
			//if( uiTextureCount != 0 )
			{
				unsigned int iNext = get_index_of( sValue, "/",  iStringPossition );

				if( iNext != iStringPossition )
				{
					string substring = sValue.substr( iStringPossition, iNext - iStringPossition );
					pface->pfMaterialValues[iPoss] = atoi( substring.c_str() ) ;

					iStringPossition += substring.length() + 1;//+1 for /
					noOfTextures++;
				}
				else
				{
					//jump over blank
					iStringPossition++;
				}
			}

			//normal
			//if( uiNormalCount != 0 )
			{
				unsigned int iNext = get_index_of( sValue, "/",  iStringPossition );

				if( iNext != iStringPossition )
				{

					string substring = sValue.substr( iStringPossition, iNext - iStringPossition );
					pface->pfNormals[iPoss] = atoi( substring.c_str() ) ;

					iStringPossition += substring.length();//+1 for /
					noOfNormals++;
				}
				else
				{
					//jump over blank
					iStringPossition++;
				}
			}

			iPoss++;
		}
	}

	if( iPoss == 3 )
	{
		pface->pfVerticies[iPoss] = -1;
		pface->pfMaterialValues[iPoss] = -1;
		pface->pfNormals[iPoss] = -1;
	}

	pface->uiNoOfVertiies = iPoss;
	pface->uiNoOfNormals = noOfNormals;
	pface->uiNoOfMaterialValues = noOfTextures;
}

void print_face_data( ModelFace* pface )
{
	cout << " FACE : ";
	cout << "Vertecies: " << pface->pfVerticies[0] << " " << pface->pfVerticies[1] << " " <<  pface->pfVerticies[2] << " " << pface->pfVerticies[3];
	cout << "\n";
}

GLuint load_texture( string sFileName, unsigned int uiWidth, unsigned int uiHeight )
{	
	void* pCBmp = loadBitmap( sFileName.c_str() , uiWidth, uiHeight );

	GLuint guiTextureID;
	glGenTextures( 1, &guiTextureID );
	glBindTexture( GL_TEXTURE_2D, guiTextureID );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, uiWidth, uiHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pCBmp );
	
	//clean up pixel data;
	delete pCBmp;
	pCBmp = 0;

	return guiTextureID;
}