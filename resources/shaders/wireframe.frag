#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
    vec3 Normal;
} fs_in;

void main()
{
    FragColor = vec4(0.0, 0.0, 0.769, 1.0);
}
