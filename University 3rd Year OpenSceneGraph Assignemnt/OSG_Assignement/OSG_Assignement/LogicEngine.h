#ifndef _LOGICENGINE_
#define _LOGICENGINE_

#include "stdafx.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
#include <osgUtil/UpdateVisitor>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Drawable>
#include <osg/Matrixf>

#include <osgManipulator/Dragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TranslateAxisDragger>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <OSG_Utils/FileFinder.h>
#include <OSG_Utils/NodeFinder.h>
#include <OSG_Utils/NodeNameFinder.h>
#include <OSG_Utils/NodePrinter.h>
#include <OSG_Utils/ModelPrinter.h>
#include <OSG_Utils/SceneIO.h>
#include <OSG_Utils/NodeSerialiser.h>
#include <OSG_Utils/DataHolder.h>

#include "KeyboardEventHandler.h"
#include "ShapeFactory.h"
#include "HUD.h"
#include "PickHandler.h"
#include "SceneManipulator.h"
#include "ModelRobot.h"
#include "GroupDataHolder.h"



using namespace std;

class HUD;
class ModelRobot;

class LogicEngine
{
	LogicEngine( const LogicEngine& );
	LogicEngine& operator = ( const LogicEngine& );

public:

	static const unsigned int CONSOL_IN = 1;

	unsigned int SCREENWIDTH;// = 1680;
	unsigned int SCREENHEIGHT;// = 945;

	//selection states
	static const unsigned int MOVE = 0;
	static const unsigned int ROTATE = 1;	
	static const unsigned int SCALE = 2;
	static const unsigned int ROBOT = 3;

	//load states
	static const unsigned int LS_WITH_TEXTURES = 100;
	static const unsigned int LS_WITHOUT_TEXTURES = 101;

	//Move Direction
	static const unsigned int DIRECTION_UP = 201;
	static const unsigned int DIRECTION_DOWN = 202;
	static const unsigned int DIRECTION_LEFT = 203;
	static const unsigned int DIRECTION_RIGHT = 204;

	virtual ~LogicEngine();

	static LogicEngine& instance()
	{
		static LogicEngine instance;
		return instance;
	};

	bool init();
	
	int osgLoop();

	ModelRobot* getRobot();

	void addObjectToScene( unsigned int uiObject, string sName );	
	void setHUDItem( string sName );
	void parseAruguments( int argc, char* argv[] );

	//selection
	void selectObject( osg::Node* pNode );
	void selectAllObjects();
	void deselectAllObjects();
	void updateAllManipulators();
	void setNewManipulators();
	void turnOffManipulators();
	void updateModifiresWithModel( osg::MatrixTransform* pMatrix );	
	void printSeleciton();
	void removeSelected();

	//movement
	void setSelectedObjectsToMove( unsigned int uiDirection );
	void setSelectedObjectsToStop();

	//inpu
	static string getFileNameFromConsol();
	static string getFileNameFromConsolSave();

protected:

	LogicEngine();

	osg::Group* _pRootSceneGroup;
	osgViewer::Viewer _OSGViewer;
	osg::Camera* _pCamera;

	HUD* _pHud;

	ModelRobot* _pRobot;

	unsigned int _uiSelectionState;
	unsigned int _uiLoadState;
	
	int _iCurrentCameraPosition;

	//selection variable
	vector<osg::Group*>* _pvSelectedGroups;
	vector<osg::Matrixf> _vCameraMatrixes;	

	//setup
	void setUpViewer();
	void addHandlers();		

	void saveSelectedToFile();
	void saveObjects();
	void writeSavedObjectCout( int iNodeCount );
	void writeMySceneFile( int iStartingCount, int iNodesWrote, vector<string> vsTextures );
	void highlightSelected();
	void unhighlightSelected();

	int getSavedObjectCount();
	

	void loadSceneFile();

	unsigned int determinMovement( unsigned int uiDirection );

	string getXMLLocation( bool bOverwrite );

	void loadPrefs();

};
#endif