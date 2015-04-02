#include "Utility.h"
#include <cstdio>
#include "gl_core_4_4.h"
#include "tiny_obj_loader.h"

bool LoadShaderType(char* filename, GLenum shader_type, unsigned int* output)
{
	//we want to be able to return if succeeded
	bool succeeded = true;

	//open the shader file
	FILE* shader_file = fopen(filename, "r");

	//did it open successfully
	if (shader_file == 0)
	{
		fclose(shader_file);
		return succeeded;
	}
	else
	{
		//find out how long the file is
		fseek(shader_file, 0, SEEK_END); //move search cursor to the end of the file
		int length = ftell(shader_file); //see how long the file is
		fseek(shader_file, 0, SEEK_SET); //move cursor back to the start

		//allocate enough space for the file
		char* shader_source = new char[length];
		//read the dile and update the length to be accurate
		length = fread(shader_source, 1, length, shader_file);
		//create the shader based on the type that was passed in
		unsigned int shader = glCreateShader(shader_type);

		//compile the shader
		glShaderSource(shader, 1, &shader_source, &length);
		glCompileShader(shader);

		//ERROR CHECKING
		int success = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			int log_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

			char* log = new char[log_length];
			glGetShaderInfoLog(shader, log_length, NULL, log);
			printf("Error: Failed to link shader program!\n");
			printf("%s\n", log);
			delete[] log;

			succeeded = false;
		}
		//only give the result to the caller if we succeeded
		if (succeeded)
		{
			*output = shader;
		}
		delete[] shader_source;
		fclose(shader_file);
	}
	return succeeded;
}

bool LoadShaders(char* vertex_filename, char* geometry_filename, char* fragment_filename, GLuint* result)
{
	bool succeeded = true;

	*result = glCreateProgram();

	unsigned int vertex_shader;
	if (LoadShaderType(vertex_filename, GL_VERTEX_SHADER, &vertex_shader))
	{
		glAttachShader(*result, vertex_shader);
		glDeleteShader(vertex_shader);
	}
	else
	{
		printf("FAILED TO LOAD THE VERTEX FILE");
	}

	if (geometry_filename != nullptr)
	{
		unsigned int geometry_shader;
		if (LoadShaderType(geometry_filename, GL_GEOMETRY_SHADER, &geometry_shader))
		{
			glAttachShader(*result, geometry_shader);
			glDeleteShader(geometry_shader);
		}
		else
		{
			printf("FAILED TO LOAD THE GEOMETRY FILE");
		}
	}
	if (fragment_filename != nullptr)
	{
		unsigned int fragment_shader;
		if (LoadShaderType(fragment_filename, GL_FRAGMENT_SHADER, &fragment_shader))
		{
			glAttachShader(*result, fragment_shader);
			glDeleteShader(fragment_shader);
		}
		else
		{
			printf("FAILED TO LOAD THE FRAGMENT FILE");
		}
	}
		
	
	glLinkProgram(*result);

	GLint success;
	glGetProgramiv(*result, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int log_length = 0;
		glGetProgramiv(*result, GL_INFO_LOG_LENGTH, &log_length);

		char* log = new char[log_length];
		glGetProgramInfoLog(*result, log_length, NULL, log);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", log);
		delete[] log;

		succeeded = false;
	}

	return succeeded;
}


OpenGLData LoadOBJ(char* filename)
{
	OpenGLData result = {};

	std::vector<tinyobj::shape_t>shapes;
	std::vector<tinyobj::material_t>materials;

	std::string error = tinyobj::LoadObj(shapes, materials, filename);

	if (error.size() != 0)
	{
		return result;
	}

	result.m_index_count = shapes[0].mesh.indices.size();

	tinyobj::mesh_t* mesh = &shapes[0].mesh;

	std::vector<float> vertex_data;
	vertex_data.reserve(mesh->positions.size() + mesh->normals.size());

	vertex_data.insert(vertex_data.end(), mesh->positions.begin(), mesh->positions.end());
	vertex_data.insert(vertex_data.end(), mesh->normals.begin(), mesh->normals.end());

	glGenVertexArrays(1, &result.m_VAO);
	glBindVertexArray(result.m_VAO);

	glGenBuffers(1, &result.m_VBO);
	glGenBuffers(1, &result.m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, result.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* mesh->indices.size(), mesh->indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //normal data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)* mesh->positions.size()));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return result;
}