#include "SceneManagement.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"


bool SceneManagement::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	m_camera = new FlyCamera();

	sphere = new BoundingSphere();
	sphere->radius = 0.5f;
	cube = new AABB();

	plane = vec4(0, 1, 0, -1);


	return true;
}

void SceneManagement::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool SceneManagement::update()
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

	cube->m_center = vec3(0, cosf(m_timer) + 1, 0);
	Gizmos::addAABBFilled(cube->m_center, vec3(1), vec4(1));
	cube->m_min = vec3(cube->m_center - 0.5f);
	cube->m_max = vec3(cube->m_center + 0.5f);

	sphere->centre = vec3(0, cosf(m_timer) + 1, 0);

	Gizmos::addSphere(sphere->centre, sphere->radius, 8, 8, vec4(1, 0, 1, 1));

	vec4 planeColour;
	switch (sphere->CheckCollision(plane))
	{
	case 1:
		planeColour = vec4(0, 1, 0, 1);
		break;
	case -1:
		planeColour = vec4(1, 1, 0, 1);
		break;
	case 0:
		planeColour = vec4(1, 0, 0, 1);
		break;
	default:
		printf("Error, CheckCollision function is broken!\n");
	}

	Gizmos::addTri(vec3(4, 1, 4), vec3(-4, 1, -4), vec3(-4, 1, 4),
		planeColour);
	Gizmos::addTri(vec3(4, 1, 4), vec3(4, 1, -4), vec3(-4, 1, -4),
		planeColour);

	vec4 planes[6];
	getFrustumPlanes(m_camera->m_view_proj, planes);

	for (int i = 0; i < 6; ++i)
	{
		float d = glm::dot(vec3(planes[i]), sphere->centre) + planes[i].w;
		if (d < -sphere->radius) 
		{
			printf("Behind, don't render it!\n");
			break;
		}
		else if (d < sphere->radius) 
		{
			printf("Touching, we still need to render it!\n");
		}
		else 
		{
			printf("Front, fully visible so render it!\n");
		}
	}


	return true;
}

void SceneManagement::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->m_proj, m_camera->m_view);
	glUseProgram(m_programID);

	int view_proj_uniform = glGetUniformLocation(m_programID, "projection_view");
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_camera->m_view_proj);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void SceneManagement::getFrustumPlanes(const glm::mat4& transform, glm::vec4* planes)
{
	// right side
	planes[0] = vec4(transform[0][3] - transform[1][0],
		transform[1][3] - transform[1][0],
		transform[2][3] - transform[2][0],
		transform[3][3] - transform[3][0]);
	// left side
	planes[1] = vec4(transform[0][3] + transform[0][0],
		transform[1][3] + transform[1][0],
		transform[2][3] + transform[2][0],
		transform[3][3] + transform[3][0]);
	// top
	planes[2] = vec4(transform[0][3] - transform[0][1],
		transform[1][3] - transform[1][1],
		transform[2][3] - transform[2][1],
		transform[3][3] - transform[3][1]);
	// bottom
	planes[3] = vec4(transform[0][3] + transform[0][1],
		transform[1][3] + transform[1][1],
		transform[2][3] + transform[2][1],
		transform[3][3] + transform[3][1]);
	// far
	planes[4] = vec4(transform[0][3] - transform[0][2],
		transform[1][3] - transform[1][2],
		transform[2][3] - transform[2][2],
		transform[3][3] - transform[3][2]);
	// near
	planes[5] = vec4(transform[0][3] + transform[0][2],
		transform[1][3] + transform[1][2],
		transform[2][3] + transform[2][2],
		transform[3][3] + transform[3][2]);
	for (int i = 0; i < 6; i++)
		planes[i] = glm::normalize(planes[i]);
}