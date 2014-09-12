#include "stdafx.h"
#include "ModelPrinter.h"

ModelPrinter::ModelPrinter()
{

}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: printToFile
description: throw away class to print out robot roation nodes
----------------------------------------------------------------------------------*/
void ModelPrinter::printToFile( string sFilePath, osg::Node* pTopNode )
{
	ofstream ofFile( sFilePath );

	if( ofFile.is_open() )
	{
		ofFile << "----Model Object---" << "\n" << "\n";

		NodeFinder<osg::Node> nodeFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );

		nodeFinder.traverse( *pTopNode );

		vector<osg::Node*>* pvFindList = nodeFinder.getList();

		for( vector<osg::Node*>::iterator i = pvFindList->begin(); i != pvFindList->end(); i++ )
		{
			string sName = ( *i )->getName();

			if( sName.length() > 8 )
			{
				//if( sName.compare( sName.length() - 8, 8, "_Rotator" ) == 0 )
				{
					ofFile << "Rotation_Node: " << ( *i )->getName() << "\n"; 
					//ofFile << "XUL: " << "\n";
					//ofFile << "XLL: " << "\n";
					//ofFile << "YUL: " << "\n";
					//ofFile << "YLL: " << "\n";
				}
			}
		}
	}
}
