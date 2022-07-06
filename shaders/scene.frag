#version 420 core

in vec2 texcoords;
in vec3 normal;

uniform sampler2D textureMap;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 lightAmbient;

out vec4 color;

float ambientStrength = 0.3;

void main()
{
    vec3 lightDir = normalize(-lightDirection);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    color = vec4(lightAmbient + diffuse, 1) * texture(textureMap, texcoords);
}
