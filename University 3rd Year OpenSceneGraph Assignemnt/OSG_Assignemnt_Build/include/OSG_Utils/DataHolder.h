#ifndef _DATAHOLDER_
#define _DATAHOLDER_

#include "stdafx.h"
#include <iostream>

class __declspec(dllexport) DataHolder : public osg::Referenced
{

public:

	DataHolder();
	virtual ~DataHolder();

	virtual void serialise( stringstream& ofFile );

protected:

};
#endif