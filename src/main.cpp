#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <cstdio>
#include "Gizmos.h"

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

int main()
{
	if (glfwInit() == false)
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(1280,720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		return -2;
	}

	glfwMakeContextCurrent(window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	int major_version = ogl_GetMajorVersion();
	int minor_version = ogl_GetMinorVersion();

	printf("successfully loaded OpenGL version %d.%d\n", major_version, minor_version);
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	
	mat4 projection = glm::perspective(glm::radians(60.0f), 1280.0f/720.0f,0.1f,1000.0f);

	
	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));

	float timer = 0.0f;

	glfwSetTime(0.0);
	while (glfwWindowShouldClose(window) == false)
	{
		//this is where our rendering code will be
		float dt = (float)glfwGetTime();
		glfwSetTime(0.0);
		timer += dt;


		

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

		mat4 sun_transform = glm::rotate(timer, vec3(0, 1, 0));

		mat4 child_one_transform = sun_transform * glm::translate(vec3(5, 0, 0)) * glm::rotate(timer * 5, vec3(0, 1, 0));
		mat4 child_two_transform = child_one_transform * glm::translate(vec3(1, 0, 0));

		mat4 child_planet_two = glm::rotate(timer * 1.2f, vec3(0, 1, 0)) * sun_transform * glm::translate(vec3(12, 0, 0));
		mat4 child_planet_three = glm::rotate(timer * 1.2f, vec3(0, 1, 0)) * sun_transform * glm::translate(vec3(12, 0, 0));

		mat4 child_planet_four = glm::rotate(timer * 0.2f, vec3(0, 1, 0)) * sun_transform * glm::translate(vec3(5, 3, 2));

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

		Gizmos::draw(projection, view);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}