#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

void main()
{
	TexCoords = aTexCoords;
	FragPos = vec3(Model * vec4(aPos, 1.0));
	Normal = NormalModel * aNorm;
	gl_Position = MVP * vec4(aPos, 1.0);
}