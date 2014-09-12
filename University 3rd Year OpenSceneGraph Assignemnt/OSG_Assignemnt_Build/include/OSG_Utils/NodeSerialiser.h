#ifndef _NodeSerialiser_
#define _NodeSerialiser_

#include "stdafx.h"
#include "DataHolder.h"

using namespace std;

class __declspec(dllexport) NodeSerialiser : public osg::NodeVisitor
{

public:


	NodeSerialiser( osg::NodeVisitor::TraversalMode mode  ) : osg::NodeVisitor( mode )
	{			
	}

	virtual ~NodeSerialiser()
	{
	}

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: apply
	description: if the node has user data serialise itself to file stream embedded in node tag
	----------------------------------------------------------------------------------*/
	virtual void apply( osg::Node& node )
	{
		string tmp = node.getName();		

		DataHolder* pDataHolder = ( DataHolder* ) node.getUserData();

		if( pDataHolder != NULL )
		{
			_ssSerialisationXML << "<node>" << "\n";
			_ssSerialisationXML << "<name>" << node.getName() << "</name>" << "\n";
			pDataHolder->serialise( _ssSerialisationXML );
			_ssSerialisationXML << "</node>" << "\n";
		}		

		traverse( node );
	}

	string getXMLString()
	{
		return _ssSerialisationXML.str();
	}


protected:

	stringstream _ssSerialisationXML;

};

#endif