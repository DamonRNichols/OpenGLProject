#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "gl_core_4_4.h"
#include "Vertex.h"

bool LoadShaders(char* vertex_filename, char* geometry_filename, char* fragment_filename, GLuint* result);

bool LoadShaderType(char* filename, GLenum shader_type, unsigned int* output);

OpenGLData LoadOBJ(char* filename);

#endif //_UTILITY_H_