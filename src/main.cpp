#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <functional>
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <memory>

#include "camera.hh"
#include "gui_renderer.hh"
#include "mesh.hh"
#include "object.hh"
#include "shader.hh"
#include "texture.hh"
#include "water_frame_buffers.hh"

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void updateCameraRotation(GLFWwindow *window);
void processInput(GLFWwindow *window);

std::vector<std::function<void(int, int)>> resizeCallbacks;

float SCREEN_W = 1920.0f / 2;
float SCREEN_H = 1080.0f / 2;
float FOV = 65.0f;
float SENSITIVITY = 0.1f;

const float NEAR_CLIP = 0.1f;
const float FAR_CLIP = 1000.0f;

double lastXPos = SCREEN_W / 2, lastYPos = SCREEN_H / 2;
double yaw = 0, pitch = 0, xPos, yPos;

bool lockMouse = true;
bool wireframe = false;

std::shared_ptr<Camera> camera =
    std::make_shared<Camera>(glm::vec3(-8.0f, 2.0f, 0.0f));
glm::highp_mat4 projection = glm::perspective(
    glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);

std::map<int, bool> heldKeys;

const float WATER_LEVEL = 1.0;
const glm::vec4 REFLECTION_CLIP_PLANE = glm::vec4(0, 1, 0, -WATER_LEVEL);
const glm::vec4 REFRACTION_CLIP_PLANE = glm::vec4(0, -1, 0, WATER_LEVEL);
const glm::vec4 DISABLED_CLIP_PLANE = glm::vec4(0, -1, 0, 100000);

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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Shaders
    auto waterShader = std::make_shared<Shader>("water.vert", "water.frag",
                                                "water.tesc", "water.tese");
    auto sceneShader = std::make_shared<Shader>("scene.vert", "scene.frag");

    // Specific to water tesselation rendering
    glPatchParameteri(GL_PATCH_VERTICES, 3); // Triangles

    // Meshes
    auto waterMesh = std::make_shared<Mesh>("assets/quad.obj", GL_PATCHES);
    auto sceneMesh = std::make_shared<Mesh>("assets/lake_scene.obj");

    // Textures
    GLuint scenePalette = loadTexture("assets/scene_palette.png");

    // Water
    WaterFrameBuffers fbos = WaterFrameBuffers(SCREEN_W, SCREEN_H);
    resizeCallbacks.push_back(
        [&fbos](int w, int h) { fbos.setResolution(w, h); });

    auto water = std::make_shared<Object>(waterMesh, waterShader);
    water->addTexture("reflectionTexture", &fbos.reflectionTexture);
    water->addTexture("refractionTexture", &fbos.refractionTexture);

    water->scale *= glm::vec3(50, 1, 50);
    water->position.y += 1;

    // Scene
    auto scene = std::make_shared<Object>(sceneMesh, sceneShader);
    scene->addTexture("textureMap", &scenePalette);

    // GUI - Debug water textures
    auto guiRenderer = GuiRenderer();
    // guiRenderer.addElement(fbos.reflectionTexture, glm::vec2(-0.6, 0.5f),
    //                        glm::vec2(0.3, -0.3));
    // guiRenderer.addElement(fbos.refractionTexture, glm::vec2(0.6, 0.5f),
    //                        glm::vec2(0.3, -0.3));

    glClearColor(135.f / 255, 205.f / 255, 235.f / 255, 1.0f); // sky blue

    while (!glfwWindowShouldClose(window))
    {
        Camera::updateDeltaTime();
        processInput(window);
        glEnable(GL_CLIP_DISTANCE0);

        sceneShader->use();
        sceneShader->setVec3("lightDirection", glm::vec3(-1, -1, -1));
        sceneShader->setVec3("lightColor", glm::vec3(1));
        sceneShader->setVec3("lightAmbient", glm::vec3(0.3));

        waterShader->use();
        waterShader->setFloat("time", glfwGetTime());
        waterShader->setVec3("cameraPosition", camera->getPosition());

        // Move camera and under water
        auto dist = 2 * (camera->getPosition().y - WATER_LEVEL);
        camera->position.y -= dist;
        camera->rotate(yaw, -pitch);

        // Render reflection texture
        sceneShader->use();
        sceneShader->setVec4("plane", REFLECTION_CLIP_PLANE);
        fbos.bindReflectionFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->render(projection, camera->getWorldToViewMatrix());

        // Move camera back above water
        camera->position.y += dist;
        camera->rotate(yaw, pitch);

        // Render refraction texture
        sceneShader->use();
        sceneShader->setVec4("plane", REFRACTION_CLIP_PLANE);
        fbos.bindRefractionFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->render(projection, camera->getWorldToViewMatrix());
        fbos.unbindCurrentFrameBuffer();

        // Clear screen frame buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        sceneShader->use();
        sceneShader->setVec4("plane", DISABLED_CLIP_PLANE);
        scene->render(projection, camera->getWorldToViewMatrix());

        // Render water
        water->render(projection, camera->getWorldToViewMatrix());

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
    pitch = glm::clamp(pitch, -89.0, 89.0);

    lastXPos = xPos;
    lastYPos = yPos;

    camera->rotate(yaw, pitch);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods)
{
    if (action == GLFW_REPEAT)
        return;
    heldKeys[key] = action == GLFW_PRESS;

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        wireframe = !wireframe;
        std::cout << "WIREFRAME: " << (wireframe ? "ON" : "OFF") << std::endl;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        lockMouse = !lockMouse;
        std::cout << "LOCK MOUSE: " << (lockMouse ? "YES" : "NO") << std::endl;
        std::cout << "LOCK MOUSE: " << (lockMouse ? "YES" : "NO") << std::endl;
        glfwSetInputMode(window, GLFW_CURSOR,
                         lockMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
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

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    SCREEN_W = (float)width;
    SCREEN_H = (float)height;
    projection = glm::perspective(
        glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);

    glViewport(0, 0, width, height);

    for (auto cb : resizeCallbacks)
        cb(width, height);
}
