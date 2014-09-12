#include "stdafx.h"
#include "maShapes.h"

int objectCounter = 1;
int iActivatedlight = 0;
unsigned int _aLightingMap[6];

/*--------------------------------------------
Author: Max Ashton
Description: Adds a triangle to the bottom of the list with basic default data apart from position
----------------------------------------------*/
void add_triangle( ObjectList& list, float x, float y, float z )
{	 
	std::cout << "triangle  added" << "\n";

	ObjectData* pData = new ObjectData();
	pData->basicData = new basicData();

	pData->basicData->iObjectID = objectCounter;
	pData->basicData->fScale = 1.0;
	pData->basicData->iObjectType = TRIANGLE;
	pData->basicData->iSelected = 0;
	pData->basicData->iDrawWireFrame = 0;

	pData->basicData->pfPossitionData = new float[4];
	pData->basicData->pfPossitionData[0] = x;
	pData->basicData->pfPossitionData[1] = y;
	pData->basicData->pfPossitionData[2] = z;
	pData->basicData->pfPossitionData[3] = 1.0f;
	
	pData->basicData->faRoataionData = new float[4];
	pData->basicData->faRoataionData[0] = 0.0f;
	pData->basicData->faRoataionData[1] = 0.0f;
	pData->basicData->faRoataionData[2] = 0.0f;
	pData->basicData->faRoataionData[3] = 1.0f;

	pData->pDrawFunc = &draw_traingle;

	add_element_tail( list, pData, OBJECT );

	objectCounter++;
}

/*--------------------------------------------
Author: Max Ashton
Description: Adds a cube to the bottom of the list with basic default data apart from position
----------------------------------------------*/
void add_client_cube( ObjectList& list, float x, float y, float z )
{
	std::cout << "client cube  added" << "\n";

	ObjectData* pData = new ObjectData();
	pData->basicData = new basicData();

	pData->basicData->iObjectID = objectCounter;
	pData->basicData->fScale = 1.0;
	pData->basicData->iObjectType = CLIENTCUBE;
	pData->basicData->iSelected = 0;
	pData->basicData->iDrawWireFrame = 0;

	pData->basicData->pfPossitionData = new float[4];
	pData->basicData->pfPossitionData[0] = x;
	pData->basicData->pfPossitionData[1] = y;
	pData->basicData->pfPossitionData[2] = z;
	pData->basicData->pfPossitionData[3] = 1.0f;

	pData->basicData->faRoataionData = new float[4];
	pData->basicData->faRoataionData[0] = 0.0f;
	pData->basicData->faRoataionData[1] = 0.0f;
	pData->basicData->faRoataionData[2] = 0.0f;
	pData->basicData->faRoataionData[3] = 1.0f;

	pData->pDrawFunc = &draw_client_cube;

	add_element_tail( list, pData, OBJECT );

	objectCounter++;
}

