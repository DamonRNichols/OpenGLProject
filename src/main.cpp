#include "CameraAndProjections.h"

int main()
{
	
	CameraAndProjections app;

	if (app.startup() == false)
	{
		return -1;
	}

	while (app.update() == true)
	{
		app.draw();
	}

	app.shutdown();
	
	return 0;
}

/*

USE THIS FOR ALL FUTURE CLASS BUILDING

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

bool _?????_::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	return true;
}

void _?????_::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool _?????_::update()
{
	if (Application::update() == false)
	{
		return false;
	}
	return true;
}

void _?????_::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
*/
