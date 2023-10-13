#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
    vec3 Normal;
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
	vs_out.Normal = aNorm;
	gl_Position = MVP * vec4(aPos, 1.0);
}