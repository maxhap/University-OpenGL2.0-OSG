#include "stdafx.h"
#include "SceneIO.h"

SceneIO::SceneIO()
{

}

SceneIO::~SceneIO()
{

}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: writeScene
description: use OSG to write an .osg file
----------------------------------------------------------------------------------*/
void SceneIO::writeScene( osg::Node* pRoot, string sFilePath )
{	
	
	osgDB::ReaderWriter* pReadWriter =  osgDB::Registry::instance()->getReaderWriterForExtension( "osg" );	
	osgDB::ReaderWriter::Options::Options* option = new osgDB::ReaderWriter::Options::Options();			

	ofstream ofFile;
	ofFile.open( sFilePath );

	if( ofFile )
	{			
		pReadWriter->writeNode( *pRoot, ofFile, option );
	}

	ofFile.close();
}
