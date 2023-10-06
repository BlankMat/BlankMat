#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

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
uniform sampler2D texture_ambient1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform sampler2D texture_alpha1;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform bool gamma;

void main()
{
    // Normal in range [-1,1]
    vec3 normal = normalize(texture(texture_normal1, fs_in.TexCoords).rgb * 2.0 - 1.0);

    // Ambient
    vec3 ambientColor = texture(texture_ambient1, fs_in.TexCoords).rgb;
    vec3 ambient = light.ambient * material.ambient * ambientColor;

    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos);
    vec3 diffuseColor = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0) * material.diffuse) * diffuseColor;
	
    // Simple attenuation
    float maxDistance = 1.5;
    float dist = length(fs_in.TangentLightPos - fs_in.TangentFragPos);
	//TODO: float attenuation = 1.0f / (gamma ? dist * dist : dist);
    float attenuation = 1.0f;
    diffuse *= attenuation;

    // Result
	vec3 result = (ambient + diffuse);
    if (gamma)
        result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result, 1.0);
}
