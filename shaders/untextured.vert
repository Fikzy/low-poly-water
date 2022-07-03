#version 420 core

vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 lightPosition = vec3(1.0, 1.0, 5.0);

vec3 materialColor = vec3(0.0, 0.0, 1.0);
vec3 materialDiffuseColor = materialColor * 0.3;

float cosTheta;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 fragmentColor;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPosition, 1.0);
    cosTheta = clamp(dot(vertexNormal, lightPosition), 0, 1);
    fragmentColor =
        materialDiffuseColor + lightColor * materialColor * cosTheta;
}
