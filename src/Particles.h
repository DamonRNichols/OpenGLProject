#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "Application.h"
#include "Camera.h"

#include "Emitter.h"

class Particles : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	unsigned int m_programID;

	Emitter m_emitter;

	FlyCamera* m_camera;
	float m_timer;
};

#endif // !_PARTICLES_H_
