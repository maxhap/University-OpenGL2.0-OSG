// raaCGAssignment1-2012.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "raaCGAssignment1-2012.h"
#include <Windows.h>
#include "maSceneIO.h"
/*------------------------------------------------------------*/
//Rob Variables
/*------------------------------------------------------------*/
int g_iWidth=0;
int g_iHeight=0;
unsigned long g_ulGrid=0;
int g_iMainMenu=0;
int g_iNavMenu=0;
int g_iMainMenuLight=0;
int g_iMainMenuObject=0;
raaCamera g_Camera;

/*------------------------------------------------------------*/
//Max Variables
/*------------------------------------------------------------*/
maStructList* _plSceneList;
maStructList* _plCurrentSceneList;
maStruListElement* _pCurrentSceneElement;
unsigned int _uiCurrentScene;

float _fIdelRotationValue = 0;

//display lists
GLuint _gluiWireframeDL;
GLuint _gluiSolidDL;

//file selection
const int OBJECTFILTER = 0;
const int TEXTUREFILTER = 1;
const int SCENEFILTER = 2;

//for glut menu
int _iAddMenu;
const int _iAddTriangle = 1;
const int _iAddLitTeapot = 2;
const int _iAddLitSphere = 3;
const int _iAddPossLight = 4;
const int _iAddSmileyQuad = 20;
const int _iAddClientCube = 21;

int _iSceneControlMenu; 
const int _iAddFromObjectFile = 10;
const int _iForwardScene = 11;
const int _iBackScene = 12;
const int _iClearScene = 5;
const int _iSaveScene = 22;
const int _iLoadScene = 23;

int _iAddSeletionControlsMenu;
const int _iRotateX = 6;
const int _iRotateY = 7;
const int _iRotateZ = 8;
const int _iRemoveObjects = 9;
const int _iTogleWireFrameMode = 50;

//light menu
int _iAddLightControlsMenu;
const int _iTogleLightOnOff = 123123;

// Picking Stuff
#define RENDER	1
#define SELECT	2
#define BUFSIZE 1024
GLuint selectBuf[BUFSIZE];
GLint _uiHits;
int _iMode = RENDER;
int _iCursorX;
int _iCursorY;

//camera stuff
unsigned int _uiRecalcuateSelectionLookAt;
unsigned int _uiEploreMode;
unsigned int _uiFlySet = 0;

int main(int argc, char* argv[])
{
	mathsInit();
	glutInit(&argc, argv); 

	// create a glut window
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1200,1000);
	glutCreateWindow("raaCGAssignment");

	// my init function
	init();
	max_init();

	// associate functions with glut actions
	glutDisplayFunc(display); // display function
	glutIdleFunc(idle); // simulation loop
	glutReshapeFunc(reshape); // reshape function 
	glutKeyboardFunc(keyboard); // basic keyboard entry
	glutSpecialFunc(sKeyboard); // extended keyboard function
	glutMouseFunc(mouse); // mouse events
	glutMotionFunc(motion); // mouse motion

	build_menu();

	// hand control over to glut
	glutMainLoop();

	return 0;
}

void build_menu()
{
	// build glut menu
	g_iNavMenu=glutCreateMenu(navMenu);
	glutAddMenuEntry("Explore", csg_uiExplore);
	glutAddMenuEntry("Fly", csg_uiFly);

	_iAddMenu = glutCreateMenu( objectMenu );
	glutAddMenuEntry( "Triangle", _iAddTriangle );
	glutAddMenuEntry( "Lit Teapot", _iAddLitTeapot );
	glutAddMenuEntry( "Lit Sphere", _iAddLitSphere );
	glutAddMenuEntry( "Positioned Light", _iAddPossLight );
	glutAddMenuEntry( "Add Object File (.obj)", _iAddFromObjectFile );
	glutAddMenuEntry( "Add Textured Quad (.bmp)", _iAddSmileyQuad );
	glutAddMenuEntry( "Add Client Cube", _iAddClientCube );

	_iSceneControlMenu = glutCreateMenu( objectMenu );
	glutAddMenuEntry( "Clear Scene", _iClearScene );
	glutAddMenuEntry( "Forward Scene", _iForwardScene );
	glutAddMenuEntry( "Back Scene", _iBackScene );
	glutAddMenuEntry( "Save Scene", _iSaveScene );
	glutAddMenuEntry( "Load Scene", _iLoadScene );

	g_iMainMenu=glutCreateMenu( objectMenu );
	glutAddSubMenu("Navigation", g_iNavMenu);

	glutAddSubMenu( "Add Object", _iAddMenu );	

	glutAddSubMenu( "Scene Controls", _iSceneControlMenu );

	glutAttachMenu(GLUT_MIDDLE_BUTTON);


	//build glut light menu
	g_iMainMenuLight = glutCreateMenu( objectMenu );

	glutAddMenuEntry( "Toggle Light", _iTogleLightOnOff );
	glutAddMenuEntry( "Remove Light", _iRemoveObjects );

	_iAddSeletionControlsMenu = glutCreateMenu( objectMenu );
	glutAddMenuEntry( "Rotate X", _iRotateX );
	glutAddMenuEntry( "Rotate Y", _iRotateY );
	glutAddMenuEntry( "Rotate Z", _iRotateZ );
	glutAddMenuEntry( "Remove Objects", _iRemoveObjects );
	glutAddMenuEntry( "Toggle Wire Frame", _iTogleWireFrameMode );

	//object menu
	g_iMainMenuObject = glutCreateMenu( objectMenu );
	glutAddSubMenu( "Object Controls", _iAddSeletionControlsMenu );
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	applyCamera(g_Camera);
	glCallList(g_ulGrid);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// add scene rendering code here
	max_render();

	glPopAttrib();

	glFlush();
	glutSwapBuffers();
}

