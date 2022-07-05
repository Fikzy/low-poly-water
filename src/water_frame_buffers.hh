#pragma once

#include <glad.h>
#include <glm/glm.hpp>

#define REFLECTION_RES 0.5
#define REFRACTION_RES 1.0

class WaterFrameBuffers
{
public:
    WaterFrameBuffers(float screenW, float screenH);

    void setResolution(float screenW, float screenH);

    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();

    void initialiseReflectionFrameBuffer();
    void initialiseRefractionFrameBuffer();
    void bindFrameBuffer(GLuint frameBuffer, int width, int height);

    void initFrameBuffer(GLuint *frameBuffer);
    void initTextureAttachment(GLuint *texture, int width, int height);
    void initDepthTextureAttachment(GLuint *texture, int width, int height);
    void initDepthBufferAttachment(GLuint *depthBuffer, int width, int height);

public:
    GLuint reflectionTexture;

    GLuint refractionTexture;
    GLuint refractionDepthTexture;

private:
    float screenW, screenH;
    int refractionW, refractionH;
    int reflectionW, reflectionH;

    GLuint reflectionFrameBuffer;
    GLuint reflectionDepthBuffer;

    GLuint refractionFrameBuffer;
};
