#ifndef _MODELPRINTER_
#define  _MODELPRINTER_

#include "stdafx.h"

#include "NodeFinder.h"

using namespace std;

class __declspec(dllexport) ModelPrinter
{

public:

	ModelPrinter();

	static void printToFile( string sFilePath, osg::Node* pTopNode );


protected:

};

#endif