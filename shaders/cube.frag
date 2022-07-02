#version 420 core

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    // color = fragmentColor;
    color = texture(textureSampler, UV);
}
