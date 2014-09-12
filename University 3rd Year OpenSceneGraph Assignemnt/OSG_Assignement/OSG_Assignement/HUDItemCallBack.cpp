#include "stdafx.h"
#include "HUDItemCallBack.h"

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: operator()
Description: if there is HUDItemDataHolder data call data->update()
----------------------------------------------------------------------------------*/
void HUDItemCallBack::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	osg::ref_ptr<HUDItemDataHolder> pUserData = dynamic_cast<HUDItemDataHolder*> ( node->getUserData() );
	
	if( pUserData )
	{
		pUserData->update();
	}

	traverse( node, nv );
}
