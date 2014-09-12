#ifndef _STATICVALS_
#define _STATICVALS_

#include "stdafx.h"

using namespace std;

class STATICVALS
{

public:

		//shape sizes
		static float SQUARE_DIM();
		static float GLOBAL_MOVE_SPEED();
		static float WIREFRAME_SPHERE_RADIOUS();
		static float DRAGER_SCALE();

		static const string INI_FILE;
		static const string ROBOT_CONTROL;
		
		static const string SCENE_OBJECT_FOLDER;
		static const string SAVED_SCENE_NODE_COUNT_FILE;
		static const string SAVE_SCENE_FOLDER;
		static const string SAVE_SCENE_OSG_FOLDER;
		static const string DATA_FOLDER;
		
		//OSG Node Name Types
		static const string MT_MO;
		static const string GM_MO;
		static const string G_MO;
		static const string S_MO;

		static const string S_SO; 
		static const string GEO_SO_M; 
		static const string GEO_SO_R; 
		static const string GEO_SO_S;

		//menu
		static const int MENU_BLINK_ITERATIONS = 10;
};

#endif