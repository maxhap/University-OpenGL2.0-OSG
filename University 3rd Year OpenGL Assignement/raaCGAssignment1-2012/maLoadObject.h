#ifndef _LOADOBJECT_
#define _LOADOBJECT_

#include <string.h>
#include <string>
#include "maLinkedList.h"

ModelData* load_object_file( std::string sFileLocation );
int get_index_of( std::string line, std::string indexString, unsigned int uiStartPoss);
void put_floats( float* fVals, std::string sLine );
void build_face( ModelFace* pface, std::string sFaceData );
void print_face_data( ModelFace* pface );
GLuint load_texture( std::string sFileName, unsigned int uiWidth, unsigned int uiHeight );

#endif