#ifndef _SCENESAVER_
#define _SCENESAVER_

#include "stdafx.h"

using namespace std;

class  __declspec(dllexport) SceneIO
{

public :

	SceneIO();
	virtual ~SceneIO();

	static void writeScene( osg::Node* pRoot, string sFilePath );

protected:


};

#endif


