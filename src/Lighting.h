#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include "Application.h"
#include "Camera.h"
#include "tiny_obj_loader.h"

#include "Vertex.h"

class Lighting : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
	void cleanupOpenGLBuffers();

	void reloadShader();

	FlyCamera* m_camera;

	unsigned int m_programID;
	std::vector<OpenGLData> m_gl_data;

	vec3 m_light_dir;
	vec3 m_light_color;
	vec3 m_material_color;
	vec3 m_ambient_light;
	float m_specular_power;
};



#endif //_LIGHTING_H_