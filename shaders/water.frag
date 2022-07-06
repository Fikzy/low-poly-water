#version 420 core

in vec4 clipSpace;
in vec3 vertexWorldPosition;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform vec3 cameraPosition;

out vec4 color;

void main()
{
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2 + 0.5;
    vec2 reflectTexcoords = vec2(ndc.x, -ndc.y);
    vec2 refractTexcoords = vec2(ndc.x, ndc.y);

    refractTexcoords = clamp(refractTexcoords, 0.001, 0.999);

    vec4 reflectColor = texture(reflectionTexture, reflectTexcoords);
    vec4 refractColor = texture(refractionTexture, refractTexcoords);

    vec3 viewVector = normalize(cameraPosition - vertexWorldPosition);
    float refractiveFactor = dot(viewVector, vec3(0, 1, 0));

    color = mix(reflectColor, refractColor, refractiveFactor);
    color = mix(color, vec4(0, 0.3, 0.5, 1), 0.2); // blue tint
}
