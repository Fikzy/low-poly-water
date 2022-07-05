#include "object.hh"

#include <glm/gtc/type_ptr.hpp>

const glm::vec3 X_AXIS = glm::vec3(1, 0, 0);
const glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
const glm::vec3 Z_AXIS = glm::vec3(0, 0, 1);

Object::Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader)
    : mesh(mesh)
    , shader(shader)
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

    auto matrix = glm::mat4(1); // identity matrix
    matrix = glm::scale(matrix, scale);
    matrix = glm::rotate(matrix, rotation.x, X_AXIS);
    matrix = glm::rotate(matrix, rotation.y, Y_AXIS);
    matrix = glm::rotate(matrix, rotation.z, Z_AXIS);
    matrix = glm::translate(matrix, position);

    auto MVP = projection * view * matrix;
    shader->setMat4("MVP", MVP);

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
