#ifndef _RAACGASSIGN_
#define _RAACGASSIGN_

#include <Windows.h>
#include <stdlib.h>
#include <iostream>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <raaMathsLibrary/raaMatrix.h>
#include <raaMathsLibrary/raaVector.h>
#include <raaMathsLibrary/raaMaths.h>

#include "raaUtilities.h"
#include "raaCamera.h"
#include "maLinkedList.h"
#include "maLoadObject.h"

#include "stdafx.h"
#include "maLinkedList.h"
#include "maShapes.h"

/*-------------------------------------------
Rob Functions
--------------------------------------------*/

void display();
void idle();
void keyboard(unsigned char c, int iXPos, int iYPos);
void sKeyboard(int iC, int iXPos, int iYPos);
void mouse(int iKey, int iEvent, int iXPos, int iYPos);
void motion(int iXPos, int iYPos);
void reshape(int iWidth, int iHeight);
void init();
void mainMenu(int i);
void navMenu(int i);

/*-------------------------------------------
Max Functions
--------------------------------------------*/

void build_menu();

void max_init();
void max_render();
void max_cleanup();

//glut menu
void selection_menu( unsigned int iMenuToSet );

void objectMenu( int i );

void draw_list();
void draw_traingle( ObjectData* pvData );
void draw_lit_teapot( ObjectData* pvData );
void draw_lit_sphere( ObjectData* pvData );
void draw_possitioned_light( ObjectData* pvData );
void draw_selection_sphere( float* pfPossitionData, float fScale );
void draw_model( ModelData* pvData );
void draw_textured_quad( ObjectData* pvData );
void draw_client_cube( ObjectData* pvData );

void start_picking();
void stop_picking();
void process_hits( GLuint hits, GLuint buffer[], int sw );

void forward_scene();
void back_scene();

void create_drawmode_displaylists();


#endif