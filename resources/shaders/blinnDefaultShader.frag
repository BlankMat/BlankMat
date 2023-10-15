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

void main()
{
    // Normal in range [-1,1]
    vec3 normal = normalize(texture(material.texture_normal1, fs_in.TexCoords).rgb * 2.0 - 1.0);

    // Ambient
    vec3 ambientColor = texture(material.texture_ambient1, fs_in.TexCoords).rgb;
    vec3 ambient = light.ambient * material.ambient * ambientColor;

    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    vec3 diffuseColor = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0) * material.diffuse) * diffuseColor;

    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specularColor = texture(material.texture_specular1, fs_in.TexCoords).rgb;
    vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * material.specular) * specularColor;
	
    // Simple attenuation
    //float maxDistance = 1.5;
    //float dist = length(fs_in.TangentLightPos - fs_in.TangentFragPos);
	//TODO: float attenuation = 1.0f / (gamma ? dist * dist : dist);
    //float attenuation = 1.0f;
    //diffuse *= attenuation;
    //specular *= attenuation;

    // Result
	vec3 result = (ambient + diffuse + specular);
    if (gamma)
        result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result, 1.0);
}
