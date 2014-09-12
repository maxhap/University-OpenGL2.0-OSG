#pragma once
#include "stdafx.h"	
#include <conio.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <string>

struct data_holder;
struct struct_list;
struct model_list_element;

typedef void ( *draw_object )( data_holder* );
typedef void ( *draw_object_model )( model_list_element* );

/*-----------------------------------------------*/
//Structures
/*-----------------------------------------------*/

struct basicData
{
	int iObjectID;
	int iObjectType;//object type held because of block scene writer its used when reading back in
	int iSelected;
	int iDrawWireFrame;

	unsigned int uiLightID;
	unsigned int uiWidth;
	unsigned int uiHeight;
	unsigned int uiLightOn;

	float fScale;		

	float* faRoataionData;
	float* pfPossitionData;

	std::string strFileLocation;
};


typedef struct data_holder
{
	draw_object pDrawFunc;

	float* pfAmbiantMaterialData;
	float* pfDiffusedMaterialData;
	float* pfSpectularMaterialData;

	GLuint guiTextureID;
	float  fAspectRatio;

	basicData* basicData;
	
} ObjectData;

typedef struct struct_model_faces
{
	//list of Verticies
	int* pfVerticies;
	unsigned int uiNoOfVertiies;

	//list of material values
	int* pfMaterialValues;
	unsigned int uiNoOfMaterialValues;

	//list of normals
	int* pfNormals;
	unsigned int uiNoOfNormals;

} ModelFace;

typedef struct model_list_element
{
	draw_object_model pDrawFunc;

	ModelFace* pmfFaces;
	unsigned int uiNoOfFaces;

	float** pfVertexData;
	float** pfTextureData;
	float** pfNormalData;

	unsigned int uiDisplalyListIndex;

	basicData* basicData;

} ModelData;

typedef struct struct_list_element
{
	void* _pvData;	
	unsigned int dataType;

	struct_list_element* _psNext;
	struct_list_element* _psBefore;

} maStruListElement;

typedef struct struct_list
{
	maStruListElement* _psListHead;
	maStruListElement* _psListTail;
	unsigned int _iListSize;

} maStructList, ObjectList;

/*-----------------------------------------------*/
//Functions
/*-----------------------------------------------*/
void clean_up( maStructList &list  );

void initialise_list( maStructList &list );

int add_element_head( maStructList &list, void* pvData, unsigned int uiDataType );
int add_element_tail( maStructList &list, void* pvData, unsigned int uiDataType );
int add_element_at( maStructList &list, void* pvData, unsigned int uiDataType ); 
int list_push_head( maStructList &list, maStruListElement* pElement );
int list_push_tail( maStructList &list, maStruListElement* pElement );
int find_element_posstion( maStructList &list, maStruListElement* pElement );

maStruListElement* remove_element_head( maStructList &list );
maStruListElement* remove_element_tail( maStructList &list );
maStruListElement* remove_element_at( maStructList &list, unsigned int uiPos );
struct_list_element* remove_element( maStructList &list, maStruListElement* pElement );

maStruListElement* list_pop_head( maStructList &list );
maStruListElement* list_pop_tail( maStructList &list );

/*-----------------------------------------------*/
//variables
/*-----------------------------------------------*/
const int ELEMENTNOTINLIST = -1;
const unsigned int LISTOFLISTS = 100;
