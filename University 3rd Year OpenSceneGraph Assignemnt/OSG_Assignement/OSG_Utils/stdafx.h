// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <iostream>
#include <fstream>
#include <string>

#include <osg/Node>
#include <osgDB/WriteFile>
#include <osgDB/ReaderWriter>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>

using namespace std;