#include "stdafx.h"
#include "maLinkedList.h"

/*--------------------------------------------
Author: Max Ashton
Description: keep removing the head till the list is empty 
----------------------------------------------*/
void clean_up( maStructList &list )
{
	while( list._psListHead > 0 )
	{
		struct_list_element* pCurrent = remove_element_head( list );

		if( pCurrent != NULL )	
		{
			delete pCurrent;
			pCurrent = 0;
		}
	}
}

/*--------------------------------------------
Author: Max Ashton
Description: set everything to 0 nothing or null however you want to word it :)
----------------------------------------------*/
void initialise_list( maStructList &list  )
{
	list._psListHead = 0;
	list._psListTail = 0;
	list._iListSize = 0;
}

/*--------------------------------------------
Author: Max Ashton
Description: build a list element containing data and type;
----------------------------------------------*/
struct_list_element* create_list_element( void* pvData, unsigned int uiDataType )
{
	struct_list_element* pElement = new struct_list_element();
	pElement->_pvData = pvData;
	pElement->dataType = uiDataType;

	return pElement;
}

/*--------------------------------------------
Author: Max Ashton
Description: insert element into the list, if the position is 0 add to the head if the position is the size add to the tail, anthing else cycle 
though te list till you find the possition
----------------------------------------------*/
int add_element_at( maStructList &list, unsigned int uiPossition, void* pvData, unsigned int uiDataType )
{
	if( uiPossition > list._iListSize )
	{
		return 0;
	}

	if( uiPossition == 0 )
	{
		add_element_head( list, pvData, uiDataType );
	}
	else if ( uiPossition == list._iListSize )
	{
		add_element_tail( list, pvData, uiDataType );
	}
	else
	{
		//find element at position
		//future performance improvement ( start at top or bottom )

		struct_list_element* pElementAtPosition;
		struct_list_element* pElementBeforePosition;
		struct_list_element* pCurrentelement = list._psListHead;

		for( unsigned int i = 0; i <= uiPossition; i++ ) 
		{			
			if( i + 1 == uiPossition )
			{
				pElementBeforePosition = pCurrentelement;
			}
			else if( i == uiPossition )
			{
				pElementAtPosition = pCurrentelement;
				break;
			}

			pCurrentelement = pCurrentelement->_psNext;
		}

		struct_list_element* pElement = create_list_element( pvData, uiDataType );

		if( pElement == NULL )
		{
			return 0;
		}

		//make references
		pElementBeforePosition->_psNext = pElement;
		pElement->_psBefore = pElementBeforePosition;
		pElement->_psNext = pElementAtPosition;
		pElementAtPosition->_psBefore = pElement;
	}

	return 1;
}

/*--------------------------------------------
Author: Max Ashton
Description: cycle though the list until listElement = passed in element
----------------------------------------------*/
int find_element_posstion( maStructList &list, maStruListElement* pElement )
{
	int iElementID = ELEMENTNOTINLIST;
	struct_list_element* pCurrentElement = list._psListHead;

	for( unsigned int i = 0; i < list._iListSize; i++ )
	{
		if( pCurrentElement == pElement )
		{
			iElementID = i;
			break;
		}

		pCurrentElement = pCurrentElement->_psNext;
	}

	return iElementID;
}

/*--------------------------------------------
Author: Max Ashton
Description: remove element by finding its position then removing it using remove_element_at
----------------------------------------------*/
struct_list_element* remove_element( maStructList &list, maStruListElement* pElement )
{
	unsigned int uiElementPost = find_element_posstion( list, pElement );

	if( uiElementPost != ELEMENTNOTINLIST )
	{
		return remove_element_at( list, uiElementPost );
	}

	return NULL;
}

/*--------------------------------------------
Author: Max Ashton
Description: cycle through the list until you find the elements position starting from the head, then dereferences it from 
the before and after nodes, then finally re link those nodes to each other
----------------------------------------------*/
struct_list_element* remove_element_at( maStructList &list, unsigned int uiPos )
{
	if( uiPos  > list._iListSize )
	{
		return NULL;
	}

	if( uiPos  == 0 )
	{
		return remove_element_head( list );
	}
	else if ( uiPos == list._iListSize - 1 )
	{
		return remove_element_tail( list );
	}
	else
	{
		//find element at position
		struct_list_element* pElementAtPosition;
		struct_list_element* pElementBeforePosition;
		struct_list_element* pElementAfterPosition;
		struct_list_element* pCurrentelement = list._psListHead;

		for( unsigned int i = 0; i <= uiPos; i++ ) 
		{			
			if( i + 1 == uiPos )
			{
				pElementBeforePosition = pCurrentelement;
			}
			else if( i == uiPos )
			{
				pElementAtPosition = pCurrentelement;
				pElementAfterPosition = pCurrentelement->_psNext;
				break;
			}

			pCurrentelement = pCurrentelement->_psNext;
		}

		//make references
		pElementBeforePosition->_psNext = pElementAfterPosition;
		pElementAfterPosition->_psBefore = pElementBeforePosition;

		list._iListSize--;

		return pElementAtPosition;
	}

	return NULL;
}