void idle()
{
	_fIdelRotationValue += 0.1;
	if(g_Camera.m_bMotion) camRot(g_Camera, 0.0001f*g_Camera.m_afMouseDisp[0], 0.0001f*g_Camera.m_afMouseDisp[1]);
	glutPostRedisplay();
}

void reshape(int iWidth, int iHeight)
{
	g_iWidth=iWidth;
	g_iHeight=iHeight;

	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, ((float)iWidth)/((float)iHeight), 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void keyboard(unsigned char c, int iXPos, int iYPos)
{
	switch(c)
	{
		case 'w':
		case 'W':
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanVert(g_Camera, 0.1f);
			else camTravel(g_Camera, -0.1f);
			break;
		case 's':
		case 'S':
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanVert(g_Camera, -0.1f);
			else camTravel(g_Camera, 0.1f);
			break;
		case 'a':
		case 'A':
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanHori(g_Camera, -0.1f);
			break;
		case 'd':
		case 'D':
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanHori(g_Camera, 0.1f);
			break;
		case 'r':
		case 'R':
			camInit(g_Camera);
			break;
		case 27:			
			break;
	}
	glutPostRedisplay();
}

void sKeyboard(int iC, int iXPos, int iYPos)
{
	switch(iC)
	{
		case GLUT_KEY_UP:
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanVert(g_Camera, 0.1f);
			else camTravel(g_Camera, -0.8f);
			break;
		case GLUT_KEY_DOWN:
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanVert(g_Camera, -0.1f);
			else camTravel(g_Camera, 0.8f);
			break;
		case GLUT_KEY_LEFT:
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanHori(g_Camera, -0.1f);
			else camRot( g_Camera, -0.05f, 0.0f );
			break;
		case GLUT_KEY_RIGHT:
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) camPanHori(g_Camera, 0.1f);
			else camRot( g_Camera, 0.05f, 0.0f );
			break;
	}
	glutPostRedisplay();
}

void mouse(int iKey, int iEvent, int iXPos, int iYPos)
{
	if(iKey==GLUT_LEFT_BUTTON && iEvent==GLUT_DOWN)
	{
		g_Camera.m_bMotion=true;
		g_Camera.m_aiLastMousePos[0]=iXPos; 
		g_Camera.m_aiLastMousePos[1]=iYPos;
		g_Camera.m_afMouseDisp[0]=0.0f;
		g_Camera.m_afMouseDisp[1]=0.0f;
	}
	else if(iKey==GLUT_RIGHT_BUTTON && iEvent==GLUT_DOWN)
	{
		// suggest this is where you implement selection
		_iCursorX = iXPos;
		_iCursorY = iYPos;
		_iMode = SELECT;

	}
	else if(iKey==GLUT_LEFT_BUTTON && iEvent==GLUT_UP)
	{
		g_Camera.m_bMotion=false;
	}
}

void motion(int iXPos, int iYPos)
{
	if(g_Camera.m_bMotion)
	{
		g_Camera.m_afMouseDisp[0]=(((float)iXPos)-((float)g_Camera.m_aiLastMousePos[0]));
		g_Camera.m_afMouseDisp[1]=-(((float)iYPos)-((float)g_Camera.m_aiLastMousePos[1]));
		glutPostRedisplay();
	}
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_LIGHT0 );
	makeGrid(g_ulGrid, 100.0f, 100.0f, 100, 100);
	camInit(g_Camera);
}

void mainMenu(int i)
{
	switch( i )
	{
		case _iClearScene: remove_all_shapes( *_plCurrentSceneList ); break;
	}
}

void navMenu(int i)
{
	float* pfAvPos = get_average_selected_possition( *_plCurrentSceneList );

	switch(i)
	{
	case 1:
		_uiFlySet = 1;
		_uiEploreMode = 0;
		break;
	default:
		_uiFlySet = 0;
		_uiEploreMode = 1;
		_uiRecalcuateSelectionLookAt = 1;
	break;
	}

	delete pfAvPos;
}

/*
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Max Stuff
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void selection_menu( unsigned int iMenuToSet )
{
	switch ( iMenuToSet )
	{
	case 0:	glutSetMenu( g_iMainMenu ); break;
	case 1: glutSetMenu( g_iMainMenuLight );break;
	case 2: glutSetMenu( g_iMainMenuObject );break;
	}

	glutAttachMenu( GLUT_MIDDLE_BUTTON );
}

/*--------------------------------------------
Author: Max Ashton
Description: Uses the win api to open a dialog box for file selection.
			 Filters passed by id then converted using switch to limit file types
----------------------------------------------*/
 std::string getFileName( unsigned int filter )
{	
	TCHAR szFilters[] = _T( "All Files (*.*)\0*.*\0\0" );
	TCHAR szFiltersObject[] = _T( "Object Files(*.obj)\0*.obj\0\0" );
	TCHAR szFiltersScene[] = _T( "Scene Files (*.scn)\0*.scn\0\0" );
	TCHAR szFiltersTexture[] = _T( "Bitmap Files (*.bmp)\0*.bmp\0\0" );

	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;

	switch( filter )
	{
		case OBJECTFILTER: ofn.lpstrFilter = szFiltersObject; break;
		case SCENEFILTER: ofn.lpstrFilter = szFiltersScene; break;
		case TEXTUREFILTER: ofn.lpstrFilter = szFiltersTexture; break;
		default: ofn.lpstrFilter = szFilters; break;
	}
	
	ofn.lpstrFile = szFilePathName;  // This will the file name
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T( "Open File" );
	ofn.Flags = OFN_FILEMUSTEXIST;

	// Bring up the dialog, and choose the file
	::GetOpenFileName(&ofn);

	char buffer[200];

	for( int i = 0; i < 200; i++ )
	{
		buffer[i] = NULL;
	}

	wcstombs( buffer, ofn.lpstrFile, sizeof( buffer ) );	
	
	return buffer;
}

