#ifndef _KeyboardEventHandler_
#define _KeyboardEventHandler_

#include "stdafx.h"

#include <osgGA/GUIEventHandler>

#include "LogicEngine.h"
#include "SceneManipulator.h"

class KeyboardEventHandler : public osgGA::GUIEventHandler
{

public:

	KeyboardEventHandler();
	virtual ~KeyboardEventHandler();

	virtual bool handle( const osgGA::GUIEventAdapter &eventAdapter, osgGA::GUIActionAdapter &actionAdapter, osg::Object *, osg::NodeVisitor * );

protected:
	
	unsigned int _uiEventsFired;

	bool bButtonDown;

};

#endif