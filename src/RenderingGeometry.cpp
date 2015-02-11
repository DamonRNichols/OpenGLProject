#include "RenderingGeometry.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "glm_header.h"

#include "Gizmos.h"


bool RenderingGeometry::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	generateShader();
	genernateGrid(10, 10);

	Gizmos::create();

	m_camera = new FlyCamera();

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	
	return true;
}

void RenderingGeometry::shutdown()
{
	delete m_camera;
	Application::shutdown();
}

bool RenderingGeometry::update()
{
	if (Application::update() == false)
	{
		return false;
	}

	float delta = (float)glfwGetTime();
	m_dt += delta;
	glfwSetTime(0.0f);
	Gizmos::clear();
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
	}
	m_camera->update(delta);
	return true;
}

void RenderingGeometry::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_programID);

	unsigned int proj_view_handle = glGetUniformLocation(m_programID, "projection_view");
	unsigned int time_handle = glGetUniformLocation(m_programID, "time");
	unsigned int height_scale_handle = glGetUniformLocation(m_programID, "heightScale");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, false, (float*)&m_camera->m_view_proj);
		glUniform1f(time_handle, m_dt);
		glUniform1f(height_scale_handle, 1);
	}
	

	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);

	Gizmos::draw(m_camera->m_view_proj);
	
	Application::draw();
}

void RenderingGeometry::genernateGrid(unsigned int rows, unsigned int cols)
{
	Vertex* vertex_array = new Vertex[(rows + 1) * (cols + 1)];

	for (unsigned int r = 0; r < rows + 1; ++r)
	{
		for (unsigned int c = 0; c < cols + 1; ++c)
		{
			vec4 pos = vec4((float)c, 0, (float)r, 1);
			vertex_array[c + r * (cols + 1)].position = pos;

			vec4 color = vec4((float)c / (cols+1), 0, (float)r, 1);
			vertex_array[c + r * (cols + 1)].color = color;
		}
	}

	m_index_count = rows * cols * 6;
	unsigned int* index_array = new unsigned int[m_index_count];

	int index_location = 0;
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			index_array[index_location + 0] = c + r * (cols +1);
			index_array[index_location + 1] = c + (r + 1) * (cols + 1);
			index_array[index_location + 2] = (c + 1) + r * (cols + 1);

			index_array[index_location + 3] = (c + 1) + r * (cols + 1);
			index_array[index_location + 4] = c + (r + 1) * (cols + 1);
			index_array[index_location + 5] = (c + 1) + (r + 1) * (cols + 1);

			index_location += 6;
		}
	}

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (cols + 1)*(rows + 1)*sizeof(Vertex), vertex_array, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //color

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec4));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count * sizeof(unsigned int), index_array, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	delete[] vertex_array;
	delete[] index_array;

}

void RenderingGeometry::generateShader()
{
	const char* vs_source = "#version 410\n"
		"layout(location=0) in vec4 position;\n"
		"layout(location=1) in vec4 color;\n"
		"out vec4 out_color;\n"
		"uniform mat4 projection_view;\n"

		"uniform float time;\n"
		"uniform float heightScale;\n"

		"void main()\n"
		"{\n"
		"	vec4 P = position;\n"
		"	P.y += sin( time + position.x ) + cos( time + position.z );\n"
		//"	out_color = color;\n"
		//"out_color.x = color.x + sin(time + position.x) + sin( time + position.z );\n"
		//"out_color.y = color.y + cos(time + position.x) + cos( time + position.z );\n"
		//"out_color.z = color.z + tan(time + position.x) + tan( time + position.z );\n"
		"out_color.x = P.y;\n"
		"out_color.y = P.y;\n"
		"out_color.z = 0;\n"
		"out_color.w = color.w;\n"
		"	gl_Position = projection_view * P;\n"
		"}";

	const char* fs_source = "#version 410\n"
		"in vec4 out_color;\n"
		"out vec4 frag_color;\n"
		"void main()\n"
		"{\n"
		"	frag_color = out_color;\n"
		"}\n";

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex_shader, 1, &vs_source, 0);
	glCompileShader(vertex_shader);

	glShaderSource(fragment_shader, 1, &fs_source, 0);
	glCompileShader(fragment_shader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertex_shader);
	glAttachShader(m_programID, fragment_shader);
	glLinkProgram(m_programID);


	//ERROR CHECKING
	int success = GL_FALSE;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		int log_length = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &log_length);

		char* log = new char[log_length];
		glGetProgramInfoLog(m_programID, log_length, 0, log);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", log);
		delete[] log;
	}

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
}