/*--------------------------------------------
Author: Max Ashton
Description: Uses the win api to open a dialog box for file selection.
		     Filters passed by id then converted using switch to limit file types
----------------------------------------------*/
std::string getSaveFileName( unsigned int filter )
{	
	TCHAR szFilters[] = _T( "All Files (*.*)\0*.*\0\0" );
	TCHAR szFiltersObject[] = _T( "Object File (*.obj)\0*.obj\0\0" );
	TCHAR szFiltersScene[] = _T( "Scene Files(*.scn)\0*.scn\0\0" );
	TCHAR szFiltersTexture[] = _T( "Bitmap Files(*.bmp)\0*.bmp\0\0" );

	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;

	switch( filter )
	{
	case OBJECTFILTER: ofn.lpstrFilter = szFiltersObject; break;
	case SCENEFILTER: ofn.lpstrFilter = szFiltersScene; break;
	case TEXTUREFILTER: ofn.lpstrFilter = szFiltersTexture; break;
	default: ofn.lpstrFilter = szFilters; break;
	}

	ofn.lpstrFile = szFilePathName;  // This will the file name
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T( "Save File" );
	ofn.Flags = OFN_FILEMUSTEXIST;

	// Bring up the dialog, and choose the file
	::GetSaveFileName(&ofn);

	char buffer[200];

	for( int i = 0; i < 200; i++ )
	{
		buffer[i] = NULL;
	}

	wcstombs( buffer, ofn.lpstrFile, sizeof( buffer ) );	

	return buffer;
}

/*--------------------------------------------
Author: Max Ashton
Description: Using a switch statement the correct functions are called based of whats returned by the
		     glut menu
----------------------------------------------*/
void objectMenu( int i )
{
	float* ofPossition = new float[4];
	vecProject( g_Camera.m_fVP, g_Camera.m_fVD, 20, ofPossition );

	switch( i )
	{
		case _iAddTriangle: add_triangle( *_plCurrentSceneList, ofPossition[0], ofPossition[1], ofPossition[2] ); break;		
		case _iAddLitTeapot: add_lit_teapot( *_plCurrentSceneList, ofPossition[0], ofPossition[1], ofPossition[2] ); break;
		case _iAddLitSphere: add_lit_sphere( *_plCurrentSceneList, ofPossition[0], ofPossition[1], ofPossition[2] ); break;
		case _iAddPossLight: add_possitioned_light( *_plCurrentSceneList, ofPossition[0], ofPossition[1], ofPossition[2] ); break;
		case _iAddFromObjectFile: add_model( *_plCurrentSceneList, ofPossition[0], ofPossition[1], ofPossition[2], getFileName( OBJECTFILTER ) );break;
		case _iAddSmileyQuad: add_smiley_quad( *_plCurrentSceneList, ofPossition[0], ofPossition[1], ofPossition[2], getFileName( TEXTUREFILTER ), 300, 300 );break;
		case _iAddClientCube: add_client_cube( *_plCurrentSceneList, ofPossition[0], ofPossition[1], ofPossition[2] ); break;
		
		//scene control
		case _iClearScene: remove_all_shapes( *_plCurrentSceneList ); break;
		case _iForwardScene: forward_scene(); break;
		case _iBackScene: back_scene(); break;
		case _iSaveScene: save_scene( *_plCurrentSceneList, getSaveFileName( SCENEFILTER ) );break;
		case _iLoadScene: load_scene( *_plCurrentSceneList, getFileName( SCENEFILTER ) );

		//selection menu
		case _iRotateX: set_rotate( *_plCurrentSceneList, ROTATEX, ROTATE );break;
		case _iRotateY: set_rotate( *_plCurrentSceneList, ROTATEY, ROTATE );break;
		case _iRotateZ: set_rotate( *_plCurrentSceneList, ROTATEZ, ROTATE );break;
		case _iRemoveObjects: remove_selected_shapes( *_plCurrentSceneList );break;	
		case _iTogleWireFrameMode: togel_wire_frame_for_selected( *_plCurrentSceneList );break;	
		case _iTogleLightOnOff: togel_light_for_selected( *_plCurrentSceneList );break;	
		default: std::cout << "No operation specified";
	}

	delete ofPossition;
}

/*--------------------------------------------
Author: Max Ashton
Description: Initialize the lists/scenes and add a few starter objects
			 also global initialize display lists
----------------------------------------------*/
void max_init()
{
	create_drawmode_displaylists();

	_uiEploreMode = 0;
	_plSceneList = new maStructList();

	add_element_tail( *_plSceneList, new maStructList(), LISTOFLISTS ); 

	_pCurrentSceneElement = _plSceneList->_psListTail;
	_plCurrentSceneList = ( maStructList* ) ( _pCurrentSceneElement->_pvData );

	add_triangle( *_plCurrentSceneList, 0.0f, 0.0f, -10.0f );
	add_triangle( *_plCurrentSceneList, 0.0f, 3.0f, -10.0f );
	add_triangle( *_plCurrentSceneList, 0.0f, 6.0f, -10.0f );

};

/*--------------------------------------------
Author: Max Ashton
Description: Move the back the scene by getting the after node from the current sceneElement
		     if at tail of list add a new item to tail increasing list
----------------------------------------------*/
void forward_scene()
{
	_uiCurrentScene++;

	if( _pCurrentSceneElement->_psNext == NULL )
	{
		add_element_tail( *_plSceneList, new maStructList(), LISTOFLISTS ); 
		std::cout << "New scene added to tail";
	}

	_pCurrentSceneElement = _pCurrentSceneElement->_psNext;
	_plCurrentSceneList = ( maStructList* ) ( _pCurrentSceneElement->_pvData );

	std::cout << "Current Scene: " << _uiCurrentScene << "\n";
}

