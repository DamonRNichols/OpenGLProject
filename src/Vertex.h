#ifndef VERTEX_H_
#define VERTEX_H_

#include "glm_header.h"

struct Vertex
{
	vec4 position;
	vec4 color;
};

struct VertexTexCoord
{
	vec4 position;
	vec2 tex_coord;
};

#endif //VERTEX_H_