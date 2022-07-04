#version 420 core

layout(location = 0) in vec2 vertexPosition;

out vec2 UV;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPosition, 0.0, 1.0);
    UV = vec2((vertexPosition.x + 1) / 2, 1 - (vertexPosition.y + 1) / 2);
}
