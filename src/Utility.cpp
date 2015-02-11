
#include <cstdio>
#include "gl_core_4_4.h"



bool LoadShaders(char* vertex_filename, char* fragment_filename, GLuint* result)
{
	bool succeeded = false;

	FILE* vertex_file = fopen(vertex_filename, "r");
	FILE* fragment_file = fopen(fragment_filename, "r");

	if (vertex_file == 0 || fragment_file == 0)
	{
		fclose(vertex_file);
		fclose(fragment_file);
		return succeeded;
	}
	else
	{
		fseek(vertex_file, 0, SEEK_END); //move search cursor to the end of the file
		int vf_length = ftell(vertex_file); //see how long the file is
		fseek(vertex_file, 0, SEEK_SET); //move cursor back to the start

		fseek(fragment_file, 0, SEEK_END); 
		int ff_length = ftell(fragment_file);
		fseek(fragment_file, 0, SEEK_SET);

		char* vs_source = new char[vf_length];
		char* fs_source = new char[ff_length];

		vf_length = fread(vs_source, 1, vf_length, vertex_file);
		ff_length = fread(fs_source, 1, ff_length, fragment_file);

		succeeded = true;

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vs_source, 0);
		glCompileShader(vertex_shader);

		glShaderSource(fragment_shader, 1, &fs_source, 0);
		glCompileShader(fragment_shader);

		*result = glCreateProgram();
		glAttachShader(*result, vertex_shader);
		glAttachShader(*result, fragment_shader);
		glLinkProgram(*result);


		//ERROR CHECKING
		int success = GL_FALSE;
		glGetProgramiv(*result, GL_LINK_STATUS, &success);

		if (success == GL_FALSE)
		{
			int log_length = 0;
			glGetProgramiv(*result, GL_INFO_LOG_LENGTH, &log_length);

			char* log = new char[log_length];
			glGetProgramInfoLog(*result, log_length, 0, log);
			printf("Error: Failed to link shader program!\n");
			printf("%s\n", log);
			delete[] log;

			succeeded = false;
		}

		glDeleteShader(fragment_shader);
		glDeleteShader(vertex_shader);

		delete vs_source;
		delete fs_source;
	}
	fclose(vertex_file);
	fclose(fragment_file);

	


	return succeeded;
}
