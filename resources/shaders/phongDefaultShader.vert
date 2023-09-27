#version 330 core
layout (location = 0) in float isSelected;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 VertColor;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

void main()
{
	TexCoords = aTexCoords;
	VertColor = aColor;
	FragPos = vec3(Model * vec4(aPos, 1.0));
	Normal = NormalModel * aNorm;
	gl_Position = MVP * vec4(aPos, 1.0);
}