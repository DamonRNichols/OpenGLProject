#ifndef _ADV_TEXTURES_H_
#define _ADV_TEXTURES_H_

#include "Application.h"
#include "Camera.h"

#include "Vertex.h"

#include "AntTweakBar.h"


class AdvancedTextures : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void loadTexture();
	void generateQuad(float size);

	unsigned int m_programID;

	OpenGLData m_quad;

	unsigned int m_diffuse_texture; //coloured texture
	unsigned int m_normal_texture; //bumpy texture
	unsigned int m_specular_texture;

	vec3 m_light_dir;
	vec3 m_light_color;
	vec3 m_ambient_light;
	vec4 m_background_color;
	float m_specular_power;

	bool m_draw_gizmos;
	float m_fps;

	FlyCamera *m_camera;
	mat4 m_projection;
	mat4 m_view;
	float m_dt;

	TwBar* m_bar;

};




#endif