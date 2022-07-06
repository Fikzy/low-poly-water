#version 420 core

in vec4 gClipSpace;
in vec3 gNormal;
in vec3 gWorldPosition;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthTextureMap;

uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightAmbient;

out vec4 color;

float near = 0.1;
float far = 1000;

void main()
{
    vec2 ndc = (gClipSpace.xy / gClipSpace.w) / 2 + 0.5;
    vec2 reflectTexcoords = vec2(ndc.x, -ndc.y);
    vec2 refractTexcoords = vec2(ndc.x, ndc.y);

    float depth = texture(depthTextureMap, refractTexcoords).r;
    float floorDistance =
        2 * near * far / (far + near - (2 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDistance =
        2 * near * far / (far + near - (2 * depth - 1.0) * (far - near));
    float waterDepth = floorDistance - waterDistance;

    refractTexcoords = clamp(refractTexcoords, 0.001, 0.999);

    vec4 reflectColor = texture(reflectionTexture, reflectTexcoords);
    vec4 refractColor = texture(refractionTexture, refractTexcoords);

    vec3 viewVector = normalize(cameraPosition - gWorldPosition);
    float refractiveFactor = dot(viewVector, vec3(0, 1, 0));

    color = mix(reflectColor, refractColor, refractiveFactor);
    color = mix(color, vec4(0, 0.3, 0.5, 1), 0.2); // blue tint

    vec3 lightDir = normalize(lightPosition);
    float diff = max(dot(gNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    color = vec4(lightAmbient + diffuse, 1) * color;

    color.a = clamp(waterDepth / 10, 0, 1);
}
