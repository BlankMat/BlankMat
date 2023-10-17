#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec4 LightSpaceFragPos;
} vs_out;

struct Light {
	vec3 position;		// Point + Spotlight
	vec3 direction;		// Directional + Spotlight

	vec3 ambient;       // All lights
	vec3 diffuse;       // All lights
	vec3 specular;      // All lights

	float pointC;		// Point only
	float pointL;		// Point only
	float pointQ;	    // Point only
	
	float spotInner;	// Spotlight only
	float spotOuter;	// Spotlight only
    int type;           // 0=point, 1=dir, 2=spot
    bool gamma;         // Gamma correct this light
};

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;
uniform mat4 LightSpace;

uniform Light light;
uniform vec3 viewPos;

void main()
{
	vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	// Calculate normal Tangent, Normal, and Bitangents
	vec3 T = normalize(NormalModel * aTangent);
	vec3 N = normalize(NormalModel * aNorm);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	// Calculate tangent matrices before fragment
	mat3 TBN = transpose(mat3(T, B, N));
	vs_out.TangentLightPos = TBN * light.position;
	vs_out.TangentViewPos = TBN * viewPos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;
	vs_out.LightSpaceFragPos = LightSpace * vec4(vs_out.FragPos, 1.0);

	gl_Position = MVP * vec4(aPos, 1.0);
}