/*--------------------------------------------
Author: Max Ashton
Description: Move the back the scene by getting the before node from the current sceneElement
		     if at head of list add a new item to head increasing list
----------------------------------------------*/
void back_scene()
{
	if( _uiCurrentScene != 0 )
	{
		_uiCurrentScene--;
	}	

	if( _pCurrentSceneElement->_psBefore == NULL )
	{
		add_element_head( *_plSceneList, new maStructList(), LISTOFLISTS ); 
		std::cout << "New scene added to head";
	}

	_pCurrentSceneElement = _pCurrentSceneElement->_psBefore;
	_plCurrentSceneList = ( maStructList* ) ( _pCurrentSceneElement->_pvData );	

	std::cout << "Current Scene: " << _uiCurrentScene << "\n";
}

void set_camera()
{
	if( _uiEploreMode != 0 )
	{
		if(  _uiRecalcuateSelectionLookAt != 0 )
		{
			_uiRecalcuateSelectionLookAt = 0;
			float* pfAvPos = get_average_selected_possition( *_plCurrentSceneList );
			camToExplore(g_Camera, pfAvPos, 40, display);
		}		
	}
	else
	{
		if( _uiFlySet != 0 )
		{
			_uiFlySet = 0;
			camToFly( g_Camera );
		}
		
	}
}
/*--------------------------------------------
Author: Max Ashton
Description: My rendering function called at the end of robs rendering function
			 Adds an ambient light to the enter scene.
			 Handles picking if the picking mode has been activated ( right mouse click )
----------------------------------------------*/
void max_render()
{
	//determin camera type based on selection
	set_camera();

	//move this update to proper glut function
	update_objects( *_plCurrentSceneList );

	glPushMatrix();

	//Add ambient light
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	if ( _plCurrentSceneList->_iListSize > 0 && _iMode == SELECT)
	{
		start_picking();
	}

	//
	draw_list();
	
	if ( _plCurrentSceneList->_iListSize > 0 &&_iMode == SELECT ) 
	{
		stop_picking();
	}

	glPopMatrix();
}

/*--------------------------------------------
Author: Max Ashton
Description: Rotates the matrix on the x and y axis
			 data stored in a pointer array size 3
----------------------------------------------*/
void rotate_matrix( float* pfData )
{
	//x y z
	glRotatef( pfData[0], 1.0f, 0.0f, 0.0f );
	glRotatef( pfData[1], 0.0f, 1.0f, 0.0f );
	glRotatef( pfData[2], 0.0f, 0.0f, 1.0f );
}

/*--------------------------------------------
Author: Max Ashton
Description: Sets the attributes to draw in wire frame mode
		     function called create_drawmode_displaylists
----------------------------------------------*/
void draw_in_wireframe()
{
	glPolygonMode( GL_FRONT, GL_LINE );
	glPolygonMode( GL_BACK, GL_LINE );
}

/*--------------------------------------------
Author: Max Ashton
Description: Sets the attributes to draw in wire solid mode
		     function called from create_drawmode_displaylists
----------------------------------------------*/
void draw_in_solid()
{
	glPolygonMode( GL_FRONT, GL_FILL );
	glPolygonMode( GL_BACK, GL_FILL );
}

void create_drawmode_displaylists()
{
	_gluiWireframeDL = glGenLists(1);	
	glNewList( _gluiWireframeDL, GL_COMPILE );

	draw_in_wireframe();

	glEndList();

	_gluiSolidDL = glGenLists(1);	
	glNewList( _gluiSolidDL, GL_COMPILE );

	draw_in_solid();

	glEndList();
}


/*--------------------------------------------
Author: Max Ashton
Description: Draw a triangle with no lighting 
			 gl used to disable lighting and enable blending for use of colors
			 simple glbeing and gl end used to set colour and vertices. 
			 Translation and rotation applied before drawing so object appears in correct place at
			 the rotated orientation.
----------------------------------------------*/
void draw_traingle( ObjectData* pvData )
{
	glPushMatrix();

	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	pvData->basicData->iDrawWireFrame ?  glCallList( _gluiWireframeDL ) : glCallList( _gluiSolidDL );

	glTranslatef( pvData->basicData->pfPossitionData[0], pvData->basicData->pfPossitionData[1], pvData->basicData->pfPossitionData[2] );
	rotate_matrix( pvData->basicData->faRoataionData );
	
	glBegin( GL_TRIANGLES );

	glNormal3f( 0.25f, 0.25f, 1.0f );

	glColor3f( 0.9f, 0.0f, 0.0f );
	glVertex4f( -1.0f, 0.0f, 0.0f, 1.0f );
	glColor3f( 0.0f, 0.9f, 0.0f );
	glVertex4f( 1.0f, 0.0f, 0.0f, 1.0f );
	glColor3f( 0.0f, 0.0f, 0.9f );
	glVertex4f( 0.0f, 1.0f, 0.0f, 1.0f );

	glEnd();

	glDisable( GL_BLEND );
	glEnable(GL_LIGHTING );
	glPopMatrix();
}

/*--------------------------------------------
Author: Max Ashton
Description: Simply call glut's function to draw a sphere
			 Material values come from info stored in the list.
			 Translation and rotation applied before drawing so object appears in correct place at
			 the rotated orientation.
----------------------------------------------*/
void draw_lit_sphere( ObjectData* pvData )
{
	glPushMatrix();

	pvData->basicData->iDrawWireFrame ?  glCallList( _gluiWireframeDL ) : glCallList( _gluiSolidDL );

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, 
		pvData->pfAmbiantMaterialData );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, pvData->pfDiffusedMaterialData ); 
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, pvData->pfSpectularMaterialData ); 
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 100.0f );

	glTranslatef( pvData->basicData->pfPossitionData[0], pvData->basicData->pfPossitionData[1], pvData->basicData->pfPossitionData[2] );
	rotate_matrix( pvData->basicData->faRoataionData );

	glutSolidSphere( 1.0f * pvData->basicData->fScale, 20.0f, 20.0f );

	glPopAttrib(); 
	glPopMatrix();
}

