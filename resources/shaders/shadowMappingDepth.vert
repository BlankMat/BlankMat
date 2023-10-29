#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 LightSpace;
uniform mat4 Model;

void main()
{
    gl_Position = LightSpace * Model * vec4(aPos, 1.0);
}
