#ifndef _NODEPRINTER_
#define _NODEPRINTER_

#include "stdafx.h"
#include "NodeFinder.h"

using namespace std;

template<class T> class __declspec(dllexport) NodePrinter : public NodeFinder<T>
{

public:

	NodePrinter( osg::NodeVisitor::TraversalMode mode ) : NodeFinder<T>( mode )
	{
		pLastNode = NULL;
		_uiLevel = 0;

		_ofStream.open( "..\\data\\printout.txt" );
	}

	virtual ~NodePrinter()
	{
		_ofStream.close();
	}

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: determinLevel
	description: 1. print out node name with - indicating level
	----------------------------------------------------------------------------------*/
	virtual void apply( osg::Node &node )
	{
		
		for( unsigned int i = 0; i < _uiLevel; i++ )
		{
			cout << "-";

			_ofStream << "-";
		}

		cout << node.getName() << "\n";	
		_ofStream << node.getName() << "\n";	

		determinLevel( node );
		pLastNode = &node;

		NodeFinder<T>::apply( node );		
	}

private:

	osg::Node* pLastNode;

	unsigned int _uiLevel;

	ofstream _ofStream;

	/*----------------------------------------------------------------------------------
	Author: MaxAshton
	function: determinLevel
	description: 1. if last node is a parent of current then increase level
				 2. if last node is a child of current node then decrease level
	----------------------------------------------------------------------------------*/
	void determinLevel( osg::Node &node )
	{
		if( pLastNode == NULL ) return;
		//if( node.getName().compare( "" ) != 0 || pLastNode->getName().compare( "" ) == 0) return;		
		
		osg::Node::ParentList nlParents = node.getParents();		

		osg::Node::ParentList::iterator i = find( nlParents.begin(), nlParents.end(), pLastNode );

		if( i != nlParents.end() )
		{
			_uiLevel++;
			return;
		}

		osg::Group* pCurrentNodeAsGroup = dynamic_cast<osg::Group*>( &node );

		if( pCurrentNodeAsGroup )
		{
			for( unsigned int i = 0; i < pCurrentNodeAsGroup->getNumChildren(); i++ )
			{				
				osg::Node* pNode = pCurrentNodeAsGroup->getChild( i );
				
				if( pNode )
				{
					if( pCurrentNodeAsGroup == &node )
					{
						//if 0 root node being checked against itself
						if( _uiLevel > 0 ) 
						{	
							_uiLevel--;
						}

						
						return;
					}
				}				
			}

			
		}
		
	}
};

#endif