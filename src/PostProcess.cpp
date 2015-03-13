#include "PostProcess.h"


#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

bool PostProcess::startup()
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

void PostProcess::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool PostProcess::update()
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

void PostProcess::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->m_proj, m_camera->m_view);
	glUseProgram(m_programID);

	int view_proj_uniform = glGetUniformLocation(m_programID, "projection_view");
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_camera->m_view_proj);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void PostProcess::GenerateFramebuffer()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_fbo_texture);
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fbo_texture, 0);

	glGenRenderbuffers(1, &m_fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1280, 720);
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

void generateScreenSpaceQuad()
{
	vec2 half_texel = 1.0f / vec2(1280, 720) * 0.5f;

	float vertex_data[]
	{
		-1,-1,0,1,	half_texel.x, half_texel.t,
		1,-1,0,1,	half_texel.x, half_texel.t,
		1,1,0,1,	half_texel.x, half_texel.t,
		-1,1,0,1,	half_texel.x, half_texel.t,
	};

	unsigned int index_data[] =
	{
		0, 1, 2, 0, 2, 3
	};

	glGenVertexArrays(1, &m_quad.m_VAO);

	glGenBuffers(1,&m_quad.VBO);
	glGenBuffers(1,&m_quad.IBO);

	glBindVertexArray()

}