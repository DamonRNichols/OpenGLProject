#ifndef _PRECEDURAL_GEN_H_
#define _PRECEDURAL_GEN_H_

#include "Application.h"
#include "Camera.h"
#include "Vertex.h"

class PreceduralGeneration : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void buildGrid(vec2 real_dims, glm::ivec2 dims);
	void buildPerlinTexture(glm::ivec2 dims, int octaves, float persistence);

	float* m_perlin_data;
	OpenGLData m_plane_mesh;
	unsigned int m_perlin_texture;
	float m_scale;

	FlyCamera* m_camera;
	float m_timer;
	unsigned int m_programID;

};



#endif // !_PRECEDURAL_GEN_H_
