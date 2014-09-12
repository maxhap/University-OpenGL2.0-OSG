#include "stdafx.h"
#include "STATICVALS.h"

typedef basic_string<char,char_traits<char>,allocator<char> > const  INIT;

INIT STATICVALS::INI_FILE = "..\\data\\prefs.ini";
INIT STATICVALS::ROBOT_CONTROL = "..\\data\\robot_control.txt";

INIT STATICVALS::SCENE_OBJECT_FOLDER = "..\\data\\save_scenes\\osg_files\\scene_object_";
INIT STATICVALS::SAVED_SCENE_NODE_COUNT_FILE = "..\\data\\save_scenes\\node_count.txt";
INIT STATICVALS::SAVE_SCENE_FOLDER = "..\\data\\save_scenes\\";
INIT STATICVALS::SAVE_SCENE_OSG_FOLDER = "..\\data\\save_scenes\\osg_files\\";

INIT STATICVALS::DATA_FOLDER = "..\\data\\";

INIT STATICVALS::GM_MO = "Geometry:MO:";
INIT STATICVALS::G_MO = "Group:MO:";
INIT STATICVALS::MT_MO = "MatrixTransform:MO:";

INIT STATICVALS::S_MO = "Switch:MO:";
INIT STATICVALS::S_SO = "Switch:SO:";
INIT STATICVALS::GEO_SO_M = "Gode::SO:M:";
INIT STATICVALS::GEO_SO_R = "Gode::SO:R:";
INIT STATICVALS::GEO_SO_S = "Gode::SO:S:";

/*-------------------------------------------------
Globabl function used to mimic static float consts
-------------------------------------------------*/

float STATICVALS::SQUARE_DIM()
{
	return 1.0f;
}

float STATICVALS::GLOBAL_MOVE_SPEED()
{
	return 0.1f;
}

float STATICVALS::WIREFRAME_SPHERE_RADIOUS()
{
	return 1.6f;	
}

float STATICVALS::DRAGER_SCALE()
{
	return 4.0f;
}
