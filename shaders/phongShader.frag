#version 330 core

in GS
{
	vec3 VertColor;
	vec3 FragPos;
	vec3 Normal;
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
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), 64) * SpecularStrength * LightColor;
	
	vec3 dist_vec = dist;
	float d = min(dist_vec[0], min(dist_vec[1], dist_vec[2]));
	float I = exp2(-2.0*d*d);
	
	vec3 edgeColor = vec3(1.0, 0.372, 0.122);
	vec3 first = I*edgeColor;
	vec3 second = (ambient + diffuse + specular) * VertColor;
	vec3 result = first + (1.0 - I)*second;
	
    FragColor = vec4(result, 1.0);
}