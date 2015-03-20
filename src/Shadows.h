#ifndef _SHADOWS_H_
#define _SHADOWS_H_

#include "Application.h"
#include "Camera.h"
#include "Vertex.h"

class Shadows : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void buildMeshes();
	void buildShadowmap();

	FlyCamera* m_camera;
	float m_timer;
	unsigned int m_programID;

	unsigned int m_fbo;
	unsigned int m_fbo_depth;
	unsigned int m_shadow_program;

	OpenGLData m_bunny;
	OpenGLData m_plane;

	vec3 m_light_dir;
	vec4 m_light_matrix;
};

#endif // !_SHADOWS_H_


