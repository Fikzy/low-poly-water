#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "mesh.hh"
#include "shader.hh"

class Object
{
public:
    Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader);

    void addTexture(std::string name, GLuint *texture);
    void removeTexture(std::string name);

    void render(const glm::mat4 &projection, const glm::mat4 &view);

public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
    std::map<std::string, GLuint *> shaderTextures;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};