/*--------------------------------------------
Author: Max Ashton
Description: Simply call glut's function to draw a teapot
			 Material values come from info stored in the list.
			 Translation and rotation applied before drawing so object appears in correct place at
			 the rotated orientation.
----------------------------------------------*/

void draw_lit_teapot( ObjectData* pvData )
{
	glPushMatrix();

	pvData->basicData->iDrawWireFrame ?  glCallList( _gluiWireframeDL ) : glCallList( _gluiSolidDL );

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, pvData->pfAmbiantMaterialData );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, pvData->pfDiffusedMaterialData ); 
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, pvData->pfSpectularMaterialData ); 
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 100.0f );

	glTranslatef( pvData->basicData->pfPossitionData[0], pvData->basicData->pfPossitionData[1], pvData->basicData->pfPossitionData[2] );
	rotate_matrix( pvData->basicData->faRoataionData );

	glutSolidTeapot( 1.0f );

	glPopAttrib(); 
	glPopMatrix();
}

/*--------------------------------------------
Author: Max Ashton
Description: Simply call glut's function to draw a sphere
			 Material calues come from info stored in the list.
			 Translation and rotation applied before drawing so object appears in correct place at
			 the rotated orientation in this case it is the same as the original object that is selected.

----------------------------------------------*/
void draw_selection_sphere( float* pfPossitionData, float fScale )
{
	glPushMatrix();
	glPolygonMode( GL_FRONT, GL_LINE );
	glPolygonMode( GL_BACK, GL_LINE );

	GLfloat am[] = {0.0f, 1.0f, 0.0f, 0.0f};
	GLfloat diff[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat spec[] = {0.0f, 0.0f, 0.0f, 0.0f};

	//DISPLIST
	glPushMatrix();
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, am);
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diff); 
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, spec ); 
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 100.0f );

	glTranslatef( pfPossitionData[0], pfPossitionData[1], pfPossitionData[2] );
	glRotatef( _fIdelRotationValue, 0.0f, 1.0f, 0.0f );

	glutSolidSphere( 1.5f * fScale, 20.0f, 20.0f );

	glPopAttrib(); 
	glPopMatrix();
	glPolygonMode( GL_FRONT, GL_FILL );
	glPolygonMode( GL_BACK, GL_FILL );

	glPopMatrix();
}

/*--------------------------------------------
Author: Max Ashton
Description: Using a switch statement the correct GLlight is used based on my light ID held in the list			 
			 Translation and rotation applied before drawing so object appears in correct place at
			 the rotated orientation.
			 A simple sphere is then drawn using the light located but slightly lower
----------------------------------------------*/
void draw_possitioned_light( ObjectData* pvData )
{
	float light = pvData->basicData->iObjectID;

	//Add positioned light
	GLfloat lightColor0[] = {pvData->pfDiffusedMaterialData[0], pvData->pfDiffusedMaterialData[1], pvData->pfDiffusedMaterialData[2], pvData->pfDiffusedMaterialData[3]};
	GLfloat lightPos0[] = {pvData->basicData->pfPossitionData[0], pvData->basicData->pfPossitionData[1] + 5.0f, pvData->basicData->pfPossitionData[2], pvData->basicData->pfPossitionData[3]};

	switch( pvData->basicData->uiLightID )
	{
		case 0:
			if( pvData->basicData->uiLightOn != 0 )
			{
				glEnable( GL_LIGHT0 );
				glLightfv( GL_LIGHT0, GL_DIFFUSE, lightColor0 );
				glLightfv( GL_LIGHT0, GL_POSITION, lightPos0 );
			}
			else
			{
				glDisable( GL_LIGHT0 );
			}

		break;

		case 1:
			if( pvData->basicData->uiLightOn != 0 )
			{
				glEnable( GL_LIGHT1 );
				glLightfv( GL_LIGHT1, GL_DIFFUSE, lightColor0 );
				glLightfv( GL_LIGHT1, GL_POSITION, lightPos0 );
			}
			else
			{
				glDisable( GL_LIGHT1 );
			}
			break;

		case 2:
			if( pvData->basicData->uiLightOn != 0 )
			{
				glEnable( GL_LIGHT2 );
				glLightfv( GL_LIGHT2, GL_DIFFUSE, lightColor0 );
				glLightfv( GL_LIGHT2, GL_POSITION, lightPos0 );
			}
			else
			{
				glDisable( GL_LIGHT2 );
			}
			break;

		case 3:
			if( pvData->basicData->uiLightOn != 0 )
			{
				glEnable( GL_LIGHT3 );
				glLightfv( GL_LIGHT3, GL_DIFFUSE, lightColor0 );
				glLightfv( GL_LIGHT3, GL_POSITION, lightPos0 );
			}
			else
			{
				glDisable( GL_LIGHT3 );
			}
			break;

		case 4:
			if( pvData->basicData->uiLightOn != 0 )
			{
				glEnable( GL_LIGHT4 );
				glLightfv( GL_LIGHT4, GL_DIFFUSE, lightColor0 );
				glLightfv( GL_LIGHT4, GL_POSITION, lightPos0 );
			}
			else
			{
				glDisable( GL_LIGHT4 );
			}
			break;

		case 5:
			if( pvData->basicData->uiLightOn != 0 )
			{
				glEnable( GL_LIGHT5 );
				glLightfv( GL_LIGHT5, GL_DIFFUSE, lightColor0 );
				glLightfv( GL_LIGHT5, GL_POSITION, lightPos0 );
			}
			else
			{
				glDisable( GL_LIGHT5 );
			}
			break;
	}

	draw_lit_sphere( pvData );
}

