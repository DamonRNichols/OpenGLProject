#version 410

in vec2 frag_texcoord;

out vec4 frag_color;

uniform sampler2D input_texture;
uniform float time;

vec4 BoxBlur()
{
	vec2 texel_uv = 1.0f / textureSize(input_texture,0).xy;

	vec4 color = texture(input_texture, frag_texcoord);

	color += texture(input_texture, frag_texcoord + texel_uv);

	color += texture(input_texture, frag_texcoord + vec2(texel_uv.x, 0));
	color += texture(input_texture, frag_texcoord + vec2(texel_uv.x, -texel_uv.y));
	color += texture(input_texture, frag_texcoord + vec2(0, texel_uv.y));
	color += texture(input_texture, frag_texcoord + vec2(0, -texel_uv.y));
	color += texture(input_texture, frag_texcoord + vec2(-texel_uv.x, texel_uv.y));
	color += texture(input_texture, frag_texcoord + vec2(-texel_uv.x, 0));
	color += texture(input_texture, frag_texcoord + vec2(-texel_uv.x, -texel_uv.y));

	color /= 9;

	return color;
}

vec4 Distort() 
{
	vec2 mid = vec2(0.5f);
	float distanceFromCentre = distance(frag_texcoord, mid);
	vec2 normalizedCoord = normalize(frag_texcoord - mid);
	float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.02f;
	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(input_texture, newCoord);
}



void main()
{
	frag_color = Distort() + BoxBlur(); //texture(input_texture, frag_texcoord);
}