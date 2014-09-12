#ifndef _HUDITEMCALLBACK_
#define  _HUDITEMCALLBACK_

#include "stdafx.h"
#include <osg/NodeCallback>
#include "HUDItemDataHolder.h"

class HUDItemCallBack : public osg::NodeCallback
{

public:

	virtual void operator() ( osg::Node* node, osg::NodeVisitor* nv );

protected:

};

#endif