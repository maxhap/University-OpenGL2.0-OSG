#ifndef _MASHAPES_ 
#define _MASHAPES_

#include "stdafx.h"	
#include "maLinkedList.h"
#include "raaCGAssignment1-2012.h"
#include "raaCamera.h"
#include <iostream>
#include <string>
#include "maLoadObject.h"

/*-----------------------------------------------*/
//Functions
/*-----------------------------------------------*/

//objects
void add_triangle( ObjectList& list, float x, float y, float z );
void add_lit_teapot( ObjectList& list, float x, float y, float z );
void add_lit_sphere( ObjectList& list, float x, float y, float z  );
void add_model( ObjectList& list, float x, float y, float z, std::string sFileLocation );
void add_smiley_quad( ObjectList& list, float x, float y, float z, std::string pcFile, unsigned int uiWidth, unsigned int uiHeight );
void add_client_cube( ObjectList& list, float x, float y, float z );

void remove_object( ObjectList& list, struct_list_element* pObject );
void remove_light( ObjectList& list, struct_list_element* pObject );
void remove_shape( ObjectList& list, struct_list_element* pObject );
void remove_all_shapes( ObjectList& list );
void remove_selected_shapes( ObjectList& list );

void update_objects( ObjectList& list );
void update_roation( maStruListElement* pelement );
float get_new_rotation_val( float fCurrentVal );

//lights
void initalise_lighting_map();
void add_possitioned_light( ObjectList& list, float x, float y, float z );
void free_up_light_number( unsigned int uiLightNo );

//select control functions
void set_rotate( ObjectList& list, int iAxis, float fToRotate );
int get_light_number();

//object selection
void select_object( ObjectList& list, int iObjctID );
void deselect_all_objects( ObjectList& list );
float* get_average_selected_possition( ObjectList& list );
void togel_wire_frame_for_selected( ObjectList& list );
void togel_light_for_selected( ObjectList& list );

/*-----------------------------------------------*/
//variables
/*-----------------------------------------------*/

//rotation increases
const float ROTATE = 1.0f;
const float NOROTATE = 0.0f;
const float ROTATIONINCREASE = 1.0f;
const int ROTATEX = 0;
const int ROTATEY = 1;
const int ROTATEZ = 2;

//lights enabled consts
const int TOTALLIGHTS = 6;
const int NOLIGHTAVAILABLE = -1;

//object identifiers consts
const int TRIANGLE = 0;
const int LITTEAPOT = 1;
const int LITSPHERE = 2;
const int POSSLIGHT = 3;
const int MODEL = 4;
const int OBJECT = 5;
const int TEXTUREDQUAD = 6;
const int CLIENTCUBE = 7;

#endif