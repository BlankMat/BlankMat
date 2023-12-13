#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
	vec3 FragColor;
} vs_out;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_ambient1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    sampler2D texture_alpha1;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform bool gamma;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

void main()
{
	vec3 fragPos = vec3(Model * vec4(aPos, 1.0));
	vec2 texCoords = aTexCoords;
	vec3 normal = normalize(NormalModel * aNorm);
	gl_Position = MVP * vec4(aPos, 1.0);

    // Calculate lighting
    // Ambient
    vec3 ambientColor = texture(material.texture_ambient1, texCoords).rgb;
    vec3 ambient = light.ambient * material.ambient * ambientColor;

    // Diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 diffuseColor = texture(material.texture_diffuse1, texCoords).rgb;
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0) * material.diffuse) * diffuseColor;

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specularColor = texture(material.texture_specular1, texCoords).rgb;
    vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * material.specular) * specularColor;

    // Result
	vs_out.FragColor = (ambient + diffuse + specular);
}