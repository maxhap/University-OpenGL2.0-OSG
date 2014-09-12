#pragma once

//load a 24bit bmp image. Returns a void* pointer to data of format GL_BGR_EXT 
// aligned as GL_UNSIGNED_BYTE. Returns 0 if load fails
// uiWidth and uiHeight populated with pixel dimensions of image
void* loadBitmap( const char *pcFile, unsigned int &uiWidth, unsigned int &uiHeight);

