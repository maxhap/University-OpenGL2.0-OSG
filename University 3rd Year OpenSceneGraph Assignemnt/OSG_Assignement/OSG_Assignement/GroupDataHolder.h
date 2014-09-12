#ifndef _GROUPDATAHOLDER_
#define _GROUPDATAHOLDER_

#include "stdafx.h"
#include <OSG_Utils/DataHolder.h>

using namespace std;

class __declspec(dllexport) GroupDataHolder : public DataHolder
{

public:

	GroupDataHolder();
	virtual ~GroupDataHolder();

	virtual void serialise( stringstream& ofstream );

	string TextureLocation;
};


#endif