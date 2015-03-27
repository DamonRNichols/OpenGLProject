#include "PreceduralGeneration.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"


bool PreceduralGeneration::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	m_camera = new FlyCamera();


	LoadShaders("./shaders/perlin_vertex.glsl", 0, "./shaders/perlin_fragment.glsl", &m_programID);

	buildGrid(vec2(10, 10), glm::ivec2(64, 64));
	buildPerlinTexture(glm::ivec2(64,64), 6, 0.3f);

	return true;
}

void PreceduralGeneration::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool PreceduralGeneration::update()
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

	//for (int i = 0; i <= 20; ++i)
	//{
	//	Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
	//	Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
	//}

	m_scale = 2;

	return true;
}

void PreceduralGeneration::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	int view_proj_uniform = glGetUniformLocation(m_programID, "view_proj");
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_camera->m_view_proj);

	int scale_uniform = glGetUniformLocation(m_programID, "scale");
	glUniform1f(scale_uniform, m_scale);

	glBindVertexArray(m_plane_mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, m_plane_mesh.m_index_count, GL_UNSIGNED_INT, 0);

	int tex_uniform = glGetUniformLocation(m_programID, "perlin_texture");
	glUniform1i(tex_uniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	Gizmos::draw(m_camera->m_proj, m_camera->m_view);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void PreceduralGeneration::buildGrid(vec2 real_dims, glm::ivec2 dims)
{
	//compute and then allocate vertex data. NOTE: dims = columns
	unsigned int vertex_count = (dims.x + 1) * (dims.y + 1);
	VertexTexCoord* vertex_data = new VertexTexCoord[vertex_count];

	//compute and then allocate index data
	unsigned int index_count = dims.x * dims.y * 6;
	unsigned int* index_data = new unsigned int[index_count];
	
	//loop through and generate vertex data
	float curr_y = -real_dims.y / 2.0f;
	for (int y = 0; y < dims.y + 1; ++y)
	{
		float curr_x = -real_dims.x / 2.0f;
		for (int x = 0; x < dims.x + 1; ++x)
		{
			vertex_data[y * (dims.x + 1) + x].position = vec4(curr_x, 0, curr_y, 1);
			vertex_data[y * (dims.x + 1) + x].tex_coord = vec2((float)x / (float)dims.x, (float)y / (float)dims.y);

			curr_x += real_dims.x / (float)dims.x;
		}
		curr_y += real_dims.y / (float)dims.y;
	}

	//loop through again for generating index data
	int curr_index = 0;
	for (int y = 0; y < dims.y; ++y)
	{
		for (int x = 0; x < dims.x; ++x)
		{
			//create our 6 indices here
			index_data[curr_index++] = y * (dims.x + 1) + x;
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + x;
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);

			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + x;
		}
	}

	m_plane_mesh.m_index_count = index_count;

	//create VertexArrayOject, buffers, etc
	glGenVertexArrays(1, &m_plane_mesh.m_VAO);
	glGenBuffers(1, &m_plane_mesh.m_VBO);
	glGenBuffers(1, &m_plane_mesh.m_IBO);

	//bind and fill buffers
	glBindVertexArray(m_plane_mesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_plane_mesh.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane_mesh.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexCoord) * vertex_count, vertex_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count, index_data, GL_STATIC_DRAW);

	//tell OpenGL about our vertex structure
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1); 

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexTexCoord), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexCoord), (void*)sizeof(vec4));

	//unbind stuff
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//free vertex and index data
	delete[] vertex_data;
	delete[] index_data;
}

void PreceduralGeneration::buildPerlinTexture(glm::ivec2 dims, int octaves, float persistence)
{
	//set scale
	float scale = (1.0f / dims.x) * 3.0f;

	//allocate memory for perlin data
	m_perlin_data = new float[dims.x * dims.y];

	//loop through all the pixels
	for (int y = 0; y < dims.y; ++y)
	{
		for (int x = 0; x < dims.x; ++x)
		{
			//set default amplitude, freq and data for generating noise
			float amplitude = 1;
			float freq = 1;
			m_perlin_data[y* dims.x + x] = 0;

			//loop through the number of octaves - however many got passed in
			for (int o = 0; o < octaves; ++o)
			{
				//call glm::perlin function
				float perlin_sample = 
					glm::perlin(vec2(x, y) * scale * freq) * 0.5f + 0.5f; //we scale our points to the frequency
				perlin_sample *= amplitude; 
				m_perlin_data[y * dims.x + x] += perlin_sample;

				amplitude *= persistence; //each subsequent will contribute less
				freq *= 2.0f; //each octave will double the frequency of the previous one
			}
		}
	}
	
	//generate openGL texture handles
	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	//pass perlin data to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims.x, dims.y, 0, GL_RED, GL_FLOAT, m_perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

}