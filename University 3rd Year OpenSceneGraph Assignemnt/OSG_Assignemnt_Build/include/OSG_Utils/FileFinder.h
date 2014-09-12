#ifndef _FILEFINDER_
#define _FILEFINDER_

#include "stdafx.h"

#include <direct.h>
#define GetCurrentDir _getcwd
#define SetCurrentDir _chdir

class  __declspec(dllexport) FileFinder
{

public:

	static const unsigned int FILTER_ALL = 0;
	static const unsigned int FILTER_OBJECT = 1;
	static const unsigned int FILTER_TEXTURE = 2;
	static const unsigned int FILTER_SCENE = 3;
	static const unsigned int FILTER_XML = 4;

	static std::string getFilePath( unsigned int uiFilter );
	static std::string getSaveFilePath( unsigned int uiFilter );

};

#endif