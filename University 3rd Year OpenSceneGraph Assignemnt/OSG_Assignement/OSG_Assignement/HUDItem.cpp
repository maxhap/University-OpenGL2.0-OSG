#include "stdafx.h"
#include "HUDItem.h"

HUDItem::HUDItem( string sName, string sText, osg::Vec3f vPosition, string sFont ) : osg::Geode()
{
	_sName = sName;
	
	_uiType = TYPE_SINGLE_SELECTION;

	_phItemData = new HUDItemDataHolder( sText, vPosition, sFont );
	_phItemData->ref();

	setup();
}

HUDItem::~HUDItem()
{
	if( _phItemData ) _phItemData->unref();
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setup
Description: set name, inherited state set and  inherited drawable
----------------------------------------------------------------------------------*/
void HUDItem::setup()
{
	this->setName( _sName );

	osg::StateSet* pStateSet = this->getOrCreateStateSet();
	pStateSet ->setMode( GL_LIGHTING,osg::StateAttribute::OFF );
	pStateSet ->setMode( GL_DEPTH_TEST,osg::StateAttribute::OFF );

	this->setUserData( _phItemData );
	this->addDrawable( _phItemData->getText() );
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getItemData
Description: return _phItemData	
----------------------------------------------------------------------------------*/
HUDItemDataHolder* HUDItem::getItemData()
{
	return _phItemData;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getType
Description: return _uiType
----------------------------------------------------------------------------------*/
unsigned int HUDItem::getType()
{
	return _uiType;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setType
Description: set _uiType and push it to data holder	
----------------------------------------------------------------------------------*/
void HUDItem::setType( unsigned int uiType )
{
	_uiType = uiType;
	_phItemData->setType( uiType );
}
