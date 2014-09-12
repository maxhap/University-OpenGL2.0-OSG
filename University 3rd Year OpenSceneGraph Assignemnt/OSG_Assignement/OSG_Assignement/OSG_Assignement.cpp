// raaRobotController.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LogicEngine.h"

int main(int argc, char* argv[] )
{
	osg::ArgumentParser arguments( &argc,argv );
	
	LogicEngine::instance().parseAruguments( argc, argv );
	return LogicEngine::instance().osgLoop();
}

