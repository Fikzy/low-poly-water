#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>

#include "camera.hh"
#include "gui_renderer.hh"
#include "model.hh"
#include "shader.hh"
#include "texture.hh"
#include "utils.hh"
#include "water_frame_buffers.hh"

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void updateCameraRotation(GLFWwindow *window);
void processInput(GLFWwindow *window);

float SCREEN_W = 1920.0f / 2;
float SCREEN_H = 1080.0f / 2;
float FOV = 65.0f;
float SENSITIVITY = 0.1f;

const float NEAR_CLIP = 0.1f;
const float FAR_CLIP = 1000.0f;

double lastXPos = SCREEN_W / 2, lastYPos = SCREEN_H / 2;
double yaw = 0, pitch = 0, xPos, yPos;

Camera *camera;
glm::highp_mat4 projection;
std::map<int, bool> heldKeys;

const float WATER_LEVEL = 1.0;

int main()
{
    srand(time(NULL));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window =
        glfwCreateWindow(SCREEN_W, SCREEN_H, "POGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Camera
    camera = new Camera(glm::vec3(-8.0f, 2.0f, 0.0f));
    projection = glm::perspective(
        glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);

    // Shaders
    Shader waterShader("water.vert", "water.frag");
    Shader sceneShader("scene.vert", "scene.frag");

    // Water
    WaterFrameBuffers fbos = WaterFrameBuffers(SCREEN_W, SCREEN_H);
    GLuint clipPlaneLocation = glGetUniformLocation(sceneShader.id, "plane");

    Model waterPlane("assets/water_plane.obj", &waterShader);
    waterPlane.addTexture("reflectionTexture", fbos.reflectionTexture);
    waterPlane.addTexture("refractionTexture", fbos.refractionTexture);

    // Textures
    GLuint sceneTexture = loadTexture("assets/scene_palette.png");

    Model scene("assets/lake_scene.obj", &sceneShader);
    scene.addTexture("textureMap", sceneTexture);

    // GUI - Debug water textures
    auto guiRenderer = GuiRenderer();
    // guiRenderer.addElement(fbos.reflectionTexture, glm::vec2(-0.6, 0.5f),
    //                        glm::vec2(0.3, -0.3));
    // guiRenderer.addElement(fbos.refractionTexture, glm::vec2(0.6, 0.5f),
    //                        glm::vec2(0.3, -0.3));

    glClearColor(0, 0.1f, 0.2f, 0.8f);
    while (!glfwWindowShouldClose(window))
    {
        Camera::updateDeltaTime();

        glEnable(GL_CLIP_DISTANCE0);

        processInput(window);

        auto modelMatrix = glm::mat4(1.0);
        auto scaledModelMatrix =
            glm::scale(glm::mat4(1.0), glm::vec3(50, 1, 50));

        // Move camera and under water
        auto dist = 2 * (camera->getPosition().y - WATER_LEVEL);
        camera->position.y -= dist;
        camera->rotate(yaw, -pitch);

        // Compute under water projection matrix
        auto reflectionMVP =
            projection * camera->getWorldToViewMatrix() * modelMatrix;

        // Move camera back above water
        camera->position.y += dist;
        camera->rotate(yaw, pitch);

        // Projection
        auto mvp = projection * camera->getWorldToViewMatrix() * modelMatrix;

        // Render reflection texture
        sceneShader.use();
        glUniform4f(clipPlaneLocation, 0, 1, 0, -WATER_LEVEL);
        fbos.bindReflectionFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render(reflectionMVP);

        // Render refraction texture
        sceneShader.use();
        glUniform4f(clipPlaneLocation, 0, -1, 0, WATER_LEVEL);
        fbos.bindRefractionFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render(mvp);
        fbos.unbindCurrentFrameBuffer();

        // Clear screen frame buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        sceneShader.use();
        glUniform4f(clipPlaneLocation, 0, -1, 0,
                    100000); // "disable" clipping plane
        scene.render(mvp);

        // Render water
        mvp = projection * camera->getWorldToViewMatrix() * scaledModelMatrix;
        waterShader.use();
        waterPlane.render(mvp);

        // Render GUI
        guiRenderer.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void updateCameraRotation(GLFWwindow *window)
{
    glfwGetCursorPos(window, &xPos, &yPos);

    yaw += (xPos - lastXPos) * SENSITIVITY;
    pitch += (lastYPos - yPos) * SENSITIVITY;
    pitch = clamp(pitch, 89.0f, -89.0f);

    lastXPos = xPos;
    lastYPos = yPos;

    camera->rotate(yaw, pitch);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
    if (action == GLFW_REPEAT)
        return;
    heldKeys[key] = action == GLFW_PRESS;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera controls
    updateCameraRotation(window);

    if (heldKeys[GLFW_KEY_SPACE])
        camera->moveUp(1.0f);
    if (heldKeys[GLFW_KEY_LEFT_CONTROL])
        camera->moveUp(-1.0f);
    if (heldKeys[GLFW_KEY_W])
        camera->moveForward(1.0f);
    if (heldKeys[GLFW_KEY_S])
        camera->moveForward(-1.0f);
    if (heldKeys[GLFW_KEY_D])
        camera->moveLeft(1.0f);
    if (heldKeys[GLFW_KEY_A])
        camera->moveLeft(-1.0f);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    SCREEN_W = (float)width;
    SCREEN_H = (float)height;
    projection = glm::perspective(
        glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);

    glViewport(0, 0, width, height);
}
