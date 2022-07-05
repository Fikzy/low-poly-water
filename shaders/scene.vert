#version 420 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexcoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec4 plane;

out vec2 texcoords;
out vec3 normal;

void main()
{
    vec4 worldPosition = vec4(vertexPosition, 1.0);

    gl_ClipDistance[0] = dot(worldPosition, plane);

    gl_Position = projection * view * model * worldPosition;

    texcoords = vertexTexcoords;
    normal = normalize(vertexNormal);
}
