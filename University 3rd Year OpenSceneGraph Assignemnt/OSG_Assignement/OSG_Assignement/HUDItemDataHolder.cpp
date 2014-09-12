#include "stdafx.h"
#include "HUDItemDataHolder.h"

HUDItemDataHolder::HUDItemDataHolder( string sText, osg::Vec3f vPosition, string sFont )
{
	_bDataChanged = true;
	_bSelected = false;
	_sText = sText;
	_vPosition = vPosition;
	_sFont = sFont;
	_uiItemType = HUDItem::TYPE_SINGLE_SELECTION;

	_pvColour = new osg::Vec4f( 1.0f, 1.0f, 1.0f, 1.0f );

	_pText = new  osgText::Text;
	_pText->ref();

	_uiBlickCounter = 0;

	setup();

}

HUDItemDataHolder::~HUDItemDataHolder()
{
	if( _pvColour ) delete _pvColour; 
	if( _pText ) _pText->unref();
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setSelected
Description: if _bSelected is different set _bSelected and change colour
----------------------------------------------------------------------------------*/
void HUDItemDataHolder::setSelected( bool bSelected )
{
	if( bSelected != _bSelected )
	{
		_bDataChanged = true;
		_bSelected = bSelected;

		if( bSelected )	_pvColour->set( 0.7f, 0.0f, 0.0f, 1.0f );
		else _pvColour->set( 1.0f, 1.0f, 1.0f, 1.0f );

		_pText->setColor( *_pvColour );
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getColour
Description: return _pvColour
----------------------------------------------------------------------------------*/
osg::Vec4f* HUDItemDataHolder::getColour() const
{
	return _pvColour;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setup
Description: set font text and colour 
----------------------------------------------------------------------------------*/
void HUDItemDataHolder::setup()
{
	std::string sTimesFont( _sFont );

	_pText->setFont( sTimesFont );
	_pText->setText( _sText );
	_pText->setPosition( _vPosition );
	_pText->setColor( *_pvColour );
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getText
Description: return _pText
----------------------------------------------------------------------------------*/
osgText::Text* HUDItemDataHolder::getText() const
{
	return _pText;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getSelected
Description: return _bSelected
----------------------------------------------------------------------------------*/
bool HUDItemDataHolder::getSelected()
{
	return _bSelected;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setType
Description: set _uiItemType
----------------------------------------------------------------------------------*/
void HUDItemDataHolder::setType( unsigned int uiType )
{
	_uiItemType = uiType;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: update
Description: 1. if blinking type item, deselected after MENU_BLINK_ITERATIONS iterations 
----------------------------------------------------------------------------------*/
void HUDItemDataHolder::update()
{
	if( _uiItemType == HUDItem::TYPE_BLINK_SELECTION )
	{
		if( _bSelected )
		{
			if( _uiBlickCounter > STATICVALS::MENU_BLINK_ITERATIONS )
			{
				_uiBlickCounter = 0;
				setSelected( false );
			}

			_uiBlickCounter++;
		}		
	}
}


