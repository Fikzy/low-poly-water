#version 420 core

layout(vertices = 3) out;

in vec3 vertexWorldPosistion[];
in vec3 vertexCameraPosistion[];
out vec3 vertexWorldPos[];
out vec3 vertexCameraPos[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    vertexWorldPos[gl_InvocationID] = vertexWorldPosistion[gl_InvocationID];
    vertexCameraPos[gl_InvocationID] = vertexCameraPosistion[gl_InvocationID];

    gl_TessLevelOuter[0] = 512; // left for triangles
    gl_TessLevelOuter[1] = 512; // bot for triangles
    gl_TessLevelOuter[2] = 512; // right for triangles

    gl_TessLevelInner[0] = 1024; // all inner sides for triangles
}
