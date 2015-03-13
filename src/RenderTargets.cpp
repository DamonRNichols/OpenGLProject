#include "RenderTargets.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"

bool RenderTarget::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	m_camera = new FlyCamera();

	GenerateFramebuffer();
	GeneratePlane();
	LoadShaders("./shaders/textured_vertex.glsl",0, "./shaders/textured_fragment.glsl", &m_programID);


	return true;
}

void RenderTarget::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool RenderTarget::update()
{
	if (Application::update() == false)
	{
		return false;
	}

	Gizmos::clear();

	Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 12, 12, vec4(1, 1, 0, 1));

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

void RenderTarget::draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 512, 512);
	glClearColor(0.75f, 0.75f, 0.75f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 world = m_camera->m_world;

	vec4 plane = vec4(0, 0, 1, -5);

	vec3 reflected = glm::reflect(-world[2].xyz(), plane.xyz());
	reflected = glm::normalize(reflected);

	float dist = (glm::dot((plane.xyz() * plane.w) - world[3].xyz(), plane.xyz())) 
		/ (glm::dot(plane.xyz(), -world[2].xyz()));

	vec3 inter = world[3].xyz() + (-world[2].xyz()) * dist;

	world[3].xyz = inter - reflected * dist;
	world[2].xyz = -reflected;

	vec3 up = vec3(0, 1, 0);
	world[2].xyz = -reflected;
	world[0].xyz = glm::normalize(glm::cross(world[2].xyz(), up));
	world[1].xyz = glm::normalize(glm::cross(world[0].xyz(), world[2].xyz()));

	mat4 view = glm::inverse(world);
	mat4 proj_view = m_camera->m_proj * view;



	Gizmos::draw(proj_view);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 1280, 720);
	glClearColor(0.3f, 0.3f, 0.3f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->m_proj * m_camera->m_view);

	
	glUseProgram(m_programID);

	int view_proj_uniform = glGetUniformLocation(m_programID, "projection_view");
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_camera->m_view_proj);

	int proj_view_ref_uniform = glGetUniformLocation(m_programID, "projection_view_reflected");
	glUniformMatrix4fv(proj_view_ref_uniform, 1, GL_FALSE, (float*)&proj_view);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

	int diffuseUniform = glGetUniformLocation(m_programID, "diffuse");
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_camera->m_view_proj);

	glBindVertexArray(m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, m_plane.m_index_count, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void RenderTarget::GenerateFramebuffer()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_fbo_texture);
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fbo_texture, 0);

	glGenRenderbuffers(1, &m_fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fbo_depth);

	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FRAMEBUFFER DID NOT GENERATE PROPERLY!!\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::GeneratePlane()
{
	float vertex_data[] =
	{
		-5, 0, -5, 1, 0, 0,
		5, 0, -5, 1, 1, 0,
		5, 10, -5, 1, 1, 1,
		-5, 10, -5, 1, 0, 1,
	};

	unsigned int index_data[] = 
	{
		0,1,2,
		0,2,3
	};

	m_plane.m_index_count = 6;

	glGenVertexArrays(1, &m_plane.m_VAO);
	glBindVertexArray(m_plane.m_VAO);

	glGenBuffers(1, &m_plane.m_VBO);
	glGenBuffers(1, &m_plane.m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_plane.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data),
		vertex_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data),
		index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //texcoord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 4));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}