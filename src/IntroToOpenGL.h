#ifndef _INTRO_OGL_H_
#define _INTRO_OGL_H_

#include "Application.h"
#include "CameraAndProjections.h"

class IntroToOpenGL : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

	mat4 m_projection;
	mat4 m_view;
	float m_timer;

	FlyCamera *m_camera;
};


#endif _INTRO_OGL_H_