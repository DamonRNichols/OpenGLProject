#ifndef _GPU_PARTICLES_H_
#define _GPU_PARTICLES_H_

#include "Application.h"
#include "Camera.h"

#include "GPUEmitter.h"

class GPUParticles : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	unsigned int m_programID;

	FlyCamera* m_camera;
	float m_time;

	GPUPointEmitter m_emitter;
};


#endif