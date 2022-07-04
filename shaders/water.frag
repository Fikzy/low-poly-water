#version 420 core

in vec3 fragmentColor;

out vec3 color;

uniform sampler2D textureSampler;

void main()
{
    color = fragmentColor;
}
