#ifndef _HUDITEM_
#define _HUDITEM_

#include "stdafx.h"
#include <osg/Geode>
#include <osgText/Text>
#include <osg/Vec3f>
#include <osg/StateSet>
#include "HUDItemDataHolder.h"

class HUDItemDataHolder;

using namespace std;

class HUDItem : public osg::Geode
{

public:

	static const unsigned int TYPE_SINGLE_SELECTION = 0;
	static const unsigned int TYPE_TOGLE_SELECTION = 1;
	static const unsigned int TYPE_BLINK_SELECTION = 2;

	HUDItem( string sName, string sText, osg::Vec3f vPosition, string sFont );
	virtual ~HUDItem();

	HUDItemDataHolder* getItemData();

	void setType( unsigned int uiType );

	unsigned int getType();

protected:

	string _sName;

	unsigned int _uiType;

	HUDItemDataHolder* _phItemData;

	void setup();

};

#endif