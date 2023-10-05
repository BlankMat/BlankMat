#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform vec3 ViewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * (max(dot(norm, lightDir), 0.0) * material.diffuse);

    // Specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specular = light.specular * (pow(max(dot(norm, halfwayDir), 0.0), material.shininess) * material.specular);
	
    // Result
    vec3 textureColor = vec3(texture(texture_diffuse1, TexCoords));
	vec3 result = (ambient + diffuse + specular) * textureColor;
	
    FragColor = vec4(result, 1.0);
}
