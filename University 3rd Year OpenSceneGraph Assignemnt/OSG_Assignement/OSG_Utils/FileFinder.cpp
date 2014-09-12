#include "stdafx.h"
#include "FileFinder.h"

#include <CommDlg.h>

using namespace std;

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getFilePath
description: Uses WINAPI GetOpenFileName, filters determined by consts
----------------------------------------------------------------------------------*/
std::string FileFinder::getFilePath( unsigned int uiFilter )
{	
	char cCurrentPath[FILENAME_MAX];

	GetCurrentDir( cCurrentPath, sizeof( cCurrentPath ) );

	TCHAR szFilters[] = _T( "All Files (*.*)\0*.*\0\0" );
	TCHAR szFiltersObject[] = _T( "Object Files(*.obj)\0*.obj\0\0" );
	TCHAR szFiltersScene[] = _T( "Scene Files (*.scn)\0*.scn\0\0" );
	TCHAR szFiltersTexture[] = _T( "Bitmap Files (*.bmp)\0*.bmp\0\0" );

	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;

	switch( uiFilter )
	{
	case FILTER_OBJECT: ofn.lpstrFilter = szFiltersObject; break;
	case FILTER_SCENE: ofn.lpstrFilter = szFiltersScene; break;
	case FILTER_TEXTURE: ofn.lpstrFilter = szFiltersTexture; break;
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

	SetCurrentDir( cCurrentPath );

	return buffer;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getSaveFilePath
description: Uses WINAPI GetSaveFileName, filters determined by consts
----------------------------------------------------------------------------------*/
std::string FileFinder::getSaveFilePath( unsigned int uiFilter )
{
	char cCurrentPath[FILENAME_MAX];

	GetCurrentDir( cCurrentPath, sizeof( cCurrentPath ) );

	TCHAR szFilters[] = _T( "All Files (*.*)\0*.*\0\0" );
	TCHAR szFiltersObject[] = _T( "Object Files(*.obj)\0*.obj\0\0" );
	TCHAR szFiltersScene[] = _T( "Scene Files (*.scn)\0*.scn\0\0" );
	TCHAR szFiltersTexture[] = _T( "Bitmap Files (*.bmp)\0*.bmp\0\0" );
	TCHAR szFiltersXML[] = _T( "XML Files (*.xml)\0*.xml\0\0" );

	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;

	switch( uiFilter )
	{
	case FILTER_OBJECT: ofn.lpstrFilter = szFiltersObject; break;
	case FILTER_SCENE: ofn.lpstrFilter = szFiltersScene; break;
	case FILTER_TEXTURE: ofn.lpstrFilter = szFiltersTexture; break;
	case FILTER_XML: ofn.lpstrFilter = szFiltersXML; break;
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

	SetCurrentDir( cCurrentPath );

	return buffer;
}
