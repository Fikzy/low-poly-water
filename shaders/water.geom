#version 420 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 clipSpace[];

uniform mat4 projection;
uniform mat4 view;

out vec4 gClipSpace;
out vec3 gNormal;
out vec3 gWorldPosition;

vec3 ComputeNormal()
{
    vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    return normalize(cross(a, b));
}

void main()
{
    vec3 normal = ComputeNormal();
    mat4 pv = projection * view;

    gl_Position = pv * gl_in[0].gl_Position;
    gClipSpace = pv * clipSpace[0];
    gNormal = normal;
    gWorldPosition = vec3(gl_in[0].gl_Position);
    EmitVertex();

    gl_Position = pv * gl_in[1].gl_Position;
    gClipSpace = pv * clipSpace[1];
    gNormal = normal;
    gWorldPosition = vec3(gl_in[1].gl_Position);
    EmitVertex();

    gl_Position = pv * gl_in[2].gl_Position;
    gClipSpace = pv * clipSpace[2];
    gNormal = normal;
    gWorldPosition = vec3(gl_in[2].gl_Position);
    EmitVertex();

    EndPrimitive();
}