/*--------------------------------------------
Author: Max Ashton
Description: Adds a teapot to the bottom of the list with basic default data apart from position
----------------------------------------------*/
void add_lit_teapot( ObjectList& list, float x, float y, float z )
{
	std::cout << "Lit teapot Added " << "\n";

	ObjectData* pvData = new ObjectData();
	pvData->basicData = new basicData();

	pvData->basicData->iObjectID = objectCounter;
	pvData->basicData->fScale = 1.0;
	pvData->basicData->iObjectType = LITTEAPOT;
	pvData->basicData->iSelected = 0;
	pvData->basicData->iDrawWireFrame = 0;

	pvData->basicData->pfPossitionData = new float[4];
	pvData->basicData->pfPossitionData[0] = x;
	pvData->basicData->pfPossitionData[1] = y;
	pvData->basicData->pfPossitionData[2] = z;
	pvData->basicData->pfPossitionData[3] = 1.0f;

	pvData->basicData->faRoataionData = new float[4];
	pvData->basicData->faRoataionData[0] = 0.0f;
	pvData->basicData->faRoataionData[1] = 0.0f;
	pvData->basicData->faRoataionData[2] = 0.0f;
	pvData->basicData->faRoataionData[3] = 1.0f;

	pvData->pfAmbiantMaterialData = new float[4];
	pvData->pfAmbiantMaterialData[0] = 0.01f;
	pvData->pfAmbiantMaterialData[1] = 0.9f;
	pvData->pfAmbiantMaterialData[2] = 0.9f;
	pvData->pfAmbiantMaterialData[3] = 1.0f;

	pvData->pfDiffusedMaterialData = new float[4];
	pvData->pfDiffusedMaterialData[0] = 0.6f;
	pvData->pfDiffusedMaterialData[1] = 0.6f;
	pvData->pfDiffusedMaterialData[2] = 0.6f;
	pvData->pfDiffusedMaterialData[3] = 1.0f;

	pvData->pfSpectularMaterialData = new float[4];
	pvData->pfSpectularMaterialData[0] = 0.0f;
	pvData->pfSpectularMaterialData[1] = 1.0f;
	pvData->pfSpectularMaterialData[2] = 1.0f;
	pvData->pfSpectularMaterialData[3] = 1.0f;

	pvData->pDrawFunc = &draw_lit_teapot;

	add_element_tail( list, pvData, OBJECT );

	objectCounter++;
}

/*--------------------------------------------
Author: Max Ashton
Description: Adds a sphere to the bottom of the list with basic default data apart from position
----------------------------------------------*/
void add_lit_sphere( ObjectList& list, float x, float y, float z  )
{
	std::cout << "Lit Sphere added" << "\n";

	ObjectData* pvData = new ObjectData();
	pvData->basicData = new basicData();

	pvData->basicData->iObjectID = objectCounter;
	pvData->basicData->fScale = 1.0;
	pvData->basicData->iObjectType = LITSPHERE;
	pvData->basicData->iSelected = 0;
	pvData->basicData->iDrawWireFrame = 0;

	pvData->basicData->pfPossitionData = new float[4];
	pvData->basicData->pfPossitionData[0] = x;
	pvData->basicData->pfPossitionData[1] = y;
	pvData->basicData->pfPossitionData[2] = z;
	pvData->basicData->pfPossitionData[3] = 1.0f;

	pvData->basicData->faRoataionData = new float[4];
	pvData->basicData->faRoataionData[0] = 0.0f;
	pvData->basicData->faRoataionData[1] = 0.0f;
	pvData->basicData->faRoataionData[2] = 0.0f;
	pvData->basicData->faRoataionData[3] = 1.0f;

	pvData->pfAmbiantMaterialData = new float[4];
	pvData->pfAmbiantMaterialData[0] = 0.01f;
	pvData->pfAmbiantMaterialData[1] = 0.9f;
	pvData->pfAmbiantMaterialData[2] = 0.9f;
	pvData->pfAmbiantMaterialData[3] = 1.0f;

	pvData->pfDiffusedMaterialData = new float[4];
	pvData->pfDiffusedMaterialData[0] = 0.6f;
	pvData->pfDiffusedMaterialData[1] = 0.6f;
	pvData->pfDiffusedMaterialData[2] = 0.6f;
	pvData->pfDiffusedMaterialData[3] = 1.0f;

	pvData->pfSpectularMaterialData = new float[4];
	pvData->pfSpectularMaterialData[0] = 0.0f;
	pvData->pfSpectularMaterialData[1] = 1.0f;
	pvData->pfSpectularMaterialData[2] = 1.0f;
	pvData->pfSpectularMaterialData[3] = 1.0f;

	pvData->pDrawFunc = &draw_lit_sphere;

	add_element_tail( list, pvData, OBJECT );

	objectCounter++;
}

