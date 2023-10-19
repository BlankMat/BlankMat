#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
    vec4 LightSpaceFragPos;
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

uniform sampler2D shadowMap;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

float CalcShadows(vec3 normal);
vec3 CalcPointLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir, float shadow);
vec3 CalcDirLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir, float shadow);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir, float shadow);

void main()
{
    if (gl_FrontFacing)
    {
        // Gather texture and material information
        vec3 normal = normalize(texture(material.texture_normal1, fs_in.TexCoords).rgb * 2.0 - 1.0);
        vec3 ambientColor = texture(material.texture_ambient1, fs_in.TexCoords).rgb * material.ambient;
        vec3 diffuseColor = texture(material.texture_diffuse1, fs_in.TexCoords).rgb * material.diffuse;
        vec3 specularColor = texture(material.texture_specular1, fs_in.TexCoords).rgb * material.specular;
        vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
        float shadow = CalcShadows(normal);

        // Calculate all lighting
        vec3 result = vec3(0,0,0);
        if (light.type == 0)
            result += CalcPointLight(light, normal, ambientColor, diffuseColor, specularColor, viewDir, shadow);
        else if (light.type == 1)
            result += CalcDirLight(light, normal, ambientColor, diffuseColor, specularColor, viewDir, shadow);
        else if (light.type == 2)
            result += CalcSpotLight(light, normal, ambientColor, diffuseColor, specularColor, viewDir, shadow);

        if (light.gamma)
            result = pow(result, vec3(1.0/2.2));
        FragColor = vec4(result, 1.0);   
    }
    else
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

// Calculates shadows for this fragment
float CalcShadows(vec3 normal)
{
    // Perform perspective divide in range [0,1]
    vec3 projCoords = (fs_in.LightSpaceFragPos.xyz / fs_in.LightSpaceFragPos.w) * 0.5 + 0.5;
    // Get closest depth value from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    // Average neighboring texels
    shadow /= 9.0;

    // Keep shadow at 0 when outside the light's far plane
    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

// Calculates the color for a point light
vec3 CalcPointLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir, float shadow)
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

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

// Calculates the color for a a directional light
vec3 CalcDirLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir, float shadow)
{
    // Ambient
    vec3 ambient = light.ambient * ambientColor;

    // Diffuse
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0)) * diffuseColor;

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess)) * specularColor;
    
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

// Calculates the color for a a spotlight
vec3 CalcSpotLight(Light light, vec3 normal, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, vec3 viewDir, float shadow)
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

        return (ambient + (1.0 - shadow) * (diffuse + specular));
    }
    // Only use ambient if outside the spotlight
    else
    {
        return ambient;
    }
}