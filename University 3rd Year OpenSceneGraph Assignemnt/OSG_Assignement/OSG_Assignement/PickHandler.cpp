#include "stdafx.h"
#include "PickHandler.h"

PickHandler::PickHandler() 
{

}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: handle
Description: if action event was a push (mouse click) call pick
----------------------------------------------------------------------------------*/
bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
	{
		case( osgGA::GUIEventAdapter::PUSH ):
		{
			osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
			if (view) pick(view,ea);
			return false;
		}    
		
		default:
		
		return false;
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: pick
Description: use LineSegmentIntersector it determine SG nodes under the mouse, if the 
			 node name starts with hud_ call LogicEngine::instance().setHUDItem() else
			 call LogicEngine::instance().selectObject() for object selection
----------------------------------------------------------------------------------*/
void PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
{
	osgUtil::LineSegmentIntersector::Intersections intersections;

	float x = ea.getX();
	float y = ea.getY();

	if ( view->computeIntersections( x, y, intersections ) )
	{
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end();	++hitr)
		{			
			if ( !hitr->nodePath.empty() && !hitr->nodePath.back()->getName().empty() )
			{
				cout << "Node clicked: " << hitr->nodePath.back()->getName() << "\n";
				osg::Node* pnClicked = hitr->nodePath.back();
					
				if( pnClicked != NULL )
				{
					if( hitr->nodePath.back()->getName().compare( 0, 4, "hud_" ) == 0 )
					{
						LogicEngine::instance().setHUDItem( hitr->nodePath.back()->getName() );
					}
					else //if( pnClicked->getName().compare( 0, 9, "Geometry:" ) == 0 ) 
					{
						LogicEngine::instance().selectObject( pnClicked );
					}
				}
			}
		}
	}
}
