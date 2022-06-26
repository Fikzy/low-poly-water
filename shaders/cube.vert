#version 420 core

layout(location = 0) in vec3 position; // per vert: position
layout(location = 1) in vec3 color; // per vert: color

out vec3 fragmentColor;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1);
    fragmentColor = color;
}
