#include "stdafx.h"
#include "LogicEngine.h"

LogicEngine::LogicEngine()
{
	_pRootSceneGroup = new osg::Group();
	_pvSelectedGroups = new vector<osg::Group*>();	
	_uiLoadState = LS_WITHOUT_TEXTURES;
	_iCurrentCameraPosition = 0;

	loadPrefs();
}

LogicEngine::~LogicEngine()
{
	if( _pHud ) _pHud->unref();

	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		( *i )->unref();
	}

	if( _pvSelectedGroups ) delete _pvSelectedGroups;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setUpViewer
Description: Sets up the GraphicsRendering context using SCREENWIDTH and SCREENHeight
			 static variables. Also add simple camera trackers to the camera
----------------------------------------------------------------------------------*/
void LogicEngine::setUpViewer()
{
	//traits
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits();	
	pTraits->x = 20;
	pTraits->y = 20;
	pTraits->width = SCREENWIDTH;
	pTraits->height = SCREENHEIGHT;
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	pTraits->sharedContext = 0;

	//graphics context
	osg::GraphicsContext* pGraphicsContext = osg::GraphicsContext::createGraphicsContext( pTraits );

	osgGA::KeySwitchMatrixManipulator* pKeyswitchManipulator = new osgGA::KeySwitchMatrixManipulator();
	pKeyswitchManipulator->addMatrixManipulator( '1', "FirstPerson", new osgGA::FirstPersonManipulator() );
	pKeyswitchManipulator->addMatrixManipulator( '2', "Trackball", new osgGA::TrackballManipulator() );
	pKeyswitchManipulator->addMatrixManipulator( '3', "Flight", new osgGA::FlightManipulator() );
	pKeyswitchManipulator->addMatrixManipulator( '4', "Drive", new osgGA::DriveManipulator() );
	
	_OSGViewer.setCameraManipulator( pKeyswitchManipulator );

	_pCamera = _OSGViewer.getCamera();

	_pCamera->setGraphicsContext( pGraphicsContext );
	_pCamera->setViewport( new osg::Viewport( 0, 0, pTraits->width, pTraits->height ) );
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addHandlers
Description: Adds Relevant keyboard, pick and view graphics context handlers to 
			 the osg viewer
----------------------------------------------------------------------------------*/
void LogicEngine::addHandlers()
{
	_OSGViewer.addEventHandler( new KeyboardEventHandler );

	_OSGViewer.addEventHandler( new PickHandler() );
	// add the state manipulator
	_OSGViewer.addEventHandler( new osgGA::StateSetManipulator( _OSGViewer.getCamera()->getOrCreateStateSet() ) ) ;

	// add the thread model handler
	_OSGViewer.addEventHandler( new osgViewer::ThreadingHandler );

	// add the window size toggle handler
	_OSGViewer.addEventHandler( new osgViewer::WindowSizeHandler );

	// add the stats handler
	_OSGViewer.addEventHandler( new osgViewer::StatsHandler );

	// add the record camera path handler
	_OSGViewer.addEventHandler( new osgViewer::RecordCameraPathHandler );

	// add the LOD Scale handler
	_OSGViewer.addEventHandler( new osgViewer::LODScaleHandler );

	// add the screen capture handler
	_OSGViewer.addEventHandler( new osgViewer::ScreenCaptureHandler );
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: osgLoop
Description: Calls setup, saves starting camera position then hands control over
			 to osg
----------------------------------------------------------------------------------*/
int LogicEngine::osgLoop()
{	
	setUpViewer();	
	addHandlers();

	init();

	// set the scene to render
	_OSGViewer.setSceneData( _pRootSceneGroup );

	_OSGViewer.realize();

	//push default camera
	_vCameraMatrixes.push_back( _OSGViewer.getCameraManipulator()->getMatrix() );	

	return _OSGViewer.run();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addObjectToScene
Description: if textures are toggled retrieve a file name then send to 
			 SceneManipulator::addObjectToScene
----------------------------------------------------------------------------------*/
void LogicEngine::addObjectToScene( unsigned int uiObject, string sName )
{	
	string sFilePath = "";
	
	//get file path is needed
	if( _uiLoadState == LS_WITH_TEXTURES || uiObject == SceneManipulator::SCENEFILE )
	{
		sFilePath = ( CONSOL_IN == 0 ) ? getFileNameFromConsol() : FileFinder::getFilePath( FileFinder::FILTER_ALL );
	}

	//send command to scene manipulator
	SceneManipulator::addObjectToScene( _pRootSceneGroup, uiObject, sName, sFilePath, osg::Matrix::inverse( _pCamera->getViewMatrix() ) );
	
	_OSGViewer.setSceneData( _pRootSceneGroup );
	//_OSGViewer.realize();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: init
Description: if robot was loaded allow robot controls, setup HUD and add it to scene
			 graph
----------------------------------------------------------------------------------*/
bool LogicEngine::init()
{
	bool bWithRobot = ( _pRobot == NULL ) ? false : true;

	_pHud = new HUD( osg::Vec4d( 0, SCREENWIDTH, 0, SCREENHEIGHT), bWithRobot );
	_pHud->ref();

	_pRootSceneGroup->addChild( _pHud );    

	if( _pRobot ) _pRootSceneGroup->addChild( _pRobot->getModel() );

	return true;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setHUDItem
Description: Using if else to determine logic based on HUD item name
----------------------------------------------------------------------------------*/
void LogicEngine::setHUDItem( string sName )
{	
	if( sName == "hud_move_item" ) 
	{
		updateAllManipulators();
		_uiSelectionState = MOVE;
		setNewManipulators();
		_pHud->setSelectedItem( sName );
		highlightSelected();
	}
	else if( sName == "hud_rotate_item" ) 
	{
		updateAllManipulators();
		_uiSelectionState = ROTATE;
		setNewManipulators();
		_pHud->setSelectedItem( sName );
		highlightSelected();
	}
	else if( sName == "hud_scale_item" )
	{
		updateAllManipulators();
		_uiSelectionState = SCALE;
		setNewManipulators();
		_pHud->setSelectedItem( sName );
		highlightSelected();
	}
	else if( sName == "hud_robot_item" ) 
	{ 
		updateAllManipulators();
		_uiSelectionState = ROBOT;
		turnOffManipulators();
		_pHud->setSelectedItem( sName );
		unhighlightSelected();
	}
	else if( sName == "hud_select_all_item" )
	{
		selectAllObjects();
		_pHud->setSelectedItem( sName );
	}
	else if( sName == "hud_deselect_item")
	{		
		deselectAllObjects();
		_pHud->setSelectedItem( sName );
	}
	else if( sName == "hud_load_with_textures_item" )
	{
		_uiLoadState = ( _uiLoadState == LS_WITHOUT_TEXTURES ) ? LS_WITH_TEXTURES : LS_WITHOUT_TEXTURES;
		_pHud->setSelectedItem( sName );
	}	
	else if( sName == "hud_reset_camera_item" )
	{			
		_OSGViewer.getCameraManipulator()->setByMatrix( _vCameraMatrixes[0] );
		_pHud->setSelectedItem( sName );
	}
	else if( sName == "hud_save_camera_item" )
	{				
		_vCameraMatrixes.push_back( _OSGViewer.getCameraManipulator()->getMatrix() );
		_pHud->setSelectedItem( sName );
	}
	else if( sName == "hud_forward_camera_item" )
	{				
		_iCurrentCameraPosition++;			

		if( _iCurrentCameraPosition >= _vCameraMatrixes.size() )
		{
			_iCurrentCameraPosition = 0;
		}

		_OSGViewer.getCameraManipulator()->setByMatrix( _vCameraMatrixes[_iCurrentCameraPosition] );
		_pHud->setSelectedItem( sName );

	}
	else if( sName == "hud_back_camera_item" )
	{				
		_iCurrentCameraPosition = _iCurrentCameraPosition - 1;			

		if( _iCurrentCameraPosition < 0 )
		{
			_iCurrentCameraPosition =  _vCameraMatrixes.size() -1;
		}
		
		_OSGViewer.getCameraManipulator()->setByMatrix( _vCameraMatrixes[_iCurrentCameraPosition] );
		_pHud->setSelectedItem( sName );
	}
	else if( sName == "hud_save_scene_item" )
	{
		saveSelectedToFile();
		_pHud->setSelectedItem( sName );			
	}
	else if( sName == "hud_load_scene_item" )
	{
		loadSceneFile();
		_pHud->setSelectedItem( sName );
	}

	else if ( sName == "hud_delete_item" )
	{
		removeSelected();
		_pHud->setSelectedItem( sName );
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: selectObject
Description: Find the group node holder by traversing upwards using NodeFinder
			 if group is in selected list then break, if not add it and turn on
			 modifier switch and selection switch.
----------------------------------------------------------------------------------*/
void LogicEngine::selectObject( osg::Node* pNode )
{
	NodeFinder<osg::Group> groupFinder( osg::NodeVisitor::TRAVERSE_PARENTS );
	
	groupFinder.traverse( *pNode );

	vector<osg::Group*>* pvGroupList = groupFinder.getList();
	osg::Group* groupNode = NULL;
	
	for( vector<osg::Group*>::iterator i = pvGroupList->begin(); i != pvGroupList->end(); i++ )
	{
		if( ( *i )->getName().compare( 0, 6, "Group:" ) == 0 )
		{
			groupNode = ( *i );
			break;
		}	
	}

	vector<osg::Group*>::iterator iFind = find( _pvSelectedGroups->begin(), _pvSelectedGroups->end(), groupNode );

	if( iFind == _pvSelectedGroups->end() )
	{
		if( groupNode != NULL )
		{
			cout << groupNode->getName() << "\n";
			NodeFinder<osg::Switch> switchFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
			switchFinder.traverse( *groupNode );

			osg::Switch* pSwitch = switchFinder.getFirst();

			string tmp = pSwitch->getName();
			
			pSwitch->setSingleChildOn( _uiSelectionState );
			_pvSelectedGroups->push_back( groupNode );
			groupNode->ref();		
		}
	}

	highlightSelected();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setNewManipulators
Description: iterate though the selected list, changing the switch child that is on 
			 to reflect selection state
----------------------------------------------------------------------------------*/
void LogicEngine::setNewManipulators()
{
	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		NodeFinder<osg::Switch> switchFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
		switchFinder.traverse( *( *i ) );

		osg::Switch* pSwitch = switchFinder.getFirst();	
		pSwitch->setSingleChildOn( _uiSelectionState );
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: turnOffManipulators
Description:
----------------------------------------------------------------------------------*/
void LogicEngine::turnOffManipulators()
{
	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		NodeFinder<osg::Switch> switchFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
		switchFinder.traverse( *( *i ) );

		osg::Switch* pSwitch = switchFinder.getFirst();	
		pSwitch->setAllChildrenOff();
	}

	unhighlightSelected();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: deselectAllObjects
Description: iterate though the selected list, updating each manipulator matrix with
			 the current and then changing all switch childes to off, end with clearing 
			 selection list
----------------------------------------------------------------------------------*/
void LogicEngine::deselectAllObjects()
{
	updateAllManipulators();
	turnOffManipulators();
	
	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		 ( *i )->unref();		 
	}	

	_pvSelectedGroups->clear();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: updateAllManipulators
Description: iterate though the selected list, updating each manipulator matrix with
			 the current manipulator that is on
----------------------------------------------------------------------------------*/
void LogicEngine::updateAllManipulators()
{
	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		NodeFinder<osg::Switch> switchFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
		switchFinder.traverse( *( *i ) );

		osg::Switch* pSwitch = switchFinder.getFirst();		

		//get active matrix
		osg::Matrixf mCurrent;
		bool bFound = false;

		for( unsigned int ii = 0; ii < pSwitch->getNumChildren(); ii++ )
		{
			//find active child
			if( pSwitch->getValue( ii ) )
			{
				mCurrent = ( ( osgManipulator::Dragger* ) pSwitch->getChild( ii ) )->getMatrix();
				bFound = true;
				break;
			}			
		}

		if( bFound )
		{
			//set new active matrix to all
			for( unsigned int ii = 0; ii < pSwitch->getNumChildren(); ii++ )
			{
				( ( osgManipulator::Dragger* ) pSwitch->getChild( ii ) )->setMatrix( mCurrent );
			}
		}		
	}

	highlightSelected();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: selectAllObjects
Description: Stats by calling deselectAllObjects() Traverse the SG root node, find 
			 all STATICVALS::G_MO group nodes and add them to selection list, then 
			 end with calling highlightSelected()
----------------------------------------------------------------------------------*/
void LogicEngine::selectAllObjects()
{
	deselectAllObjects();

	NodeFinder<osg::Group> groupFiner( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
	groupFiner.traverse( *_pRootSceneGroup );

	vector<osg::Group*>* pvList = groupFiner.getList();

	for( vector<osg::Group*>::iterator i = pvList->begin(); i != pvList->end(); i++ )
	{
		if( ( *i )->getName().compare( 0, 9, STATICVALS::G_MO ) == 0 )
		{
			NodeFinder<osg::Switch> switchFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
			switchFinder.traverse( *( *i ) );

			osg::Switch* pSwitch = switchFinder.getFirst();

			string tmp = pSwitch->getName();

			pSwitch->setSingleChildOn( _uiSelectionState );
			_pvSelectedGroups->push_back( ( *i ) );
			( *i )->ref();
		}
	}


	highlightSelected();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: parseAruguments
Description: For each argument check that it ends with .osg determine if its a relevant
		     file that can be loaded, if no .osg loading file blank message is displayed
----------------------------------------------------------------------------------*/
void LogicEngine::parseAruguments( int argc, char* argv[] )
{
	bool bFileFound = false;

	for( int i = 1; i < argc; i++ )
	{
		string sValue = argv[i];
		
		if( sValue.compare( sValue.length() - 4, 4, ".osg" ) == 0 )
		{
			bFileFound = true;

			cout << "Info: Loading command line argument file (" << sValue << ") \n";
			osg::Matrixf matrix;
			matrix.makeRotate( 1.4f, 1.0f, 0.0f, 0.0f );

			stringstream ss;
			ss << i;

			_pRobot = new ModelRobot( sValue, STATICVALS::ROBOT_CONTROL );			
		}
	}

	if( !bFileFound )
	{
		cout << "Error: No suitable file (.osg) in command line arguments, scene will load blank" << "\n";
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setSelectedObjectsToMove
Description: if the select state is not ROBOT, then iterate though selection list
			 calling setMovemnt from the TransformNodes user data (TransformDataHolder)
----------------------------------------------------------------------------------*/
void LogicEngine::setSelectedObjectsToMove( unsigned int uiDirection )
{
	if( _uiSelectionState != ROBOT )
	{
		for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
		{
			NodeFinder<osg::MatrixTransform> mtFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
			mtFinder.traverse( *( *i ) );

			vector<osg::MatrixTransform*>* pvFindList = mtFinder.getList();

			for( vector<osg::MatrixTransform*>::iterator ii = pvFindList->begin(); ii != pvFindList->end(); ii++ )
			{			
				if( ( *ii )->getName().compare( 0, 19, STATICVALS::MT_MO ) == 0 )
				{
					string sTemp = ( *ii )->getName();

					TransformDataHolder* pUserData = dynamic_cast<TransformDataHolder*> ( ( *ii )->getUserData() );

					if( pUserData )
					{
						pUserData->setMovement( determinMovement( uiDirection ) );
						break;
					}
				}
			}
		}
	}
	else if( _uiSelectionState == ROBOT )
	{
		_pRobot->setMovement( determinMovement( uiDirection )  );
	}
	else
	{
		cout << "Error: unknown state" << "\n";
	}
	
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: determinMovement
Description: use if and switch to determine what action to take based on the arrow
			 keys, allows reuse of a single key for rotate and move
----------------------------------------------------------------------------------*/
unsigned int LogicEngine::determinMovement( unsigned int uiDirection )
{
	if( _uiSelectionState == MOVE )
	{
		switch( uiDirection )
		{
			case DIRECTION_UP: return TransformDataHolder::TRANSLATE_UP; break;
			case DIRECTION_DOWN: return TransformDataHolder::TRANSLATE_DOWN; break;
			case DIRECTION_LEFT: return TransformDataHolder::TRANSLATE_LEFT; break;
			case DIRECTION_RIGHT: return TransformDataHolder::TRANSLATE_RIGHT; break;
		}
	}

	else if( _uiSelectionState == ROTATE )
	{
		switch( uiDirection )
		{
			case DIRECTION_UP: return TransformDataHolder::ROTATE_UP; break;
			case DIRECTION_DOWN: return TransformDataHolder::ROTATE_DOWN; break;
			case DIRECTION_LEFT: return TransformDataHolder::ROTATE_LEFT; break;
			case DIRECTION_RIGHT: return TransformDataHolder::ROTATE_RIGHT; break;
		}
	}

	else
	{
		switch( uiDirection )
		{
			case DIRECTION_UP: return TransformDataHolder::ROTATE_UP; break;
			case DIRECTION_DOWN: return TransformDataHolder::ROTATE_DOWN; break;
			case DIRECTION_LEFT: return TransformDataHolder::ROTATE_AROUND_LEFT; break;
			case DIRECTION_RIGHT: return TransformDataHolder::ROTATE_AROUND_RIGHT; break;
		}
	}

	return TransformDataHolder::NO_MOVEMENT;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setSelectedObjectsToStop
Description: if the select state is not ROBOT, then iterate though selection list
			 calling setMovemnt from the TransformNodes user data (TransformDataHolder)
			 with no direction
----------------------------------------------------------------------------------*/
void LogicEngine::setSelectedObjectsToStop()
{
	if( _uiSelectionState != ROBOT )
	{
	
		for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
		{
			NodeFinder<osg::MatrixTransform> mtFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
			mtFinder.traverse( *( *i ) );

			vector<osg::MatrixTransform*>* pvFindList = mtFinder.getList();

			for( vector<osg::MatrixTransform*>::iterator ii = pvFindList->begin(); ii != pvFindList->end(); ii++ )
			{			
				if( ( *ii )->getName().compare( 0, 19, STATICVALS::MT_MO ) == 0 )
				{
					string sTemp = ( *ii )->getName();

					TransformDataHolder* pUserData = dynamic_cast<TransformDataHolder*> ( ( *ii )->getUserData() );

					if( pUserData )
					{
						pUserData->setMovement( TransformDataHolder::NO_MOVEMENT );
					}
				}
			}
		}
	}
	else
	{
		_pRobot->setMovement( TransformDataHolder::NO_MOVEMENT );
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: updateModifiresWithModel
Description: traverse up the tree till you find the group node, then for each dragger
		     in the switch  update its matrix with the passed in objects matrix
----------------------------------------------------------------------------------*/
void LogicEngine::updateModifiresWithModel( osg::MatrixTransform* pMatrix )
{
	osg::Group* pGroup = pMatrix->getParent( 0 );

	NodeFinder<osg::Switch> switchFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
	switchFinder.traverse( *pGroup );

	osg::Switch* pSwitch = switchFinder.getFirst();

	osgManipulator::Dragger* pDrager = ( osgManipulator::Dragger* ) pSwitch->getChild( _uiSelectionState );
	
	osg::Matrixf mScales;
	mScales.makeScale( STATICVALS::DRAGER_SCALE(), STATICVALS::DRAGER_SCALE(), STATICVALS::DRAGER_SCALE() );

	if( pDrager ) pDrager->setMatrix( mScales* pMatrix->getMatrix() );
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getRobot
Description: return the ModelRobot instance
----------------------------------------------------------------------------------*/
ModelRobot* LogicEngine::getRobot()
{
	return _pRobot;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: saveSelectedToFile
Description: calls saveObjects ( futuer planned functio )
----------------------------------------------------------------------------------*/
void LogicEngine::saveSelectedToFile()
{
	saveObjects();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: loadSceneFile
Description: get xml file, then parse it in loading the objects and textures stored
			 within it by calling SceneManipulator::addMySDFToScene
----------------------------------------------------------------------------------*/
void LogicEngine::loadSceneFile()
{
	string sLoction = getXMLLocation( false );

	if( sLoction == "" || !boost::filesystem::exists( sLoction ) )
	{
		cout << "Error: can not find file";
		return;
	}
	
	{
		using boost::property_tree::ptree;

		ptree pt;

		read_xml( sLoction, pt );

		BOOST_FOREACH( ptree::value_type &v, pt.get_child( "scene" ) )
		{
			string sFileName = "";
			string sTexture = "";		
			osg::Group* pGroup = NULL;
			bool bRobotModel = false;

			BOOST_FOREACH( ptree::value_type &v2, v.second )
			{
				stringstream ss;
				ss << v2.second.data();
				string aaa = ss.str();				

				if( v2.first == "Robot" )
				{
					bRobotModel = ( ss.str() == "0" ) ? true : false;
				}
				
				else if( !bRobotModel && v2.first == "filename" )
				{								
					sFileName = STATICVALS::SAVE_SCENE_OSG_FOLDER + ss.str();					
				}
				else if ( !bRobotModel && v2.first == "Texture" )
				{
					sTexture = ss.str();
					//texture is last info needed to load blank model
					pGroup = SceneManipulator::addMySDFToScene( _pRootSceneGroup, sFileName, sFileName, sTexture );
				}				
				
				else if( v2.first == "datanodes" )
				{
					BOOST_FOREACH( ptree::value_type &v3, v2.second )
					{
						string sNodeType = "";
						string sNodeName = "";
						string sX = "";
						string sY = "";
						string sZ = "";
						string sLimits = "";

						BOOST_FOREACH( ptree::value_type &v4, v3.second )
						{
							stringstream ssNodeData;
							ssNodeData << v4.second.data();
							
							if( v4.first == "name" )
							{
								sNodeName = ssNodeData.str();
							}
							
							else if( v4.first == "NodeType" )
							{
								sNodeType = ssNodeData.str();
							}

							else if( v4.first == "RotationX" )
							{
								sX = ssNodeData.str();
							}

							else if( v4.first == "RotationY" )
							{
								sY = ssNodeData.str();
							}

							else if( v4.first == "RotationZ" )
							{
								sZ = ssNodeData.str();
							}
						}

						if( bRobotModel )
						{
							NodeNameFinder<osg::Node> nodeFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN, sNodeName );
							nodeFinder.traverse( *_pRootSceneGroup );
							osg::Node* pNode = nodeFinder.getFirst();							
							TransformDataHolder* pDataHolder = dynamic_cast<TransformDataHolder*>( pNode->getUserData() );

							if( pDataHolder != NULL )
							{
								pDataHolder->setData( stof( sX ), stof( sY ), stof( sZ ) );
							}
						}
					}														
				}
			}			
		}
	}	
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: saveObjects
Description: write selected objects to a file using SceneIO::writeScene, filname is
		     a number that is loaded from a prefs file, this number is then incremented
			 and then rewrote to the prefs file
----------------------------------------------------------------------------------*/
void LogicEngine::saveObjects()
{	
	int iNodeCount = getSavedObjectCount();

	if( iNodeCount == -1 ) return;

	int iStartingCount = iNodeCount;	

	//---------------------------------

	string sLoction = getXMLLocation( true );

	if( sLoction == "" ) return;

	ofstream ofFile;

	ofFile.open( sLoction );

	//---------------------------------

	if( ofFile )
	{
		string sTmp;
		ofFile << "<?xml version=\"1.0\"?>" << "\n";
		ofFile << "<scene>" << "\n";
		
		int iIncrement = 0;
		for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++, iIncrement++ )
		{
			stringstream ssStream;
			ssStream << iNodeCount;
			SceneIO::writeScene( ( *i )->getChild( 1 ), STATICVALS::SCENE_OBJECT_FOLDER + ssStream.str() + ".osg" );							

			ofFile << "<object>" << "\n";

			stringstream ssXMLStream;
			ssXMLStream << ( iStartingCount + iIncrement );

			sTmp = "scene_object_" + ssXMLStream.str() + ".osg";

			string sRobot = ( ( *i )->getName().find( "ROBOT" ) != string::npos ) ? "0" : "1";
			ofFile << "<Robot>" << sRobot << "</Robot>" << "\n";
			ofFile << "<filename>" << sTmp <<"</filename>" << "\n";
			ofFile << "<Texture>" << ( ( GroupDataHolder* ) ( *i )->getUserData() )->TextureLocation <<"</Texture>" << "\n";

			ofFile << "<datanodes>" << "\n";

			NodeSerialiser nodeSerialser( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
			nodeSerialser.traverse( *( *i ) );
			ofFile << nodeSerialser.getXMLString() << "\n";

			ofFile << "</datanodes>" << "\n";
			ofFile << "</object>" << "\n";			
			
			iNodeCount++;
		}

		ofFile << "</scene>" << "\n";

		writeSavedObjectCout( iNodeCount );
	
		//writeMySceneFile( iStartingCount, _pvSelectedGroups->size(), vsTextures );
		
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getSavedObjectCount
Description: read object count from prefs file using ifstream
----------------------------------------------------------------------------------*/
int LogicEngine::getSavedObjectCount()
{
	ifstream ifFile;
	ifFile.open( STATICVALS::SAVED_SCENE_NODE_COUNT_FILE );

	unsigned int iNodeCount = 0;

	if( ifFile )
	{
		string sLine;
		getline( ifFile, sLine );

		iNodeCount = stoi( sLine );

		ifFile.close();
	}
	else
	{
		return -1;
	}	

	return iNodeCount;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: writeSavedObjectCout
Description: write object count from prefs file using ofstream
----------------------------------------------------------------------------------*/
void LogicEngine::writeSavedObjectCout( int iNodeCount )
{
	ofstream ofFile;
	ofFile.open( STATICVALS::SAVED_SCENE_NODE_COUNT_FILE );

	if( ofFile )
	{
		ofFile << iNodeCount;

		ofFile.close();
	}
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getFileNameFromConsol
Description: uses cin to get file name
----------------------------------------------------------------------------------*/
std::string LogicEngine::getFileNameFromConsol()
{
	string sInput;

	cout << "-------------------------- Input ---------------------------" << "\n";
	cout << "            Please Insert a file from data folder " << "\n";

	cin >> sInput;
	cout << "------------------------------------------------------------" << "\n";

	return STATICVALS::DATA_FOLDER + sInput;

}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getFileNameFromConsolSave
Description: uses cin to get file name to be saved in data folder
----------------------------------------------------------------------------------*/
std::string LogicEngine::getFileNameFromConsolSave()
{
	string sInput;

	cout << "---------------------------- Input ------------------------------" << "\n";
	cout << "Please Insert a file name to be saved in the data Scene folder" << "\n";

	cin >> sInput;
	cout << "-----------------------------------------------------------------" << "\n";

	return sInput;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getXMLLocation
Description: uses an if to dermin how to get a file name, then calls relevant funciton
----------------------------------------------------------------------------------*/
std::string LogicEngine::getXMLLocation( bool bOverwrite )
{
	string sLoction = ( CONSOL_IN == 0 ) ? getFileNameFromConsolSave() : FileFinder::getFilePath( FileFinder::FILTER_ALL );	

	if( sLoction == "" )
	{
		return "";
	}

	if( CONSOL_IN == 0 )
	{
		sLoction = STATICVALS::SAVE_SCENE_FOLDER + sLoction;
	}

	if( sLoction.compare( sLoction.length() - 4, 4, ".xml" ) )
	{
		sLoction = sLoction + ".xml";
	}

	//check if exists
	if ( bOverwrite )
	{	
		boost::filesystem::path pFile( sLoction );

		if( boost::filesystem::exists( pFile ) )
		{
			cout << "------------------------------ Input Error ------------------------------" << "\n";
			cout << "     Your chosen file already exists do you wish to overwrite? (y/n)" << "\n";

			string sError;
			cin >> sError;

			if( sError.compare( "y" ) == 0 )
			{
				cout << "File will be over written!" << "\n";
			}
			else
			{
				cout << "File will not be over written!" << "\n";
				sLoction = "";
			}

			cout << "-------------------------------------------------------------------------" << "\n";
		}
	}

	return sLoction;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: highlightSelected
Description: if selection state is no ROBOT iterate though selected list setting the
		     second nod node of the MatrixTransform ( a switch ) to the relevant
			 on child
----------------------------------------------------------------------------------*/
void LogicEngine::highlightSelected()
{
	if( _uiSelectionState != ROBOT )
	{
		for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
		{
			osg::MatrixTransform* pMT = ( osg::MatrixTransform* ) ( *i )->getChild( 1 );

			if( pMT )
			{
				osg::Switch* pSwitch = ( osg::Switch* ) pMT->getChild( 0 );

				if( pSwitch )
				{
					pSwitch->setSingleChildOn( _uiSelectionState );
				}

			}		
		}
	}	
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: unhighlightSelected
Description: iterate though selected list setting the second nod node of the 
			 MatrixTransform ( a switch ) to all childeren off
----------------------------------------------------------------------------------*/
void LogicEngine::unhighlightSelected()
{
	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		osg::MatrixTransform* pMT = ( osg::MatrixTransform* ) ( *i )->getChild( 1 );

		if( pMT )
		{
			osg::Switch* pSwitch = ( osg::Switch* ) pMT->getChild( 0 );

			if( pSwitch )
			{
				pSwitch->setAllChildrenOff();
			}

		}		
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: loadPrefs
Description: using ifstream read prefs from file 
----------------------------------------------------------------------------------*/
void LogicEngine::loadPrefs()
{
	ifstream iFile( STATICVALS::INI_FILE );

	if( iFile )
	{
		string sLine;

		while( iFile.good() )
		{
			getline( iFile, sLine );

			if( sLine.compare( 0, 13, "screen_width:") == 0 ) SCREENWIDTH = stoi( sLine.substr( 13 ) );
			if( sLine.compare( 0, 14, "screen_height:") == 0 ) SCREENHEIGHT = stoi( sLine.substr( 14 ) );
		}
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: printSeleciton
Description: iterate though selection list using NodePrinter to traverse though
----------------------------------------------------------------------------------*/
void LogicEngine::printSeleciton()
{
	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		NodePrinter<osg::Node> nodePrinter( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
		nodePrinter.traverse( *( *i ) );
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: removeSelected
Description: iterate though selected list, removing each group node from the SG
----------------------------------------------------------------------------------*/
void LogicEngine::removeSelected()
{
	for( vector<osg::Group*>::iterator i = _pvSelectedGroups->begin(); i != _pvSelectedGroups->end(); i++ )
	{
		_pRootSceneGroup->removeChild( ( *i ) );
	}

	deselectAllObjects();
}
