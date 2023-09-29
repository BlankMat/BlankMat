#version 330 core

in GS
{
	vec3 VertColor;
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec3 dist;
};

out vec4 FragColor;

uniform float AmbientStrength;
uniform float SpecularStrength;

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 ViewPos;

void main()
{
    vec3 ambient = AmbientStrength * LightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * LightColor;

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 specular = SpecularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 64) * LightColor;
	
	float d = min(dist[0], min(dist[1], dist[2]));
	float I = exp2(-2.0*d*d);
	
	vec3 edgeColor = vec3(1.0, 0.372, 0.122);
	vec3 result = (ambient + diffuse + specular) * VertColor;
	
    FragColor = vec4(I*edgeColor + (1.0-I)*result, 1.0);
}