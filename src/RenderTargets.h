#ifndef _RENDER_TARGETS_H_
#define _RENDER_TARGETS_H_

#include "Application.h"
#include "Camera.h"
#include "Vertex.h"

class RenderTarget : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void GenerateFramebuffer();
	void GeneratePlane();

	FlyCamera* m_camera;
	float m_timer;
	unsigned int m_programID;

	unsigned int m_fbo;
	unsigned int m_fbo_texture;
	unsigned int m_fbo_depth;

	OpenGLData m_plane;

};



#endif // !_RENDER_TARGETS_H_