/*--------------------------------------------
Author: Max Ashton
Description: Adds a textured quad to the bottom of the list with basic default data apart from position
----------------------------------------------*/
void add_smiley_quad( ObjectList& list, float x, float y, float z, std::string pcFile, unsigned int uiWidth, unsigned int uiHeight )
{
	GLuint guiTextureID = load_texture( pcFile, uiWidth, uiHeight );

	std::cout << "Textured Quad  added" << "\n";

	ObjectData* pData = new ObjectData();
	pData->basicData = new basicData();

	pData->basicData->uiWidth = uiWidth;
	pData->basicData->uiHeight = uiHeight;
	pData->basicData->strFileLocation = pcFile;

	pData->basicData->iObjectID = objectCounter;
	pData->basicData->fScale = 1.0;
	pData->basicData->iObjectType = TEXTUREDQUAD;
	pData->basicData->iSelected = 0;
	pData->basicData->iDrawWireFrame = 0;

	pData->basicData->pfPossitionData = new float[4];
	pData->basicData->pfPossitionData[0] = x;
	pData->basicData->pfPossitionData[1] = y;
	pData->basicData->pfPossitionData[2] = z;
	pData->basicData->pfPossitionData[3] = 1.0f;

	pData->basicData->faRoataionData = new float[4];
	pData->basicData->faRoataionData[0] = 0.0f;
	pData->basicData->faRoataionData[1] = 0.0f;
	pData->basicData->faRoataionData[2] = 0.0f;
	pData->basicData->faRoataionData[3] = 1.0f;	
		
	pData->guiTextureID = guiTextureID;
	pData->fAspectRatio = uiWidth / uiHeight;
	pData->pDrawFunc = &draw_textured_quad;

	add_element_tail( list, pData, OBJECT );

	objectCounter++;
}

/*--------------------------------------------
Author: Max Ashton
Description: Adds a light to the top of the list, if the new light has excided the max light limit then no light is added
			 and the light counter incremented			 
----------------------------------------------*/
void add_possitioned_light( ObjectList& list, float x, float y, float z )
{
	std::cout << "Positioned Light  added" << "\n";
	
	int uiLightID = get_light_number();

	if( iActivatedlight >= TOTALLIGHTS || uiLightID == NOLIGHTAVAILABLE )
	{
			std::cout << "Failed to add Positioned light, you have reached the maximum amount of lights available" << "\n";
		return;
	}

	std::cout << "Positioned Light added " << iActivatedlight + 1 << "/6" << "\n";

	ObjectData* pvData = new ObjectData();
	pvData->basicData = new basicData();

	pvData->basicData->iObjectID = objectCounter;
	pvData->basicData->uiLightID = uiLightID;
	pvData->basicData->fScale = 0.05;
	pvData->basicData->iObjectType = POSSLIGHT;
	pvData->basicData->iSelected = 0;
	pvData->basicData->iDrawWireFrame = 0;
	pvData->basicData->uiLightOn = 0;

	pvData->basicData->pfPossitionData = new float[4];
	pvData->basicData->pfPossitionData[0] = x;
	pvData->basicData->pfPossitionData[1] = y;
	pvData->basicData->pfPossitionData[2] = z;
	pvData->basicData->pfPossitionData[3] = 1.0f;

	pvData->basicData->faRoataionData = new float[4];
	pvData->basicData->faRoataionData[0] = 0.0f;
	pvData->basicData->faRoataionData[1] = 0.0f;
	pvData->basicData->faRoataionData[2] = 0.0f;
	pvData->basicData->faRoataionData[3] = 1.0f;

	pvData->pfAmbiantMaterialData = new float[4];
	pvData->pfAmbiantMaterialData[0] = 0.01f;
	pvData->pfAmbiantMaterialData[1] = 0.9f;
	pvData->pfAmbiantMaterialData[2] = 0.9f;
	pvData->pfAmbiantMaterialData[3] = 1.0f;

	//diffused used in this light
	pvData->pfDiffusedMaterialData = new float[4];
	pvData->pfDiffusedMaterialData[0] = 1.0f;
	pvData->pfDiffusedMaterialData[1] = 1.0f;
	pvData->pfDiffusedMaterialData[2] = 0.5f;
	pvData->pfDiffusedMaterialData[3] = 1.0f;

	pvData->pfSpectularMaterialData = new float[4];
	pvData->pfSpectularMaterialData[0] = 0.0f;
	pvData->pfSpectularMaterialData[1] = 1.0f;
	pvData->pfSpectularMaterialData[2] = 1.0f;
	pvData->pfSpectularMaterialData[3] = 1.0f;

	pvData->pDrawFunc = &draw_possitioned_light;

	add_element_head( list, pvData, OBJECT );

	iActivatedlight++;
	objectCounter++;
}

