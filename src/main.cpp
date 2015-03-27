#include "PreceduralGeneration.h"

int main()
{
	PreceduralGeneration app;

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

#include "Application.h"
#include "Camera.h"

class REPLACE : public Application
{
public:
bool startup();
void shutdown();
bool update();
void draw();

FlyCamera* m_camera;
float m_timer;
unsigned int m_programID;

};



#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

bool REPLACE::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	m_camera = new FlyCamera();

	return true;
}

void REPLACE::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool REPLACE::update()
{
	if (Application::update() == false)
	{
		return false;
	}

	Gizmos::clear();

	float dt = (float)glfwGetTime();
	glfwSetTime(0.0f);
	m_timer += dt;

	m_camera->update(dt);

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i <= 20; ++i)
	{
	Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
	Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
	}


	return true;
}

void REPLACE::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->m_proj, m_camera->m_view);
	glUseProgram(m_programID);

	int view_proj_uniform = glGetUniformLocation(m_programID, "projection_view");
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_camera->m_view_proj);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
*/
