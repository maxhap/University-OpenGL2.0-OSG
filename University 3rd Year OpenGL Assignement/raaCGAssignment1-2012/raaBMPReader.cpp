#include "stdafx.h"
#include <windows.h>
#include <malloc.h>
#include "raaBMPReader.h"

#ifdef WIN32
#pragma warning( disable : 4996 )
#endif

void* loadBitmap( const char *pcFile, unsigned int &uiWidth, unsigned int &uiHeight )
{
	FILE *pFile=0;            
	void *pvBits=0;
	long lBitsize=0, lInfosize=0;
	BITMAPFILEHEADER header;
	BITMAPINFO *pInfo=0;

	if(pFile=fopen(pcFile, "rb"))
	{
		if(fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile)==1&&header.bfType=='MB'&&(pInfo=(BITMAPINFO *)_alloca(lInfosize=header.bfOffBits-sizeof(BITMAPFILEHEADER)))&&fread(pInfo, 1, lInfosize, pFile)==lInfosize&&(pvBits=malloc(lBitsize=(((uiWidth=pInfo->bmiHeader.biWidth)*pInfo->bmiHeader.biBitCount+7)>>3)*(uiHeight=pInfo->bmiHeader.biHeight)))&&fread(pvBits,1,lBitsize,pFile)!=lBitsize)
		{
			free(pvBits);
			pvBits=0;
		}
		fclose(pFile);
	}
	return pvBits;
}



