#include "gui_renderer.hh"

#include <glm/gtc/type_ptr.hpp>

GuiRenderer::GuiRenderer()
    : shader("gui.vert", "gui.frag")
{
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    GLuint quadPositionsBuffer;
    glGenBuffers(1, &quadPositionsBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, quadPositionsBuffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), QUAD_VERTICES,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindVertexArray(0);
}

void GuiRenderer::addElement(GLuint texture, glm::vec2 position,
                             glm::vec2 scale)
{
    auto transform = glm::mat4(1.0);
    transform = glm::translate(transform, glm::vec3(position.x, position.y, 0));
    transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1));

    elements.emplace_back(texture, transform);
}

void GuiRenderer::render()
{
    shader.use();

    glBindVertexArray(quadVAO);

    for (auto elm : elements)
    {
        GLuint mvpLocation = glGetUniformLocation(shader.id, "MVP");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE,
                           glm::value_ptr(elm.second));

        glBindTexture(GL_TEXTURE_2D, elm.first);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
    }

    glBindVertexArray(0);
}