/*--------------------------------------------
Author: Max Ashton
Description: Remove object based on its type ( light or not ) because the light removal is handles differently 		 
----------------------------------------------*/
void remove_object( ObjectList& list, struct_list_element* pObject )
{
	if( pObject->dataType == OBJECT && ( ( ObjectData* ) pObject->_pvData )->basicData->iObjectType == POSSLIGHT )//expand for all types of light
	{
		remove_light( list, pObject );
	}
	else
	{
		remove_shape( list, pObject );
	}	
}

/*--------------------------------------------
Author: Max Ashton
Description: free up the light number in the light map so it can be reused, then delete it as a regular shape		 
----------------------------------------------*/
void remove_light( ObjectList& list, struct_list_element* pObject )
{
	free_up_light_number( ( ( ObjectData* ) pObject->_pvData )->basicData->uiLightID );
	iActivatedlight--;
	remove_shape( list, pObject );
}

/*--------------------------------------------
Author: Max Ashton
Description: Delete the data stored on the stack so that the memory can be reused 	 
			 remove item from the list
----------------------------------------------*/
void remove_shape( ObjectList& list, struct_list_element* pObject )
{
	if( pObject->dataType == OBJECT )
	{
		ObjectData* pData = ( ObjectData* ) pObject->_pvData;

		pData->pDrawFunc = NULL;
		delete pData->pfAmbiantMaterialData;
		delete pData->pfDiffusedMaterialData;
		delete pData->pfSpectularMaterialData;
		delete pData->basicData->pfPossitionData;
		delete pData->basicData->faRoataionData;
		delete pObject->_pvData;
	}
	else
	{
		ModelData* pData = ( ModelData* ) pObject->_pvData;
		pData->pDrawFunc = NULL;
		delete pData->pfNormalData;
		delete pData->pfTextureData;
		delete pData->pfVertexData;
		
		for( unsigned int i = 0; i < pData->uiNoOfFaces; i++ )
		{
			delete pData->pmfFaces[i].pfVerticies;
			delete pData->pmfFaces[i].pfNormals;
			delete pData->pmfFaces[i].pfMaterialValues;
		}

		delete pData->pmfFaces;
	}

	struct_list_element* pErrorcheckReturned = remove_element( list, pObject );

	if( pErrorcheckReturned != pObject )
	{
		//this should never print out
		std::cout << "Error Wrong Object Removed From List \n";
	}
	else
	{
		delete pObject;
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: Loop through entire list remove all objects		 
----------------------------------------------*/
void remove_all_shapes( ObjectList& list )
{
	while( list._iListSize > 0 )
	{
		if( list._psListHead->dataType == OBJECT )
		{
			if( ( ( ObjectData* ) list._psListHead->_pvData)->basicData->iObjectType == POSSLIGHT )
			{
				remove_light( list, list._psListHead );
			}
			else
			{
				remove_shape( list, list._psListHead );
			}
		}
		else
		{
			remove_shape( list, list._psListHead );
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: Loop through entire list remove all objects selected		 
----------------------------------------------*/
void remove_selected_shapes( ObjectList& list )
{
	bool bAtEnd = false;
	maStruListElement* pCurrentPoss = list._psListHead;
	maStruListElement* pJump = NULL;

	while( !bAtEnd )
	{
		if( pCurrentPoss == list._psListTail )
		{
			bAtEnd = true;
		}

		int iSelected = ( pCurrentPoss->dataType == OBJECT ) ? ( ( ObjectData* ) pCurrentPoss->_pvData)->basicData->iSelected : ( ( ModelData* ) pCurrentPoss->_pvData )->basicData->iSelected;

		if( iSelected != 0 )
		{
			pJump = pCurrentPoss->_psNext;
			remove_shape( list, pCurrentPoss );
		}

		if( !bAtEnd && pJump != NULL )
		{
			pCurrentPoss = pJump;
			pJump = NULL;
		}
		else
		{
			pCurrentPoss = pCurrentPoss->_psNext;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: loop through the array clearing all light values ( 0 is light unused ) 	 
----------------------------------------------*/
void initalise_lighting_map()
{
	for( int i = 0; i < TOTALLIGHTS; i++ )
	{
		_aLightingMap[0] = 0;
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: fine the next available light array position is the light number	 
----------------------------------------------*/
int get_light_number()
{
	int inum = NOLIGHTAVAILABLE;

	for( int i = 0; i < TOTALLIGHTS; i++ )
	{
		if( _aLightingMap[i] == 0 )
		{
			inum = i;
			_aLightingMap[i] = 1;
			break;
		}
	}

	return inum;
}

/*--------------------------------------------
Author: Max Ashton
Description: array position is equal to light number so assign the value at that position to 0
----------------------------------------------*/
void free_up_light_number( unsigned int uiLightNo )
{
	_aLightingMap[uiLightNo] = 0;
}

void select_object( ObjectList& list, int iObjctID )
{
	//iterate through list setting selected objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	while( bNotEndOfList )
	{
		if( pCurrentNode->dataType == OBJECT )
		{
			if( ( ( ObjectData* )pCurrentNode->_pvData )->basicData->iObjectID == iObjctID )
			{
				( ( ObjectData* )pCurrentNode->_pvData )->basicData->iSelected = 1;

				if( ( ( ObjectData* )pCurrentNode->_pvData )->basicData->iObjectType == POSSLIGHT )
				{
					selection_menu( 1 );
				}
				else
				{
					selection_menu( 2 );
				}
			}			
		}
		else
		{
			if( ( ( ModelData* )pCurrentNode->_pvData )->basicData->iObjectID == iObjctID )
			{
				( ( ModelData* )pCurrentNode->_pvData )->basicData->iSelected = 1;	

				if( ( ( ModelData* )pCurrentNode->_pvData )->basicData->iObjectType == POSSLIGHT )
				{
					selection_menu( 1 );
				}
				else
				{
					selection_menu( 2 );
				}
			}
		}

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: loop through list, if object selected set the wire frame mode.
			 Wire frame mode is set to reverse of the current value e.g 1 = 0 0 = 1 
----------------------------------------------*/
void togel_wire_frame_for_selected( ObjectList& list )
{
	//iterate through list setting selected objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	while( bNotEndOfList )
	{
		if( pCurrentNode->dataType == OBJECT )
		{
			if( ( ( ObjectData* )pCurrentNode->_pvData )->basicData->iSelected != 0 )
			{
				( ( ObjectData* )pCurrentNode->_pvData )->basicData->iDrawWireFrame = ( ( ( ObjectData* )pCurrentNode->_pvData )->basicData->iDrawWireFrame == 0 ) ? 1 : 0;
			}			
		}
		else
		{
			if( ( ( ModelData* )pCurrentNode->_pvData )->basicData->iSelected != 0 )
			{
				( ( ModelData* )pCurrentNode->_pvData )->basicData->iDrawWireFrame = ( ( ( ModelData* )pCurrentNode->_pvData )->basicData->iDrawWireFrame == 0 ) ? 1 : 0;
			}
		}

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: loop through list, if object selected set the light on or off.
			 light is set to reverse of the current value e.g 1 = 0 0 = 1 
----------------------------------------------*/
void togel_light_for_selected( ObjectList& list )
{
	//iterate through list setting selected objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	while( bNotEndOfList )
	{
		if( pCurrentNode->dataType == OBJECT )
		{
			if( ( ( ObjectData* )pCurrentNode->_pvData )->basicData->iSelected != 0 && ( ( ObjectData* )pCurrentNode->_pvData )->basicData->iObjectType == POSSLIGHT )
			{
				( ( ObjectData* )pCurrentNode->_pvData )->basicData->uiLightOn = ( ( ( ObjectData* )pCurrentNode->_pvData )->basicData->uiLightOn == 0 ) ? 1 : 0;
			}			
		}

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: Loop through setting 0 (false) to all objects
----------------------------------------------*/
void deselect_all_objects( ObjectList& list )
{
	//iterate through list setting selected objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	while( bNotEndOfList )
	{
		if( pCurrentNode->dataType == OBJECT )
		{
			( ( ObjectData* )pCurrentNode->_pvData )->basicData->iSelected = 0;			
		}
		else
		{		
			( ( ModelData* )pCurrentNode->_pvData )->basicData->iSelected = 0;
			
		}

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: Find the mean by adding all the selected items positions together then dividing by the count of the the selected
			 items.
----------------------------------------------*/
float* get_average_selected_possition( ObjectList& list )
{
	//iterate through list de-selecting all objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	int iSelCount = 0;
	float* pfAveragePos = new float[4];

	pfAveragePos[0] = 0.0f;
	pfAveragePos[1] = 0.0f;
	pfAveragePos[2] = 0.0f;
	pfAveragePos[3] = 0.0f;

	while( bNotEndOfList )
	{
		if( pCurrentNode->dataType == OBJECT )
		{
			if( ( ( ObjectData* )pCurrentNode->_pvData )->basicData->iSelected != 0 )
			{
				pfAveragePos[0] += ( ( ObjectData* )pCurrentNode->_pvData )->basicData->pfPossitionData[0];
				pfAveragePos[1] += ( ( ObjectData* )pCurrentNode->_pvData )->basicData->pfPossitionData[1];
				pfAveragePos[2] += ( ( ObjectData* )pCurrentNode->_pvData )->basicData->pfPossitionData[2];
				pfAveragePos[3] += ( ( ObjectData* )pCurrentNode->_pvData )->basicData->pfPossitionData[3];

				iSelCount++;
			}
		}
		else
		{
			if( ( ( ModelData* )pCurrentNode->_pvData )->basicData->iSelected != 0 )
			{
				pfAveragePos[0] += ( ( ModelData* )pCurrentNode->_pvData )->basicData->pfPossitionData[0];
				pfAveragePos[1] += ( ( ModelData* )pCurrentNode->_pvData )->basicData->pfPossitionData[1];
				pfAveragePos[2] += ( ( ModelData* )pCurrentNode->_pvData )->basicData->pfPossitionData[2];
				pfAveragePos[3] += ( ( ModelData* )pCurrentNode->_pvData )->basicData->pfPossitionData[3];

				iSelCount++;
			}
		}

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}

	pfAveragePos[0] /= iSelCount;
	pfAveragePos[1] /= iSelCount;
	pfAveragePos[2] /= iSelCount;
	pfAveragePos[3] /= iSelCount;


	return pfAveragePos;
}

/*--------------------------------------------
Author: Max Ashton
Description: Cycle through the list calling updating everything required
			 1. update_roation
----------------------------------------------*/
void update_objects( ObjectList& list )
{
	//iterate through list setting selected objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	while( bNotEndOfList && list._iListSize > 0 )
	{
		//object rotation
		update_roation( pCurrentNode );

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: for x y and z set new rotation values by using the get_new_rotation_val function
		     Casting used to set correct data for model and object objects 
----------------------------------------------*/
void update_roation( maStruListElement* pElement )
{
	if( pElement->dataType == OBJECT )
	{
		//x
		if( ( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[0] != 0 )
		{
			( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[0] = get_new_rotation_val( ( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[0] );
		}

		//y
		if( ( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[1] != 0 )
		{
			( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[1] = get_new_rotation_val( ( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[1] );
		}

		//z
		if( ( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[2] != 0 )
		{
			( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[2] = get_new_rotation_val( ( ( ObjectData* ) pElement->_pvData )->basicData->faRoataionData[2] );
		}
	}
	else
	{
		//x
		if( ( ( ModelData* ) pElement->_pvData )->basicData->faRoataionData[0] != 0 )
		{
		}

		//y
		if( ( ( ModelData* ) pElement->_pvData )->basicData->faRoataionData[1] != 0 )
		{
			( ( ModelData* ) pElement->_pvData )->basicData->faRoataionData[1] = get_new_rotation_val( ( ( ModelData* ) pElement->_pvData )->basicData->faRoataionData[1] );
		}

		//z
		if( ( ( ModelData* ) pElement->_pvData )->basicData->faRoataionData[2] != 0 )
		{
			( ( ModelData* ) pElement->_pvData )->basicData->faRoataionData[2] = get_new_rotation_val( ( ( ModelData* ) pElement->_pvData )->basicData->faRoataionData[2] );
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: increase rotation but make sure it does not exceed 360
----------------------------------------------*/
float get_new_rotation_val( float fCurrentVal )
{
	float fTemp = fCurrentVal + ROTATIONINCREASE;

	if( fTemp > 360.0f )
	{
		fTemp = fTemp - 360.0f;
	}

	return fTemp;
}

/*--------------------------------------------
Author: Max Ashton
Description: loop through obejcts setting new rotation, casting used for model or obejct
----------------------------------------------*/
void set_rotate( ObjectList& list, int iAxis, float fToRotate )
{
	//iterate through list setting selected objects
	bool bNotEndOfList = true;

	maStruListElement* pCurrentNode = list._psListHead;

	while( bNotEndOfList )
	{
		//object rotation
		if( pCurrentNode->dataType == OBJECT )
		{
			if( ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->iSelected != 0 )
			{
				( ( ObjectData* ) pCurrentNode->_pvData )->basicData->faRoataionData[iAxis] = ( ( ( ObjectData* ) pCurrentNode->_pvData )->basicData->faRoataionData[iAxis] == 0.0f ) ?  fToRotate : 0;
			}
		}
		else
		{
			if( ( ( ModelData* ) pCurrentNode->_pvData )->basicData->iSelected != 0 )
			{
				( ( ModelData* ) pCurrentNode->_pvData )->basicData->faRoataionData[iAxis] = ( ( ( ModelData* ) pCurrentNode->_pvData )->basicData->faRoataionData[iAxis] == 0.0f ) ?  fToRotate : 0;
			}
		}

		if( pCurrentNode != list._psListTail )
		{
			pCurrentNode = pCurrentNode->_psNext;
		}	
		else
		{
			bNotEndOfList = false;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: Add model using ModelData structure rather than ObjectData 
			 structure passed to load_object_file to set faces and vertices
			 array
----------------------------------------------*/
void add_model( ObjectList& list, float x, float y, float z, std::string sFileLocation )
{
	std::cout << "Model  added ( " << sFileLocation << ") \n";

	ModelData* pmdModel = load_object_file( sFileLocation );
	pmdModel->basicData = new basicData();

	pmdModel->basicData->strFileLocation = sFileLocation;
	pmdModel->basicData->iObjectType = MODEL;
	pmdModel->basicData->iObjectID = objectCounter;
	pmdModel->basicData->iSelected = 0;
	pmdModel->basicData->iDrawWireFrame = 0;

	pmdModel->basicData->pfPossitionData = new float[4];
	pmdModel->basicData->pfPossitionData[0] = x;
	pmdModel->basicData->pfPossitionData[1] = y;
	pmdModel->basicData->pfPossitionData[2] = z;
	pmdModel->basicData->pfPossitionData[3] = 1.0f;

	pmdModel->basicData->faRoataionData = new float[4];
	pmdModel->basicData->faRoataionData[0] = 0.0f;
	pmdModel->basicData->faRoataionData[1] = 0.0f;
	pmdModel->basicData->faRoataionData[2] = 0.0f;
	pmdModel->basicData->faRoataionData[3] = 1.0f;

	pmdModel->uiDisplalyListIndex = 0;

	pmdModel->pDrawFunc = &draw_model;
	add_element_tail( list, pmdModel, MODEL );	
}