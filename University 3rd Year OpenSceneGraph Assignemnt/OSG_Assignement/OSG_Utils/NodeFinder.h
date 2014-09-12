#ifndef _NodeFinder_
#define _NodeFinder_

#include "stdafx.h"

using namespace std;

template<class T> class __declspec(dllexport) NodeFinder : public osg::NodeVisitor
{

public:

	NodeFinder( osg::NodeVisitor::TraversalMode mode ) : osg::NodeVisitor( mode )
	{
		_pvNodes = new vector<T*>();
	}

	virtual ~NodeFinder()
	{
		for( vector<T*>::iterator i = _pvNodes->begin(); i != _pvNodes->end(); i++ )
		{
			( *i )->unref();
		}

		if( _pvNodes ) delete _pvNodes;
	}

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: writeScene
	description: 1. if node can be cases to template type add it to list
	----------------------------------------------------------------------------------*/
	virtual void apply( osg::Node& node )
	{
		string tmp = node.getName();

		//T* pCasterNode = dynamic_cast<T*>( &node );
		T* pCasterNode = ( T* ) &node;

		if( pCasterNode != NULL )
		{
			addNode( pCasterNode );
		}

		traverse( node );
	}

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: getFirst
	description: 1. return first node
	----------------------------------------------------------------------------------*/
	T* getFirst()
	{
		if( _pvNodes->size() == 0 ) return NULL;
		else return ( *_pvNodes )[0];
	}

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: getList
	description: 1. return vector list of nodes
	----------------------------------------------------------------------------------*/
	vector<T*>* getList()
	{
		return _pvNodes;
	}


protected:

	vector<T*>* _pvNodes;

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: addNode
	description: 1. add node to list and reference it
	----------------------------------------------------------------------------------*/
	void addNode( T* pNode )
	{
		_pvNodes->push_back( pNode );
		pNode->ref();
	}
};

#endif