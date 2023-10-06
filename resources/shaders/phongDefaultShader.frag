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
    vec3 ambient = light.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 specular = light.specular * pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	
    // Simple attenuation
    float maxDistance = 1.5;
    float dist = length(light.position - FragPos);
	//TODO: float attenuation = 1.0f / (gamma ? dist * dist : dist);
    float attenuation = 1.0f;
    diffuse *= attenuation;
    specular *= attenuation;

    // Result
    vec3 textureColor = texture(texture_diffuse1, TexCoords).rgb;
	vec3 result = (ambient + diffuse + specular) * textureColor;
    if (gamma)
        result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result, 1.0);
}
