#version 330 core

in vec3 FragPos;
in vec3 VertColor;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D TextureDiffuse1;
uniform float AmbientStrength;
uniform float SpecularStrength;

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 ViewPos;

void main()
{
    // Ambient
    vec3 ambient = AmbientStrength * LightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * LightColor;

    // Specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 specular = SpecularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32) * LightColor;
	
    // Result
    // vec3 textureColor = vec3(texture(TextureDiffuse1, TexCoords));
	vec3 result = (ambient + diffuse + specular) * VertColor;
	
    FragColor = vec4(result, 1.0);
}
