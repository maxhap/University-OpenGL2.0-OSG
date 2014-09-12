#include "stdafx.h"
#include "TransformCallBack.h"

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: operator()
Description: if there is TransformDataHolder data call data->update()
----------------------------------------------------------------------------------*/
void TransformCallBack::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	osg::ref_ptr<TransformDataHolder> pUserData = dynamic_cast<TransformDataHolder*> ( node->getUserData() );

	if( pUserData )
	{
		pUserData->update();
	}

	traverse( node, nv );
}
