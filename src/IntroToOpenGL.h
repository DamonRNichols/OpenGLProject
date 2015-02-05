#ifndef _INTRO_OGL_H_
#define _INTRO_OGL_H_

#include "Application.h"

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class IntroToOpenGL : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

	mat4 m_projection;
	mat4 m_view;
	float m_camera_x;
	float m_camera_y;
	float m_timer;

};


#endif _INTRO_OGL_H_