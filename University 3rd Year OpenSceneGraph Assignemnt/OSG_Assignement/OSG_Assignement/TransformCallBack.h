#ifndef _TRANSFORMCALLBACK_
#define _TRANSFORMCALLBACK_

#include "stdafx.h"
#include <osg/NodeCallback>

class TransformCallBack : public osg::NodeCallback
{

public: 
	
	virtual void operator() ( osg::Node* node, osg::NodeVisitor* nv );		
};

#endif