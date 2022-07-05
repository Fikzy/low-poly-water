#include "object.hh"

#include <glm/gtc/type_ptr.hpp>

Object::Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader)
    : mesh(mesh)
    , shader(shader)
{}

void Object::addTexture(std::string name, GLuint texture)
{
    shaderTextures.emplace(name, texture);
}

void Object::removeTexture(std::string name)
{
    shaderTextures.erase(name);
}

void Object::render(glm::mat4x4 &mvp)
{
    shader->use();

    GLuint mvpLocation = glGetUniformLocation(shader->id, "MVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    int textureIdx = 0;
    for (auto p : shaderTextures)
    {
        shader->setInt(p.first, textureIdx);
        glActiveTexture(GL_TEXTURE0 + textureIdx);
        glBindTexture(GL_TEXTURE_2D, p.second);
        textureIdx++;
    }

    mesh->render();

    textureIdx = 0;
    for (auto p : shaderTextures)
    {
        glActiveTexture(GL_TEXTURE0 + textureIdx);
        glBindTexture(GL_TEXTURE_2D, 0);
        textureIdx++;
    }
}
