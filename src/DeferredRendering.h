#ifndef _DEFERRED_RENDERING_H_
#define _DEFERRED_RENDERING_H_

#include "Application.h"
#include "Camera.h"
#include "Vertex.h"

struct PointLight
{
	vec3 position;
	vec3 diffuse;
	float radius;
};

class DeferredRendering : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void buildMeshes();
	void buildGbuffer();
	void buildLightBuffer();
	void buildCube();
	void buildQuad();

	void renderDirectionalLight(vec3 light_dir, vec3 light_color);
	void renderPointLight(vec3 position, float radius, vec3 diffuse);
	void renderPointLights(PointLight lights[]);

	vec3 colors[6];

	static const int MAX_POINT_LIGHTS = 20;
	PointLight PointLights[MAX_POINT_LIGHTS];

	//load up a mesh
	OpenGLData m_light_cube;
	OpenGLData m_bunny;
	OpenGLData m_screenspace_quad;

	//generate a g-buffer
	unsigned int m_gbuffer_fbo;
	unsigned int m_albedo_texture;
	unsigned int m_position_texture;
	unsigned int m_normals_texture;
	unsigned int m_gbuffer_depth;

	//render lights
	unsigned int m_light_fbo;
	unsigned int m_light_texture;

	//we need our shaders
	unsigned int m_gbuffer_program;
	unsigned int m_dir_light_program;
	unsigned int m_point_light_program;
	unsigned int m_spot_light_program;
	unsigned int m_comp_light_program;

	FlyCamera* m_camera;
	float m_timer;
	unsigned int m_programID;

};


#endif // !_DEFERRED_RENDERING_H_


