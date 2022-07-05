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

    void addTexture(std::string name, GLuint texture);
    void removeTexture(std::string name);

    void render(glm::mat4x4 &mvp);

private:
    std::shared_ptr<Mesh> mesh;

    std::shared_ptr<Shader> shader;
    std::map<std::string, GLuint> shaderTextures;
};
