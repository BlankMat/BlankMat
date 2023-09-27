#version 330 core
layout (location = 0) in float isSelected;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec2 aTexCoords;

out VS
{
	vec3 geoVertColor;
	float vertExcludeEdge;
};

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

uniform float AmbientStrength;
uniform float SpecularStrength;

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 ViewPos;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);

    vec3 normal = NormalModel * aNorm;
    vec3 fragPos = vec3(Model * vec4(aPos, 1.0f));
   
    vec3 ambient = AmbientStrength * LightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(LightPos - fragPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * LightColor;

    vec3 viewDir = normalize(ViewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), 32) * SpecularStrength * LightColor;

	if (isSelected == 1.0)
      vertExcludeEdge = 0.0;
	else
      vertExcludeEdge = 1.0;

    geoVertColor = (ambient + diffuse + specular) * aColor;
}