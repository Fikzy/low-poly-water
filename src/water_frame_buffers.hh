#pragma once

#include <glad.h>
#include <glm/glm.hpp>

#define REFLECTION_RES 0.5
#define REFRACTION_RES 1.0

class WaterFrameBuffers
{
public:
    WaterFrameBuffers(int screenW, int screenH);
    ~WaterFrameBuffers();

    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();

    void initialiseReflectionFrameBuffer();
    void initialiseRefractionFrameBuffer();
    void bindFrameBuffer(GLuint frameBuffer, int width, int height);

    GLuint createFrameBuffer();
    GLuint createTextureAttachment(int width, int height);
    GLuint createDepthTextureAttachment(int width, int height);
    GLuint createDepthBufferAttachment(int width, int height);

public:
    GLuint reflectionTexture;

    GLuint refractionTexture;
    GLuint refractionDepthTexture;

private:
    int screenW, screenH;
    int refractionW, refractionH;
    int reflectionW, reflectionH;

    GLuint reflectionFrameBuffer;
    GLuint reflectionDepthBuffer;

    GLuint refractionFrameBuffer;
};
