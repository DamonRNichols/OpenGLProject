#include "IntroToOpenGL.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"


struct Planet
{
	float m_radius;
	float m_orbit_days; //how many Earth days does it take to orbit the sun?
	float m_rotation; //how many Earth days does it take to rotate its own day?
	vec3 m_distance; //how far away from the sun's edge? (sun's radius is added in the constructor)
	vec3 m_temp_dist; //an addition variable to hold the distance when we want to change by key press
	mat4 m_transform; //all those variables put together
	vec4 m_color;
	int m_planet_num; //to keep track of placement for scaling reasons

	Planet(float radius, float orbit_days, float rotation, vec3 distance_sun, vec4 color, int planet_number)
	{
		m_radius = radius;
		m_orbit_days = orbit_days;
		m_rotation = rotation;
		m_distance = distance_sun;
		m_temp_dist = distance_sun;
		m_color = color;
		m_planet_num = planet_number;

		m_transform = glm::rotate(1.f * m_orbit_days, vec3(0, 1, 0)) * glm::translate(m_temp_dist) * glm::rotate(0.f * m_rotation, vec3(0, 1, 0));

	}

	void update(float dt)
	{
		//orbit rotation = deltaTime * (comp_speed / orbit_days)
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
			if (m_orbit_days == 0)
				m_temp_dist = vec3(0, 0, 0);
			else
				m_temp_dist = vec3(6.958f + 3 * m_planet_num, 0, 0);
		else
			m_temp_dist = m_distance;

		if (m_orbit_days == 0) //is this the sun? if so, don't change its position.
			m_transform = glm::rotate(0.f, vec3(0, 1, 0)) * glm::translate(m_temp_dist) * glm::rotate(dt * m_rotation, vec3(0, 1, 0));
		else
			m_transform = glm::rotate(dt * (100 / m_orbit_days), vec3(0, 1, 0)) * glm::translate(m_temp_dist) * glm::rotate(dt * (100 / m_rotation), vec3(0, 1, 0));

		Gizmos::addSphere(m_transform[3].xyz, m_radius, 20, 20, m_color,&m_transform);
	}
};

struct Moon
{
	float m_radius;
	float m_orbit_days;
	float m_rotation;
	vec3 m_distance;
	mat4 m_transform;
	vec4 m_color;
	Planet* m_home_planet;

	Moon(Planet planet, float orbit_days, float rotation, float radius, vec3 distance_sun, vec4 color)
	{
		m_radius = radius;
		m_orbit_days = orbit_days;
		m_rotation = rotation;
		m_distance = distance_sun;

		m_color = color;


	}

	void update(float dt)
	{

	}
};




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
	m_view = glm::lookAt(vec3(0, 10, 40), vec3(0, 0, 0), vec3(0, 1, 0));
	m_timer = 0.0f;

	

}

void IntroToOpenGL::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
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
	vec4 cyan(0, 1, 1, 1);
	vec4 magenta(1, 0, 1, 1);


	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);

	}
	// 1 = 100,000km distance
	//1 = 1 day

	float suns_radius = 6.958f;

	Planet Sun(suns_radius, 0.f, 25.05f, vec3(0, 0, 0), vec4(1, 1, 0, 1), 0);

	Planet Mercury(0.024397f, 87.969f, 58.646f, vec3(suns_radius + 0.58f, 0, 0), vec4(1), 1);
	Planet Venus(0.060518f, 224.701f, -243.0185f, vec3(suns_radius + 1.08f, 0, 0), vec4(1, 0.5f, 0, 1), 2);
	Planet Earth(0.06371f, 365.256f, 1.f, vec3(suns_radius + 1495.978f, 0, 0), vec4(0, 0, 1, 1), 3);
	Planet Mars(0.033962f, 686.971f, 1.025957f, vec3(suns_radius + 2279.391f, 0, 0), vec4(1,0,0,1), 4);

	Planet Jupiter(0.71492f, 4332.59f, 0.413541f, vec3(suns_radius + 7785.472f, 0, 0), vec4(1, 0.5f, 0.5f, 1), 5);
	Planet Saturn(0.60268f, 10759.22f, 0.440416f, vec3(suns_radius + 14334.49f, 0, 0), vec4(0.5f, 0.2f, 0.1f, 1), 6);
	Planet Uranus(0.25559f, 30687.15f, 0.71833f, vec3(suns_radius + 28706.71f, 0, 0), vec4(0.1f, 0.1f, 1, 1), 7);
	Planet Neptune(0.24764f, 60190.03f, 0.6713f, vec3(suns_radius + 44985.42f, 0, 0), vec4(0.1f, 0.1f, 1, 1), 8);

	Sun.update(m_timer);

	Mercury.update(m_timer);
	Venus.update(m_timer);
	Earth.update(m_timer);
	Mars.update(m_timer);

	Jupiter.update(m_timer);
	Saturn.update(m_timer);
	Uranus.update(m_timer);
	Neptune.update(m_timer);
	/*
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
	*/
	

	return true;
}

void IntroToOpenGL::draw()
{
	Gizmos::draw(m_projection, m_view);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
