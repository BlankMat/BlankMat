#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform bool gamma;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position);
    vec3 diffuse = light.diffuse * (max(dot(norm, lightDir), 0.0));

    vec3 textureColor = vec3(texture(texture_diffuse1, TexCoords));
	vec3 result = (ambient + diffuse) * textureColor;
	
    FragColor = vec4(result, 1.0);
}
