#ifndef _NodeNameFinder_
#define _NodeNameFinder_

#include "stdafx.h"
#include "NodeFinder.h"


using namespace std;

template<class T> class __declspec(dllexport) NodeNameFinder : public NodeFinder<T>
{

public:

	NodeNameFinder( osg::NodeVisitor::TraversalMode mode, string sName ): NodeFinder<T>( mode )
	{
		_sName = sName;	
	}

	virtual ~NodeNameFinder()
	{

	}

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: apply
	description: 1. if node name matches call superclass apply
	----------------------------------------------------------------------------------*/
	virtual void apply( osg::Node &node )
	{
		if( node.getName() == _sName)
		{
			NodeFinder<T>::apply( node );
		}
		else
		{
			traverse( node );
		}	
	}

protected:

	string _sName;	
};

#endif