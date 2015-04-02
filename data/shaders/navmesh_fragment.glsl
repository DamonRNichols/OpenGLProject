#version 410

in vec4 world_pos;
in vec4 world_normal;

out vec4 frag_color;

void main()
{
	vec3 color = vec3(1);

	if (fract(world_pos.x) < 0.05 ||fract(world_pos.y) < 0.05 ||fract(world_pos.z) < 0.05)
	{
		color = vec3(0);
	}

	vec3 L = normalize(vec3(1));
	vec3 N = normalize(world_normal.xyz);

	float d = max(0, dot(N,L)) * 0.75;

	frag_color.rgb = color * 0.25 + color * d;
	frag_color.a = 1;
}