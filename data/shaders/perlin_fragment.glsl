#version 410

in vec2 frag_texcoord;
in vec4 out_pos;

out vec4 frag_color;

uniform sampler2D perlin_texture;

void main()
{
	frag_color = texture(perlin_texture, frag_texcoord).rrrr;
	
	frag_color.a = 1;
}