#version 330 core
layout (location = 0) in float isSelected;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec2 aTexCoords;

out VS
{
	vec3 geoVertColor;
	vec3 geoFragPos;
	vec3 geoNormal;
	vec2 texCoords;
	float vertExcludeEdge;
};

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

void main()
{
	texCoords = aTexCoords;
	gl_Position = MVP * vec4(aPos, 1.0);
	geoVertColor = aColor;
	geoNormal = NormalModel * aNorm;
	if (isSelected == 1.0)
		vertExcludeEdge = 0.0;
	else
		vertExcludeEdge = 1.0;
	geoFragPos = vec3(Model * vec4(aPos, 1.0f));
}