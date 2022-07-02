#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>

#include "camera.hh"
#include "obj_loader.hh"
#include "shader.hh"
#include "stb_image.h"
#include "utils.hh"

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

    // Shader
    Shader cubeShader("cube.vert", "cube.frag");

    // Model VAO
    GLuint modelVAO;
    glGenVertexArrays(1, &modelVAO);
    glBindVertexArray(modelVAO); // start recording bindings

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> UVs;
    std::vector<glm::vec3> normals;
    loadObj("assets/tree2.obj", vertices, UVs, normals);

    std::cout << "vertices: " << vertices.size() << " | UVs: " << UVs.size()
              << " | normals: " << normals.size() << std::endl;

    // Vertices
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Normals
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // UVs
    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Texture
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(
        GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    unsigned char *data =
        stbi_load("assets/tree_palette.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));

    glBindVertexArray(0); // stop recording
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0, 0.1f, 0.2f, 0.8f);
    while (!glfwWindowShouldClose(window))
    {
        Camera::updateDeltaTime();

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Projection
        auto modelMatrix = glm::mat4(1.0);
        auto scaledModelMatrix = glm::scale(modelMatrix, glm::vec3(1));

        auto viewMatrix = camera->getWorldToViewMatrix();
        auto mvp = projection * viewMatrix * scaledModelMatrix;

        GLuint mvpLocation = glGetUniformLocation(cubeShader.id, "MVP");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        // Render model
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(modelVAO); // enable
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        cubeShader.use();
        glBindVertexArray(0); // disable

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &modelVAO);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);

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

    camera->rotate((float)yaw, (float)pitch);
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
