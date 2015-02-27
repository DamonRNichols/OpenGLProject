#include "Utility.h"
#include <cstdio>
#include "gl_core_4_4.h"

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
