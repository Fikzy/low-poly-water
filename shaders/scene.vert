#version 420 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexcoords;

out vec2 texcoords;

uniform mat4 MVP;
uniform vec4 plane;

void main()
{
    vec4 worldPosition = vec4(1) * vec4(vertexPosition, 1.0);

    gl_ClipDistance[0] = dot(worldPosition, plane);

    gl_Position = MVP * worldPosition;

    texcoords = vertexTexcoords;
}
