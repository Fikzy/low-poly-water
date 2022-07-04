#version 420 core

in vec2 texcoords;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    color = texture(textureSampler, texcoords);
}
