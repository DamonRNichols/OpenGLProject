#ifndef _QUATERNIONS_H_
#define _QUATERNIONS_H_

#include "Application.h"
#include "Camera.h"

struct KeyFrame
{
	vec3 position;
	glm::quat rotation;
};

class Quaternions : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();
	float m_timer;

	FlyCamera *cam;

	KeyFrame m_hip_frames[2];
	KeyFrame m_knee_frames[2];
	KeyFrame m_ankle_frames[2];

	mat4 m_hip_bone;
	mat4 m_knee_bone;
	mat4 m_ankle_bone;


};


#endif // !_QUATERNIONS_H_