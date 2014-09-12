#ifndef _HUDITEMDATAHOLDER_
#define _HUDITEMDATAHOLDER_

#include "stdafx.h"
#include <osgText/Text>
#include <osg/Referenced>
#include <osg/Vec4f>
#include <osg/Vec3f>

#include "HUDItem.h"
#include <OSG_Utils/DataHolder.h>
#include "STATICVALS.h"

class HUDItem;

using namespace std;

class HUDItemDataHolder : public osg::Referenced
{

public:

	HUDItemDataHolder( string sText, osg::Vec3f vPosition, string sFont );
	virtual ~HUDItemDataHolder();

	void setSelected( bool bSelected );
	void setType( unsigned int uiType );
	void update();

	bool getSelected();
	
	osg::Vec4f* getColour() const;
	osgText::Text* getText() const;

protected:

	unsigned int _uiItemType;
	unsigned int _uiBlickCounter;

	bool _bDataChanged;
	bool _bSelected;

	string _sText;
	string _sFont;

	osg::Vec3f _vPosition;	
	osg::Vec4f* _pvColour;

	osgText::Text* _pText; 

	void setup();

};

#endif