/*--------------------------------------------
Author: Max Ashton
Description: Texture mapped to a simple quad consisting of 4 vertices glbegin and glend
			 used to specify vertices 
----------------------------------------------*/
void draw_textured_quad( ObjectData* pvData )
{
	glPushMatrix();

	pvData->basicData->iDrawWireFrame ?  glCallList( _gluiWireframeDL ) : glCallList( _gluiSolidDL );

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, pvData->guiTextureID );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glTranslatef( pvData->basicData->pfPossitionData[0], pvData->basicData->pfPossitionData[1], pvData->basicData->pfPossitionData[2] );
	rotate_matrix( pvData->basicData->faRoataionData );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glBegin(GL_QUADS);
	glColor3f( 1.0f, 1.0f, 1.0f );

	glNormal3f( 0.0, 0.0f, 1.0f);
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( -1.0f, -1.0f * pvData->fAspectRatio, 0.0f );
	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( 1.0f, -1.0f * pvData->fAspectRatio, 0.0f );
	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( 1.0f, 1.0f * pvData->fAspectRatio, 0.0f );
	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( -1.0f, 1.0f * pvData->fAspectRatio, 0.0f );

	glEnd();	

	glDisable( GL_TEXTURE_2D );

	glPopMatrix();
}

/*--------------------------------------------
Author: Max Ashton
Description: Cube using client side gl functions. 8 x 3 glfloat array constructed to pass the cuves vertices and then 6 x 6 
		     indices's array created to tell opengl which vertices to use
----------------------------------------------*/

GLfloat aVertices[] = {-1.0f, -1.0f, 1.0f, 
	-1.0f, 1.0f, 1.0f,				  
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,						  
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,						   
	1.0f, 1.0f, -1.0f};         


GLubyte aIndices[] = {0,1,2, 2,3,0, 
	0,3,4, 4,5,0,
	0,5,6, 6,1,0,
	1,6,7, 7,2,1,
	7,4,3, 3,2,7,
	4,7,6, 6,5,4};

unsigned int bNormalsCalculated = 0;

GLfloat aNormals[] = {0.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f,				  
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,						  
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,						   
	0.0f, 0.0f, 0.0f};

/* Normal Calculation
0. Normalize ( Cross(0,3) + Cross(0,1) + Cross(0,5) )
1. Normalize ( Cross(1,2) + Cross(1,6) + Cross(1,0) )
2. Normalize ( Cross(2,1) + Cross(2,3) + Cross(2,7) )
3. Normalize ( Cross(3,4) + Cross(3,2) + Cross(3,0) )
4. Normalize ( Cross(4,3) + Cross(4,7) + Cross(4,5) )
5. Normalize ( Cross(5,4) + Cross(5,0) + Cross(5,6) )
6. Normalize ( Cross(6,7) + Cross(6,1) + Cross(6,5) )
7. Normalize ( Cross(6,2) + Cross(7,6) + Cross(7,4) )
*/
void calculate_cube_normals()
{
	float* pfV1 = new float[4];
	float* pfV2 = new float[4];
	float* pfOutcome1 = new float[4];
	float* pfOutcome2 = new float[4];
	float* pfOutcome3 = new float[4];
	float* pfAddRes = new float[4];
	float* pfAddRes2 = new float[4];
	float* pfNormalized = new float[4];

	//0
	pfV1[0] = aVertices[0];
	pfV1[1] = aVertices[1];
	pfV1[2] = aVertices[2];

	pfV2[0] = aVertices[9];
	pfV2[1] = aVertices[10];
	pfV2[2] = aVertices[11];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[3];
	pfV2[1] = aVertices[4];
	pfV2[2] = aVertices[5];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[16];
	pfV2[1] = aVertices[17];
	pfV2[2] = aVertices[18];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes2 );

	vecNormalise( pfAddRes2, pfNormalized );

	aNormals[0] = pfNormalized[0];
	aNormals[1] = pfNormalized[1];
	aNormals[2] = pfNormalized[2];

	//1
	pfV1[0] = aVertices[3];
	pfV1[1] = aVertices[4];
	pfV1[2] = aVertices[5];

	pfV2[0] = aVertices[6];
	pfV2[1] = aVertices[7];
	pfV2[2] = aVertices[8];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[18];
	pfV2[1] = aVertices[19];
	pfV2[2] = aVertices[20];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[0];
	pfV2[1] = aVertices[1];
	pfV2[2] = aVertices[2];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes );

	vecNormalise( pfAddRes, pfNormalized );

	aNormals[3] = pfNormalized[0];
	aNormals[4] = pfNormalized[1];
	aNormals[5] = pfNormalized[2];

	//2
	pfV1[0] = aVertices[6];
	pfV1[1] = aVertices[7];
	pfV1[2] = aVertices[8];

	pfV2[0] = aVertices[3];
	pfV2[1] = aVertices[4];
	pfV2[2] = aVertices[5];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[9];
	pfV2[1] = aVertices[10];
	pfV2[2] = aVertices[11];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[21];
	pfV2[1] = aVertices[22];
	pfV2[2] = aVertices[23];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes );

	vecNormalise( pfAddRes, pfNormalized );

	aNormals[6] = pfNormalized[0];
	aNormals[7] = pfNormalized[1];
	aNormals[8] = pfNormalized[2];

	//3
	pfV1[0] = aVertices[9];
	pfV1[1] = aVertices[10];
	pfV1[2] = aVertices[11];

	pfV2[0] = aVertices[12];
	pfV2[1] = aVertices[13];
	pfV2[2] = aVertices[14];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[6];
	pfV2[1] = aVertices[7];
	pfV2[2] = aVertices[8];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[0];
	pfV2[1] = aVertices[1];
	pfV2[2] = aVertices[2];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes );

	vecNormalise( pfAddRes, pfNormalized );

	aNormals[9] = pfNormalized[0];
	aNormals[10] = pfNormalized[1];
	aNormals[11] = pfNormalized[2];

	//4
	pfV1[0] = aVertices[12];
	pfV1[1] = aVertices[13];
	pfV1[2] = aVertices[14];

	pfV2[0] = aVertices[9];
	pfV2[1] = aVertices[10];
	pfV2[2] = aVertices[11];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[21];
	pfV2[1] = aVertices[22];
	pfV2[2] = aVertices[23];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[15];
	pfV2[1] = aVertices[16];
	pfV2[2] = aVertices[17];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes );

	vecNormalise( pfAddRes, pfNormalized );

	aNormals[12] = pfNormalized[0];
	aNormals[13] = pfNormalized[1];
	aNormals[14] = pfNormalized[2];

	//5

	pfV1[0] = aVertices[15];
	pfV1[1] = aVertices[16];
	pfV1[2] = aVertices[17];

	pfV2[0] = aVertices[12];
	pfV2[1] = aVertices[13];
	pfV2[2] = aVertices[14];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[0];
	pfV2[1] = aVertices[1];
	pfV2[2] = aVertices[2];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[18];
	pfV2[1] = aVertices[19];
	pfV2[2] = aVertices[20];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes );

	vecNormalise( pfAddRes, pfNormalized );

	aNormals[15] = pfNormalized[0];
	aNormals[16] = pfNormalized[1];
	aNormals[17] = pfNormalized[2];

	// 6

	pfV1[0] = aVertices[18];
	pfV1[1] = aVertices[19];
	pfV1[2] = aVertices[20];

	pfV2[0] = aVertices[21];
	pfV2[1] = aVertices[22];
	pfV2[2] = aVertices[23];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[3];
	pfV2[1] = aVertices[4];
	pfV2[2] = aVertices[5];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[15];
	pfV2[1] = aVertices[16];
	pfV2[2] = aVertices[17];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes );

	vecNormalise( pfAddRes, pfNormalized );

	aNormals[18] = pfNormalized[0];
	aNormals[19] = pfNormalized[1];
	aNormals[20] = pfNormalized[2];

	// 7

	pfV1[0] = aVertices[21];
	pfV1[1] = aVertices[22];
	pfV1[2] = aVertices[23];

	pfV2[0] = aVertices[6];
	pfV2[1] = aVertices[7];
	pfV2[2] = aVertices[8];
	vecCrossProduct( pfV1, pfV2, pfOutcome1 ); 

	pfV2[0] = aVertices[18];
	pfV2[1] = aVertices[19];
	pfV2[2] = aVertices[20];
	vecCrossProduct( pfV1, pfV2, pfOutcome2 );

	pfV2[0] = aVertices[12];
	pfV2[1] = aVertices[13];
	pfV2[2] = aVertices[14];
	vecCrossProduct( pfV1, pfV2, pfOutcome3 ); 

	vecAdd( pfOutcome1, pfOutcome2, pfAddRes );
	vecAdd( pfAddRes, pfOutcome3, pfAddRes );

	vecNormalise( pfAddRes, pfNormalized );

	aNormals[21] = pfNormalized[0];
	aNormals[22] = pfNormalized[1];
	aNormals[23] = pfNormalized[2];

	delete pfV1; 
	delete pfV2; 
	delete pfOutcome1; 
	delete pfOutcome2; 
	delete pfOutcome3; 
	delete pfAddRes; 
	delete pfAddRes2;
	delete pfNormalized;

}
void draw_client_cube( ObjectData* pvData )
{
	glPushMatrix();

	pvData->basicData->iDrawWireFrame ?  glCallList( _gluiWireframeDL ) : glCallList( _gluiSolidDL );

	glTranslatef( pvData->basicData->pfPossitionData[0], pvData->basicData->pfPossitionData[1], pvData->basicData->pfPossitionData[2] );
	rotate_matrix( pvData->basicData->faRoataionData );

	//calculate normals
	if( bNormalsCalculated == 0 )
	{
		bNormalsCalculated = 1;
		calculate_cube_normals();
	}


	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, aVertices );
	glNormalPointer( GL_FLOAT, 0, aNormals );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, aIndices );

	glDisableClientState( GL_VERTEX_ARRAY );

	glPopMatrix();

}

