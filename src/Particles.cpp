#include "Particles.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

#include "Utility.h"

bool Particles::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	m_camera = new FlyCamera();

	LoadShaders("./shaders/particle_vertex.glsl",0, "./shaders/particle_fragment.glsl", &m_programID);

	m_emitter.Init(1000, vec3(0, 0, 0), 500, 1, 4, 0.5f, 3.0f, 0.1f, 0.075f, vec4(0, 0, 1, 1), vec4(1, 1, 0, 0.5f));

	return true;
}

void Particles::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool Particles::update()
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

	Gizmos::addSphere(vec3(0, 0, 0), 2, 20, 20, vec4(1, 1, 0, 1));

	m_emitter.Update(dt);
	m_emitter.UpdateVertexData(m_camera->m_world[3].xyz, m_camera->m_world[1].xyz);

	return true;
}

void Particles::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->m_proj, m_camera->m_view);

	glUseProgram(m_programID);

	int proj_viewUniform = glGetUniformLocation(m_programID, "projection_view");
	glUniformMatrix4fv(proj_viewUniform, 1, GL_FALSE, (float*)&m_camera->m_view_proj);

	m_emitter.Render();

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}