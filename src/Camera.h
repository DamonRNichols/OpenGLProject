#ifndef _CAMERA_H_
#define _CAMERA_H_

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:
	Camera();

	virtual void update(float dt) = 0;
	void UpdateViewProj();

	mat4 m_world;
	mat4 m_view;
	mat4 m_proj;
	mat4 m_view_proj;

	void setPerspective(float FoV, float aspectRatio, float near, float far);
	void setLookAt(vec3 pos, vec3 target, vec3 up);
	void setPosition(vec3 pos);
	
};

class FlyCamera : public Camera
{
public:
	FlyCamera();
	virtual void update(float dt);
	
	float m_speed;
	vec3 m_up;

	void setSpeed(float speed);


};


#endif _CAMERA_H_