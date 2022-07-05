#include "water_frame_buffers.hh"

#include <iostream>

WaterFrameBuffers::WaterFrameBuffers(float screenW, float screenH)
{
    setResolution(screenW, screenH);
}

void WaterFrameBuffers::setResolution(float screenW, float screenH)
{
    this->screenW = screenW;
    this->screenH = screenH;
    initialiseReflectionFrameBuffer();
    initialiseRefractionFrameBuffer();
}

void WaterFrameBuffers::bindReflectionFrameBuffer()
{
    bindFrameBuffer(reflectionFrameBuffer, REFLECTION_RES * screenW,
                    REFLECTION_RES * screenH);
}

void WaterFrameBuffers::bindRefractionFrameBuffer()
{
    bindFrameBuffer(refractionFrameBuffer, REFRACTION_RES * screenW,
                    REFRACTION_RES * screenH);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenW, screenH);
}

void WaterFrameBuffers::initialiseReflectionFrameBuffer()
{
    initFrameBuffer(&reflectionFrameBuffer);
    initTextureAttachment(&reflectionTexture, REFLECTION_RES * screenW,
                          REFLECTION_RES * screenH);
    initDepthBufferAttachment(&reflectionDepthBuffer, REFLECTION_RES * screenW,
                              REFLECTION_RES * screenH);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initialiseRefractionFrameBuffer()
{
    initFrameBuffer(&refractionFrameBuffer);
    initTextureAttachment(&refractionTexture, REFRACTION_RES * screenW,
                          REFRACTION_RES * screenH);
    initDepthTextureAttachment(&refractionDepthTexture,
                               REFRACTION_RES * screenW,
                               REFRACTION_RES * screenH);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::bindFrameBuffer(GLuint frameBuffer, int width,
                                        int height)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

void WaterFrameBuffers::initFrameBuffer(GLuint *frameBuffer)
{
    glGenFramebuffers(1, frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void WaterFrameBuffers::initTextureAttachment(GLuint *texture, int width,
                                              int height)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *texture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterFrameBuffers::initDepthTextureAttachment(GLuint *texture, int width,
                                                   int height)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *texture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterFrameBuffers::initDepthBufferAttachment(GLuint *depthBuffer,
                                                  int width, int height)
{
    glGenRenderbuffers(1, depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, *depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, *depthBuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
