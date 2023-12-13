#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragColor;
} fs_in;

void main()
{
    FragColor = vec4(fs_in.FragColor, 1.0);
}
