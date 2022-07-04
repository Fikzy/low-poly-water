#pragma once

#include <glad.h>
#include <glm/glm.hpp>

#define REFLECTION_WIDTH 320
#define REFLECTION_HEIGHT 180
#define REFRACTION_WIDTH 1280
#define REFRACTION_HEIGHT 720

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

    GLuint reflectionFrameBuffer;
    GLuint reflectionDepthBuffer;

    GLuint refractionFrameBuffer;
};
