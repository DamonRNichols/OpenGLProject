#ifndef _POST_PROCESS_H_
#define _POST_PROCESS_H_

#include "Application.h"
#include "Camera.h"
#include "Vertex.h"

class PostProcess : public Application
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

	OpenGLData m_quad;
};



#endif // !_POST_PROCESS_H_