/*--------------------------------------------
Author: Max Ashton
Description: create element then push it onto the head then increase list size
----------------------------------------------*/
int add_element_head( maStructList &list, void* pvData, unsigned int uiDataType )
{
	struct_list_element* pElement = create_list_element( pvData, uiDataType );

	struct_list_element* psNewElement = new struct_list_element;
	psNewElement->_psNext = 0;
	psNewElement->_psBefore = 0;

	if( pElement == NULL )
	{
		return 0;
	}

	int success = list_push_head( list, pElement );

	if( success == 1 )
	{
		list._iListSize++;
	}

	return success;
}

/*--------------------------------------------
Author: Max Ashton
Description: create element then push it onto the tail then increase list size
----------------------------------------------*/
int add_element_tail( maStructList &list, void* pvData, unsigned int uiDataType )
{
	struct_list_element* pElement = create_list_element( pvData, uiDataType );

	if( pElement == NULL )
	{
		return 0;
	}

	int success = list_push_tail( list, pElement );

	if( success == 1 )
	{
		list._iListSize++;
	}

	return success;
}

/*--------------------------------------------
Author: Max Ashton
Description: remove element from head then decrease list size
----------------------------------------------*/
struct_list_element* remove_element_head( maStructList &list )
{
	struct_list_element* pdPoped = list_pop_head( list );

	if( pdPoped != NULL )
	{
		list._iListSize--;
	}

	return pdPoped;
}

/*--------------------------------------------
Author: Max Ashton
Description: remove element from tail then decrease list size
----------------------------------------------*/
struct_list_element* remove_element_tail( maStructList &list )
{
	struct_list_element* pdPoped = list_pop_tail( list );

	if( pdPoped != NULL )
	{
		list._iListSize--;
	}

	return pdPoped;
}

/*--------------------------------------------
Author: Max Ashton
Description: push element onto the head by setting the current head to the new head after then the new node 
			 to the head. If the list is not empty, otherwise jsut set the item as the head and tail
----------------------------------------------*/
int list_push_head( maStructList &list, maStruListElement* pElement )
{
	if( list._psListHead == 0 )
	{
		list._psListHead = pElement;
		list._psListTail = pElement;
		pElement->_psBefore = 0;
		pElement->_psNext = 0;

	}
	else
	{
		struct_list_element* psCurrentHead = list._psListHead;
		list._psListHead = pElement;
		pElement->_psBefore = 0;
		pElement->_psNext = psCurrentHead;
		psCurrentHead->_psBefore = pElement;
	}

	return 1;
}

/*--------------------------------------------
Author: Max Ashton
Description: push element onto the head by setting the current tail to the new tail before then the new node 
			 to the tail. If the list is not empty, otherwise just set the item as the head and tail
----------------------------------------------*/
int list_push_tail( maStructList &list, maStruListElement* pElement )
{
	if( list._psListHead == 0 )
	{
		list._psListHead = pElement;
		list._psListTail = pElement;
		pElement->_psBefore = 0;
		pElement->_psNext = 0;
	}
	else
	{
		struct_list_element* psCurrentTail = list._psListTail;
		list._psListTail = pElement;
		pElement->_psBefore = psCurrentTail;
		pElement->_psNext = 0;
		psCurrentTail->_psNext = pElement;
	}

	return 1;
}

/*--------------------------------------------
Author: Max Ashton
Description: remove the first node or head by taking the current head next, assigning it to the head and remove the new heads before
----------------------------------------------*/
struct_list_element* list_pop_head( maStructList &list )
{
	if( list._psListHead == 0 )
	{
		return NULL;
	}

	struct_list_element* psCurrentHead = list._psListHead;

	if( list._psListHead == list._psListTail )
	{
		list._psListHead = 0;	
		list._psListTail = 0;		
	}
	else
	{
		struct_list_element* itemAfterhead =  list._psListHead->_psNext;

		list._psListHead = itemAfterhead;
		itemAfterhead->_psBefore = 0;		
	}

	psCurrentHead->_psBefore = 0;
	psCurrentHead->_psNext = 0;

	return psCurrentHead;
}

/*--------------------------------------------
Author: Max Ashton
Description: remove the last node or tail by taking the current tail before, assigning it to the tail and remove the new heads after
----------------------------------------------*/
struct_list_element* list_pop_tail( maStructList &list )
{
	if( list._psListHead == 0 )
	{
		return NULL;
	}

	struct_list_element* psCurrentTail = list._psListTail;

	if( list._psListHead == list._psListTail )
	{
		list._psListHead = 0;	
		list._psListTail = 0;		
	}
	else
	{
		struct_list_element* itemBeforeTail =  list._psListTail->_psBefore;

		list._psListTail = itemBeforeTail;
		itemBeforeTail->_psNext = 0;		
	}

	psCurrentTail->_psBefore = 0;
	psCurrentTail->_psNext = 0;

	return psCurrentTail;
}