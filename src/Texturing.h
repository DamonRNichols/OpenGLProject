#ifndef _TEXTURING_H_
#define _TEXTURING_H_

#include "Application.h"
#include "Camera.h"

#include "Vertex.h"

class Texturing : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void loadTexture(const char * filename);
	void generateQuad(float size);
	OpenGLData generateSphere(float radius, int rows, int cols);
	unsigned int m_programID;

	FlyCamera *m_camera;
	mat4 m_projection;
	mat4 m_view;
	float m_dt;

	unsigned int m_texture;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;


	OpenGLData m_sphere;
};





#endif //_TEXTURING_H_