/*--------------------------------------------
Author: Max Ashton
Description: Model drawn by looping through faces and mapping the face values to vertices stored in
			 the vertices array. This function uses a display list to store the model on the graphics card.
			 The display list gets build the first time the model is drawn this value is then stored in the list
			 data.
----------------------------------------------*/
void draw_model( ModelData* pmData )
{
	// if create list create list
	if( pmData->uiDisplalyListIndex == 0 )
	{
		GLuint index = glGenLists(1);
		pmData->uiDisplalyListIndex = index;
		glNewList( index, GL_COMPILE );

		for( unsigned int i = 0; i < pmData->uiNoOfFaces; i++ )
		{
			//if position 4 = 0 then there are only 3 vertecies for the face
			if( pmData->pmfFaces[i].pfVerticies[3] == -1 )
			{
				//vertices come from map
				glBegin( GL_TRIANGLES );

					glNormal3fv( pmData->pfNormalData[pmData->pmfFaces[i].pfNormals[0] -1] );
					glVertex3fv( pmData->pfVertexData[pmData->pmfFaces[i].pfVerticies[0] -1] );

					glNormal3fv( pmData->pfNormalData[pmData->pmfFaces[i].pfNormals[1] -1] );
					glVertex3fv( pmData->pfVertexData[pmData->pmfFaces[i].pfVerticies[1] -1] );

					glNormal3fv( pmData->pfNormalData[pmData->pmfFaces[i].pfNormals[2] -1] );
					glVertex3fv( pmData->pfVertexData[pmData->pmfFaces[i].pfVerticies[2] -1] );

				glEnd();
			}
			else
			{
				glBegin( GL_QUADS );

					glNormal3fv(pmData->pfNormalData[pmData->pmfFaces[i].pfNormals[0] -1]);
					glVertex3fv( pmData->pfVertexData[pmData->pmfFaces[i].pfVerticies[0] -1] );

					glNormal3fv( pmData->pfNormalData[pmData->pmfFaces[i].pfNormals[1] -1] );
					glVertex3fv( pmData->pfVertexData[pmData->pmfFaces[i].pfVerticies[1] -1] );
				
					glNormal3fv( pmData->pfNormalData[pmData->pmfFaces[i].pfNormals[2] -1] );
					glVertex3fv( pmData->pfVertexData[pmData->pmfFaces[i].pfVerticies[2] -1] );

					glNormal3fv( pmData->pfNormalData[pmData->pmfFaces[i].pfNormals[3] -1] );
					glVertex3fv( pmData->pfVertexData[pmData->pmfFaces[i].pfVerticies[3] -1] );

				glEnd();
			}
		}

		glEndList();
	}

	pmData->basicData->iDrawWireFrame ?  glCallList( _gluiWireframeDL ) : glCallList( _gluiSolidDL );

	glTranslatef( pmData->basicData->pfPossitionData[0], pmData->basicData->pfPossitionData[1], pmData->basicData->pfPossitionData[2] );
	rotate_matrix( pmData->basicData->faRoataionData );

	glCallList( pmData->uiDisplalyListIndex );
}

