#include "stdafx.h"
#include "KeyboardEventHandler.h"

KeyboardEventHandler::KeyboardEventHandler()
{
	_uiEventsFired = 0;
	bButtonDown = false;
}

KeyboardEventHandler::~KeyboardEventHandler()
{

}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: handle
Description: 1. class scope bool is used so only one key can be pressed at once
			 2. switch based osgGA::GUIEventAdapter::KEY constanse used to determin
				action based on key
----------------------------------------------------------------------------------*/
bool KeyboardEventHandler::handle( const osgGA::GUIEventAdapter &eventAdapter, osgGA::GUIActionAdapter &actionAdapter, osg::Object *, osg::NodeVisitor * )
{
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*> ( actionAdapter.asView() );

	if( pViewer && eventAdapter.getEventType() == osgGA::GUIEventAdapter::KEYDOWN && !bButtonDown )
	{	
		bButtonDown = true;
		bool bActualEvent = false;
		
		ostringstream stringStream;
		stringStream << _uiEventsFired;

		//cout << eventAdapter.getKey() << "\n";

		switch( eventAdapter.getKey() )
		{
			//fkeys
			case osgGA::GUIEventAdapter::KEY_F1: LogicEngine::instance().addObjectToScene( SceneManipulator::SCENEFILE, "scenefile" + stringStream.str() ); bActualEvent = true; break;			
			case osgGA::GUIEventAdapter::KEY_F2: LogicEngine::instance().addObjectToScene( SceneManipulator::SPHERE, "sphere" + stringStream.str() ); bActualEvent = true; break;
			case osgGA::GUIEventAdapter::KEY_F3: LogicEngine::instance().addObjectToScene( SceneManipulator::PHYRAMID, "pyramid" + stringStream.str() ); bActualEvent = true; break;
			case osgGA::GUIEventAdapter::KEY_F4: LogicEngine::instance().addObjectToScene( SceneManipulator::RECTANGLE, "rectangle" + stringStream.str() ); bActualEvent = true; break;
			case osgGA::GUIEventAdapter::KEY_F5: LogicEngine::instance().addObjectToScene( SceneManipulator::BOX, "box" + stringStream.str() ); bActualEvent = true; break;	

			//arrow keys
			case osgGA::GUIEventAdapter::KEY_Left: LogicEngine::instance().setSelectedObjectsToMove( LogicEngine::DIRECTION_LEFT ); break;
			case osgGA::GUIEventAdapter::KEY_Up: LogicEngine::instance().setSelectedObjectsToMove( LogicEngine::DIRECTION_UP ); break;
			case osgGA::GUIEventAdapter::KEY_Right: LogicEngine::instance().setSelectedObjectsToMove( LogicEngine::DIRECTION_RIGHT ); break;
			case osgGA::GUIEventAdapter::KEY_Down: LogicEngine::instance().setSelectedObjectsToMove( LogicEngine::DIRECTION_DOWN ); break;
			
			//tab
			case osgGA::GUIEventAdapter::KEY_Tab: LogicEngine::instance().getRobot()->incrementSelection(); break;		

			//letter keys
			case osgGA::GUIEventAdapter::KEY_P: LogicEngine::instance().printSeleciton();break;
		}

		if( bActualEvent ) _uiEventsFired++;
	}

	else if( pViewer && eventAdapter.getEventType() == osgGA::GUIEventAdapter::KEYUP && bButtonDown )
	{	
		bButtonDown = false;

		bool bActualEvent = false;
		
		ostringstream stringStream;
		stringStream << _uiEventsFired;		

		switch( eventAdapter.getKey() )
		{
			//arrow keys
			case 65361: LogicEngine::instance().setSelectedObjectsToStop();
			case 65362: LogicEngine::instance().setSelectedObjectsToStop();
			case 65363: LogicEngine::instance().setSelectedObjectsToStop();
			case 65364: LogicEngine::instance().setSelectedObjectsToStop();
		}

		if( bActualEvent ) _uiEventsFired++;
	}

	return false;
}
