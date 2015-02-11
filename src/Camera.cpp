#include "Camera.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"

Camera::Camera()
{
	setPerspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.f);
	setLookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
}

void Camera::UpdateViewProj()
{
	m_view_proj = m_proj * m_view ;
}

void Camera::setPerspective(float FoV, float aspectRatio, float nearView, float farView)
{
	m_proj = glm::perspective(FoV, aspectRatio, nearView, farView);
	UpdateViewProj();
}

void Camera::setLookAt(vec3 pos, vec3 target, vec3 up)
{
	m_view = glm::lookAt(pos, target, up);
	m_world = glm::inverse(m_view);
	UpdateViewProj();
}

void Camera::setPosition(vec3 pos)
{
	m_world[3] = vec4(pos, 1);
	m_view = glm::inverse(m_world);
	UpdateViewProj();
}


/*******************FLY CAMERA************************/

FlyCamera::FlyCamera()
{
	m_speed = 50.f;
	
}

void FlyCamera::update(float dt)
{
	GLFWwindow* curr_window = glfwGetCurrentContext();

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		m_world[3] -= m_world[2] * m_speed * dt; //forwards
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		m_world[3] += m_world[2] * m_speed * dt; //backwards
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		m_world[3] -= m_world[0] * m_speed * dt; //left
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		m_world[3] += m_world[0] * m_speed * dt; //right
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_world[3] += m_world[1] * m_speed * dt; //up
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
	{
		m_world[3] -= m_world[1] * m_speed * dt; //down
	}

	m_world[3][3] = 1;

	
	double x_delta, y_delta;

	glfwGetCursorPos(curr_window, &x_delta, &y_delta);
	glfwSetCursorPos(curr_window, 1280.f / 2.f, 720.f / 2.f);

	x_delta -= (1280.f / 2.f);
	y_delta -= (720.f / 2.f);

	x_delta /= (1280.f / 2.f);
	y_delta /= (720.f / 2.f);

	x_delta *= -0.5f;
	y_delta *= -0.5f;

	if (glfwGetMouseButton(curr_window, 1))
	{
		vec3 camera_right = (vec3)m_world[0];

		mat4 yaw = glm::rotate((float)x_delta, vec3(0, 1, 0));
		mat4 pitch = glm::rotate((float)y_delta, camera_right);
		mat4 rot = yaw * pitch;

		m_world[0] = rot * m_world[0];
		m_world[1] = rot * m_world[1];
		m_world[2] = rot * m_world[2];

	}

	m_view = glm::inverse(m_world);
	UpdateViewProj();

}

void FlyCamera::setSpeed(float speed)
{
	m_speed = speed;
}