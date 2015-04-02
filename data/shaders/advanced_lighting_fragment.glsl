#version 410

in vec4 frag_normal;
in vec4 frag_position;

out vec4 frag_color;

uniform vec3 light_dir;
uniform vec3 light_color;

uniform vec3 material_color;

uniform vec3 ambient_light;

uniform vec3 eye_pos;
uniform float specular_power;

void main()
{
	float roughness = 0.5f;

	vec3 N = normalize(frag_normal.xyz);				//N - is the vector the light is coming from (surface to light)
	vec3 L = normalize(light_dir);						//L - is the surface normal
	vec3 E = normalize(eye_pos - frag_position.xyz);	//E - is vector of the surface to the viewer

	float NdL = max( 0.0f, dot( N, L ) );
	float NdE = max( 0.0f, dot( N, E ) );

	float R2 = roughness * roughness;

	vec3 H = normalize( L + E ); // light and view half vector
	float NdH = max( dot( N, H ), 0.0f );
	float NdH2 = NdH * NdH; 

	//Oren-Nayar Diffuse Term
	float A = 1.0f - 0.5f * R2 / (R2 + 0.33f);
	float B = 0.45f * R2 / (R2 + 0.09f);

	//CX = Max(0, cos(l,e))
	vec3 lightProjected = normalize( L - N * NdL );
	vec3 viewProjected = normalize( E - N * NdE);
	float CX = max( 0.0f, dot( lightProjected, viewProjected ) );

	//DX = sin(alpha) * tan(beta)
	float alpha = sin( max( acos( NdE ), acos( NdL ) ) );
	float beta = tan( min( acos( NdE ), acos( NdL ) ) );
	float DX = alpha * beta;

	//Calculate Oren-Nayar, replaces the Phong Lambertian Term
	float OrenNayarDiffuse = NdL * (A + B * CX * DX);



	float e = 2.71828182845904523536028747135f;
	float pi = 3.1415926535897932384626433832f;

	//Beckman's Distribution Function D
	float exponent = -(1 - NdH2) / (NdH2 * R2);
	float D = pow( e, exponent ) / (R2 * NdH2 * NdH2);

	//Fresnel Term F
	float FresnelScale = 0.5f;
	float HdE = dot( H, E );
	float F = mix( pow( 1 - HdE, 5 ), 1, FresnelScale );

	//Geometric Attenuation Factor G
	float X = 2.0f * NdH / dot( E, H );
	float G = min(1, min(X * NdL, X * NdE));

	//Calculate Cook-Torrance
	float CookTorranceSpecular = max( (D*G*F) / (NdE * pi), 0.0f );

	frag_color = vec4(material_color * (OrenNayarDiffuse + CookTorranceSpecular), 1);
}