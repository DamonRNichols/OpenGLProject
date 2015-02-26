#version 410

in vec2 frag_tex_coord;
in vec4 vert_normal;
out vec4 frag_color;
uniform sampler2D diffuse;

void main()
{
	frag_color = texture(diffuse, frag_tex_coord);
}