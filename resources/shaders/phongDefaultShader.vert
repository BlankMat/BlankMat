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
} vs_out;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

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

	gl_Position = MVP * vec4(aPos, 1.0);
}