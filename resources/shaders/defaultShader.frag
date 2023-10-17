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
	vec3 position;		// Point + Spotlight
	vec3 direction;		// Directional + Spotlight

	vec3 ambient;       // All lights
	vec3 diffuse;       // All lights
	vec3 specular;      // All lights

	float pointC;		// Point only
	float pointL;		// Point only
	float pointQ;	    // Point only
	
	float spotInner;	// Spotlight only
	float spotOuter;	// Spotlight only
    int type;           // 0=point, 1=dir, 2=spot
    bool gamma;         // Gamma correct this light
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

vec3 CalcPointLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir);
vec3 CalcDirLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir);

void main()
{
    // Gather texture and material information
    vec3 normal = normalize(texture(material.texture_normal1, fs_in.TexCoords).rgb * 2.0 - 1.0);
    vec3 ambientColor = texture(material.texture_ambient1, fs_in.TexCoords).rgb * material.ambient;
    vec3 diffuseColor = texture(material.texture_diffuse1, fs_in.TexCoords).rgb * material.diffuse;
    vec3 specularColor = texture(material.texture_specular1, fs_in.TexCoords).rgb * material.specular;
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    // Calculate all lighting
    vec3 result = vec3(0,0,0);
    if (light.type == 0)
        result += CalcPointLight(light, normal, ambientColor, diffuseColor, specularColor, viewDir);
    else if (light.type == 1)
        result += CalcDirLight(light, normal, ambientColor, diffuseColor, specularColor, viewDir);
    else if (light.type == 2)
        result += CalcSpotLight(light, normal, ambientColor, diffuseColor, specularColor, viewDir);

    if (light.gamma)
        result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result, 1.0);
}

// Calculates the color for a point light
vec3 CalcPointLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * ambientColor;

    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0)) * diffuseColor;

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess)) * specularColor;
    
    // Attenuation
    float dist = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.pointC + light.pointL * dist + light.pointQ * (dist * dist));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// Calculates the color for a a directional light
vec3 CalcDirLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * ambientColor;

    // Diffuse
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0)) * diffuseColor;

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess)) * specularColor;
    
    return (ambient + diffuse + specular);
}

// Calculates the color for a a spotlight
vec3 CalcSpotLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float theta = dot(lightDir, normalize(-light.direction)); 

    // Ambient
    vec3 ambient = light.ambient * ambientColor;

    // Only light inside of the spotlight
    if (theta > light.spotInner)
    {
        // Diffuse
        vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0)) * diffuseColor;

        // Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess)) * specularColor;

        // Spotlight edges
        float epsilon = (light.spotInner - light.spotOuter);
        float intensity = clamp((theta - light.spotOuter) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    
        // Attenuation
        float dist = length(light.position - fs_in.FragPos);
        float attenuation = 1.0 / (light.pointC + light.pointL * dist + light.pointQ * (dist * dist));
        diffuse *= attenuation;
        specular *= attenuation;

        return (ambient + diffuse + specular);
    }
    // Only use ambient if outside the spotlight
    else
    {
        return ambient;
    }
}