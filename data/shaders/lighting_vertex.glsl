#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;

out vec4 frag_normal;
out vec3 frag_position;

uniform mat4 projection_view;

void main()
{
	frag_position = position.xyz;
	frag_normal = normal;
	gl_Position = projection_view * position;
}