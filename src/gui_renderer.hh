#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "shader.hh"

const float QUAD_VERTICES[]{ -1, 1, -1, -1, 1, 1, 1, -1 };

class GuiRenderer
{
public:
    GuiRenderer();

    void addElement(GLuint texture, glm::vec2 position, glm::vec2 scale);

    void render();

private:
    GLuint quadVAO;
    Shader shader;

    std::vector<std::pair<GLuint, glm::mat4>> elements;
};
