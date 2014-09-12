#include "stdafx.h"
#include "HUD.h"

HUD::HUD( osg::Vec4d  vWindowSize, bool bWithRobot ) : osg::Group()
{
	_pHudCamera = new HUDCamera( "hud_camera", vWindowSize );
	_pHudCamera->ref();

	_pvItemList = new vector<HUDItem*>();

	setup( bWithRobot );
}

HUD::~HUD()
{
	if( _pHudCamera ) _pHudCamera->unref();

	for( vector<HUDItem*>::iterator i = _pvItemList->begin(); i != _pvItemList->end(); i++ )
	{
		( *i )->unref();
	}

	if( _pvItemList ) delete _pvItemList;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setup
Description: create HUD items and add them to the _pvItemList
----------------------------------------------------------------------------------*/
void HUD::setup( bool bWithRobot )
{
	osg::Vec3 vPosition( LogicEngine::instance().SCREENWIDTH - 210, LogicEngine::instance().SCREENHEIGHT - 50, 0.0f );
	osg::Vec3 vDelta( 0.0f, -60.0f, 0.0f );

	//title creating
	HUDItem* phiTitle = new HUDItem( "hud_title", "Object Modes", vPosition, "fonts/times.ttf" );
	_pHudCamera->addChild( phiTitle );
	_pvItemList->push_back( phiTitle );
	phiTitle->ref();	
	phiTitle->addUpdateCallback( new HUDItemCallBack );

	vPosition += vDelta;

	//move creation
	HUDItem* phiMove = new HUDItem( "hud_move_item", "Move", vPosition, "fonts/times.ttf" );
	phiMove->getItemData()->setSelected( true );
	_pHudCamera->addChild( phiMove );
	_pvItemList->push_back( phiMove );
	phiMove->ref();

	vPosition += vDelta;

	//rotate creation
	HUDItem* phRotate = new HUDItem( "hud_rotate_item", "Rotate", vPosition, "fonts/times.ttf" );
	_pHudCamera->addChild( phRotate );	
	_pvItemList->push_back( phRotate );
	phRotate->ref();

	vPosition += vDelta;

	//scale creation
	HUDItem* phScale = new HUDItem( "hud_scale_item", "Scale", vPosition, "fonts/times.ttf" );
	_pHudCamera->addChild( phScale );	
	_pvItemList->push_back( phScale );
	phScale->ref();

	vPosition += vDelta;

	if( bWithRobot )
	{
		HUDItem* phRobot = new HUDItem( "hud_robot_item", "Robot Control", vPosition, "fonts/times.ttf" );
		_pHudCamera->addChild( phRobot );
		_pvItemList->push_back( phRobot );
		phRobot->ref();
	}	

	osg::Vec3 vSelectionPosition( 20.0f, LogicEngine::instance().SCREENHEIGHT - 50, 0.0f );

	//selection title creating
	HUDItem* phiSelectionTitle = new HUDItem( "hud_selection_title", "Selection Modes", vSelectionPosition, "fonts/times.ttf" );
	_pHudCamera->addChild( phiSelectionTitle );
	_pvItemList->push_back( phiSelectionTitle );
	phiSelectionTitle->ref();

	vSelectionPosition += vDelta;

	//select all
	HUDItem* phSelectAll = new HUDItem( "hud_select_all_item", "Select All", vSelectionPosition, "fonts/times.ttf" );
	phSelectAll->addUpdateCallback( new HUDItemCallBack() );
	phSelectAll->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phSelectAll );
	_pvItemList->push_back( phSelectAll );
	phSelectAll->ref();

	vSelectionPosition += vDelta;

	//deselection
	HUDItem* phDeselect = new HUDItem( "hud_deselect_item", "Deselect All", vSelectionPosition, "fonts/times.ttf" );
	phDeselect->addUpdateCallback( new HUDItemCallBack() );
	phDeselect->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phDeselect );
	_pvItemList->push_back( phDeselect );
	phDeselect->ref();

	vSelectionPosition += vDelta;
	
	//delete selected
	HUDItem* phDelselect = new HUDItem( "hud_delete_item", "Delete Selected", vSelectionPosition, "fonts/times.ttf" );
	phDelselect->addUpdateCallback( new HUDItemCallBack() );
	phDelselect->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phDelselect );
	_pvItemList->push_back( phDelselect );
	phDelselect->ref();

	osg::Vec3 vTogglePosition( 20.0f, 90.0f, 0.0f );


	//toggle title creating
	HUDItem* phiTogleTitle = new HUDItem( "hud_toggle_title", "Toggles", vTogglePosition, "fonts/times.ttf" );	
	_pHudCamera->addChild( phiTogleTitle );
	_pvItemList->push_back( phiTogleTitle );
	phiTogleTitle->ref();

	vTogglePosition += vDelta;

	//textures
	HUDItem* phTextures = new HUDItem( "hud_load_with_textures_item", "Load with Textures", vTogglePosition, "fonts/times.ttf" );
	phTextures->setType( HUDItem::TYPE_TOGLE_SELECTION );
	_pHudCamera->addChild( phTextures );
	_pvItemList->push_back( phTextures );
	phTextures->ref();

	vTogglePosition += vDelta;

	//camera control 
	
	osg::Vec3 vCameraPosition( LogicEngine::instance().SCREENWIDTH - 240, 270.0f, 0.0f );

	HUDItem* phiCameraTitle = new HUDItem( "hud_camera_title", "Camera Control", vCameraPosition, "fonts/times.ttf" );
	_pHudCamera->addChild( phiCameraTitle );
	_pvItemList->push_back( phiCameraTitle );
	phiCameraTitle->ref();

	vCameraPosition += vDelta;

	//CameraReset

	HUDItem* phCameraReset = new HUDItem( "hud_reset_camera_item", "Reset Position", vCameraPosition, "fonts/times.ttf" );
	phCameraReset->addUpdateCallback( new HUDItemCallBack() );
	phCameraReset->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phCameraReset );
	_pvItemList->push_back( phCameraReset );
	phCameraReset->ref();

	vCameraPosition += vDelta;

	//save camera
	HUDItem* phSaveCamera = new HUDItem( "hud_save_camera_item", "Save Position", vCameraPosition, "fonts/times.ttf" );
	phSaveCamera->addUpdateCallback( new HUDItemCallBack() );
	phSaveCamera->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phSaveCamera );
	_pvItemList->push_back( phSaveCamera );
	phSaveCamera->ref();

	vCameraPosition += vDelta;

	//forward
	HUDItem* phForwardCamera = new HUDItem( "hud_forward_camera_item", "Forward Position", vCameraPosition, "fonts/times.ttf" );
	phForwardCamera->addUpdateCallback( new HUDItemCallBack() );
	phForwardCamera->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phForwardCamera );
	_pvItemList->push_back( phForwardCamera );
	phForwardCamera->ref();

	vCameraPosition += vDelta;

	//back
	HUDItem* phBackCamera = new HUDItem( "hud_back_camera_item", "Back Position", vCameraPosition, "fonts/times.ttf" );
	phBackCamera->addUpdateCallback( new HUDItemCallBack() );
	phBackCamera->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phBackCamera );
	_pvItemList->push_back( phBackCamera );
	phBackCamera->ref();

	vCameraPosition += vDelta;


	osg::Vec3 vScenePosition( 20.0f, LogicEngine::instance().SCREENHEIGHT / 2 + 50.0f, 0.0f );

	HUDItem* phiSceneTitle = new HUDItem( "hud_scene_title", "Scene Control", vScenePosition, "fonts/times.ttf" );
	_pHudCamera->addChild( phiSceneTitle );
	_pvItemList->push_back( phiSceneTitle );
	phiSceneTitle->ref();

	vScenePosition += vDelta;

	HUDItem* phSaveScene = new HUDItem( "hud_save_scene_item", "Save Selected", vScenePosition, "fonts/times.ttf" );
	phSaveScene->addUpdateCallback( new HUDItemCallBack() );
	phSaveScene->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phSaveScene );
	_pvItemList->push_back( phSaveScene );
	phSaveScene->ref();

	vScenePosition += vDelta;

	HUDItem* phLoadScene = new HUDItem( "hud_load_scene_item", "Load Into Current", vScenePosition, "fonts/times.ttf" );
	phLoadScene->addUpdateCallback( new HUDItemCallBack() );
	phLoadScene->setType( HUDItem::TYPE_BLINK_SELECTION );
	_pHudCamera->addChild( phLoadScene );
	_pvItemList->push_back( phLoadScene );
	phLoadScene->ref();

	vScenePosition += vDelta;

	//attach camera
	this->addChild( _pHudCamera );
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getCamera
Description: return _pHudCamera
----------------------------------------------------------------------------------*/
HUDCamera* HUD::getCamera()
{
	return _pHudCamera;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setup
Description: 1. iterate though list finding the node by that name,
			 2. if the node type is a toggle call setToggleItem
			 3. if its a blink, simply setSelected true
			 4. if its anything else (single item selection) setSelected false on
				similar type nodes then set this nodes selection to true
----------------------------------------------------------------------------------*/
void HUD::setSelectedItem( string sName )
{
	//find if togle item
	unsigned int bItemType = 0;

	for( vector<HUDItem*>::iterator i = _pvItemList->begin(); i != _pvItemList->end(); i++ )
	{
		if( ( *i )->getName() == sName ) 
		{
			bItemType = ( *i )->getType();
			break;
		}
	}

	if( bItemType == HUDItem::TYPE_TOGLE_SELECTION )
	{
		setToggleItem( sName );
	}
	else if( bItemType == HUDItem::TYPE_BLINK_SELECTION )
	{
		for( vector<HUDItem*>::iterator i = _pvItemList->begin(); i != _pvItemList->end(); i++ )
		{			
			if( ( *i )->getName() == sName ) 
			{	
				( *i )->getItemData()->setSelected( true );				
				break;
			}
		}
	}
	else
	{
		for( vector<HUDItem*>::iterator i = _pvItemList->begin(); i != _pvItemList->end(); i++ )
		{
			if( ( *i )->getType() != HUDItem::TYPE_TOGLE_SELECTION || ( *i )->getType() != HUDItem::TYPE_BLINK_SELECTION )
			{
				if( ( *i )->getName() == sName ) ( *i )->getItemData()->setSelected( true );
				else ( *i )->getItemData()->setSelected( false );	
			}			
		}
	}	
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setToggleItem
Description: iterate through to find the node, if its state is true then set it to false
			 and vister verser
----------------------------------------------------------------------------------*/
void HUD::setToggleItem( string sName )
{
	for( vector<HUDItem*>::iterator i = _pvItemList->begin(); i != _pvItemList->end(); i++ )
	{
		if( ( *i )->getName() == sName ) 
		{
			( ( *i )->getItemData()->getSelected() ) ? ( *i )->getItemData()->setSelected( false ) : ( *i )->getItemData()->setSelected( true );		
		}
	}
}