/*--------------------------------------------
Author: Max Ashton
Description: Loop through the list telling all the objects to be drawn by calling the function pointer;
			 Casting is used to retrieve the correct data from the list_item as models use a different structure than
			 primitive shapes
----------------------------------------------*/
void draw_list()
{
	bool bEndOfList = false;

	int iCurrentVertecie = 0;		

	maStruListElement* pvCurrentVertice = _plCurrentSceneList->_psListHead;

	while( !bEndOfList && _plCurrentSceneList->_iListSize > 0 )
	{				
		//draw object and set "name" for selection process
		glPushMatrix();

		if( pvCurrentVertice->dataType == OBJECT )
		{
			glPushName( ( ( ObjectData* ) pvCurrentVertice->_pvData )->basicData->iObjectID );
			( ( ObjectData* ) pvCurrentVertice->_pvData )->pDrawFunc( ( ObjectData* ) pvCurrentVertice->_pvData );

			if( ( ( ObjectData* ) pvCurrentVertice->_pvData )->basicData->iSelected != 0 )
			{
				draw_selection_sphere( ( ( ObjectData* ) pvCurrentVertice->_pvData )->basicData->pfPossitionData, ( ( ObjectData* ) pvCurrentVertice->_pvData )->basicData->fScale );
			}

		}
		else
		{
			glPushName( ( ( ModelData* ) pvCurrentVertice->_pvData )->basicData->iObjectID );
			( ( ModelData* ) pvCurrentVertice->_pvData )->pDrawFunc( ( ModelData* ) pvCurrentVertice->_pvData );

			if( ( ( ModelData* ) pvCurrentVertice->_pvData )->basicData->iSelected != 0 )
			{
				draw_selection_sphere( ( ( ModelData* ) pvCurrentVertice->_pvData )->basicData->pfPossitionData, ( ( ModelData* ) pvCurrentVertice->_pvData )->basicData->fScale );
			}
		}

		glPopName();
		glPopMatrix();

		if( pvCurrentVertice == _plCurrentSceneList->_psListTail )
		{
			bEndOfList = true;
		}
		else
		{
			pvCurrentVertice = pvCurrentVertice->_psNext; 		
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: Remove all shapes to clean up
----------------------------------------------*/
void max_cleanup()
{
	remove_all_shapes( *_plCurrentSceneList );
}

/*--------------------------------------------
Author: Max Ashton
Description: creates a frustum down the sight of the mouse pointer ( -z axis ) to determine object under the mouse
----------------------------------------------*/
void start_picking() 
{
	GLint agiViewport[4];
	float fRatio;

	glSelectBuffer( BUFSIZE, selectBuf );

	glGetIntegerv( GL_VIEWPORT, agiViewport );

	glRenderMode( GL_SELECT );

	glInitNames();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix( _iCursorX, agiViewport[3] - _iCursorY ,5 , 5, agiViewport );
	fRatio = ( agiViewport[2] + 0.0 ) / agiViewport[3];
	gluPerspective( 30.0f, fRatio, 1.0f, 1000.0f );
	glMatrixMode( GL_MODELVIEW );
}

/*--------------------------------------------
Author: Max Ashton
Description: flushes the selection buffer for hits
----------------------------------------------*/
void stop_picking() 
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	_uiHits = glRenderMode(GL_RENDER);

	if ( _uiHits != 0 )
	{
		process_hits( _uiHits, selectBuf, 0 );
		_uiRecalcuateSelectionLookAt = 1;//set recalculate to true so cam focuses on objects
		_uiEploreMode = 1; //explore mode to true so cam moves
	}
	else
	{
		std::cout << "No Object in selection range " << "\n";
		deselect_all_objects( *_plCurrentSceneList );				
		_uiEploreMode = 0; // come out of explore mode on deselection
		_uiFlySet = 1;
		selection_menu( 0 );
	}
	
	_iMode = RENDER;
}

/*--------------------------------------------
Author: Max Ashton
Description: Uses the vertices object names to determine selected object.
----------------------------------------------*/
void process_hits( GLuint hits, GLuint buffer[], int sw )
{
	GLuint uiNumberOfNames;
	GLuint uiNames, *pName, uiMinZ,*pNames;

	pName = ( GLuint* ) buffer;
	uiMinZ = 0xffffffff;//-1 
	
	for ( GLuint i = 0; i < hits; i++ )
	{	
		uiNames = *pName;
		pName++;

		if ( *pName < uiMinZ ) 
		{
			uiNumberOfNames = uiNames;
			uiMinZ = *pName;
			pNames = pName + 2;
		}

		pName += uiNames + 2;
	}

	if ( uiNumberOfNames > 0 ) 
	{
		printf ( "You selected Object:  " );
		pName = pNames;
		
		for ( GLuint j = 0; j < uiNumberOfNames; j++,pName++ ) 
		{ 
			printf( "%d ", *pName );
			select_object( *_plCurrentSceneList, *pName );			
		}
	}

	printf ("\n");
}