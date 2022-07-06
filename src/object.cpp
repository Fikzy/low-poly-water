#include "object.hh"

#include <glm/gtc/type_ptr.hpp>

const glm::vec3 X_AXIS = glm::vec3(1, 0, 0);
const glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
const glm::vec3 Z_AXIS = glm::vec3(0, 0, 1);

Object::Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader)
    : mesh(mesh)
    , shader(shader)
    , position(glm::vec3(0))
    , rotation(glm::vec3(0))
    , scale(glm::vec3(1))
{}

void Object::addTexture(std::string name, GLuint *texture)
{
    shaderTextures.emplace(name, texture);
}

void Object::removeTexture(std::string name)
{
    shaderTextures.erase(name);
}

void Object::render(const glm::mat4 &projection, const glm::mat4 &view)
{
    shader->use();

    auto model = glm::mat4(1); // identity matrix
    model = glm::scale(model, scale);
    // FIXME: rotation along more than one axis is broken
    model = glm::rotate(model, rotation.x, X_AXIS);
    model = glm::rotate(model, rotation.y, Y_AXIS);
    model = glm::rotate(model, rotation.z, Z_AXIS);
    model = glm::translate(model, position);

    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);

    int textureIdx = 0;
    for (auto p : shaderTextures)
    {
        shader->setInt(p.first, textureIdx);
        glActiveTexture(GL_TEXTURE0 + textureIdx);
        glBindTexture(GL_TEXTURE_2D, *p.second);
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
