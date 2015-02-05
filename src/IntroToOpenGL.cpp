#include "IntroToOpenGL.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"



bool IntroToOpenGL::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	m_projection = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	m_view = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
	m_timer = 0.0f;

}

void IntroToOpenGL::shutdown()
{
	Gizmos::destroy();
}

bool IntroToOpenGL::update()
{
	
	if (Application::update() == false)
	{
		return false;
	}

	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);
	m_timer += dt;

	


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::clear();

	Gizmos::addTransform(mat4(1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 red(1, 0, 0, 1);
	vec4 green(0, 1, 0, 1);
	vec4 blue(0, 0, 1, 1);
	vec4 yellow(1, 1, 0, 1);

	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);

	}

	mat4 sun_transform = glm::rotate(m_timer, vec3(0, 1, 0));

	mat4 child_one_transform = sun_transform * glm::translate(vec3(5, 0, 0)) * glm::rotate(m_timer * 5, vec3(0, 1, 0));
	mat4 child_two_transform = child_one_transform * glm::translate(vec3(1, 0, 0));

	mat4 child_planet_two = glm::rotate(m_timer * 1.2f, vec3(0, 1, 0)) * sun_transform * glm::translate(vec3(12, 0, 0));
	mat4 child_planet_three = glm::rotate(m_timer * 1.2f, vec3(0, 1, 0)) * sun_transform * glm::translate(vec3(12, 0, 0));

	mat4 child_planet_four = glm::rotate(m_timer * 0.2f, vec3(0, 1, 0)) * sun_transform * glm::translate(vec3(5, 3, 2));

	//SUN
	Gizmos::addSphere(sun_transform[3].xyz, 2.0f, 20, 20, yellow, &sun_transform);

	//Planet 1
	Gizmos::addSphere(child_one_transform[3].xyz, 0.5f, 20, 20, blue, &child_one_transform);
	//P1 Moon
	Gizmos::addSphere(child_two_transform[3].xyz, 0.2f, 20, 20, green, &child_two_transform);

	//Planet 2
	Gizmos::addArcRing(child_planet_two[3].xyz, 0, 3.0f, 4.0f, glm::radians(360.0f), 50, red, &child_planet_two);
	Gizmos::addSphere(child_planet_three[3].xyz, 1.0f, 20, 20, red, &child_planet_three);

	//station
	Gizmos::addAABBFilled(child_planet_four[3].xyz, vec3(1, 1, 1), black, &child_planet_four);

	return true;
}

void IntroToOpenGL::draw()
{
	Gizmos::draw(m_projection, m_view);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
