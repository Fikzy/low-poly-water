#version 420 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexcoords;

uniform mat4 MVP;

out vec4 clipSpace;

void main()
{
    vec4 worldPosition = vec4(1) * vec4(vertexPosition, 1.0);

    clipSpace = MVP * worldPosition;
    gl_Position = clipSpace;
}
