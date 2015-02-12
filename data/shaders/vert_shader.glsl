#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
out vec4 out_color;
uniform mat4 projection_view;

uniform float time;
uniform float heightScale;

void main()
{
	vec4 P = position;
	P.y += sin( time + position.x ) + cos( time + position.z );
//out_color = color;
//out_color.x = color.x + sin(time + position.x) + sin( time + position.z );
//out_color.y = color.y + cos(time + position.x) + cos( time + position.z );
//out_color.z = color.z + tan(time + position.x) + tan( time + position.z );
out_color.x = P.y;
out_color.y = P.y;
out_color.z = 0;
out_color.w = color.w;
	gl_Position = projection_view * P;
}