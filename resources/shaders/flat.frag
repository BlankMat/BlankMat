#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
    vec3 Normal;
} fs_in;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    int mode;           // 0 = Flat, 1 = Lambert, 2 = Blinn-Phong

    sampler2D texture_diffuse1;
    sampler2D texture_ambient1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    sampler2D texture_alpha1;
};

uniform Material material;

void main()
{
    if (gl_FrontFacing)
    {
        // Calculate all lighting
        FragColor = vec4(texture(material.texture_diffuse1, fs_in.TexCoords).rgb * material.diffuse, 1.0);
    }
    else
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
