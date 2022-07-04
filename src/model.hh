#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

#include "shader.hh"
#include "texture.hh"

class Model
{
public:
    Model(const std::string objFile, Shader *shader);
    ~Model();

    void addTexture(std::string name, GLuint texture);
    void removeTexture(std::string name);

    void render(glm::mat4x4 &mvp);

private:
    void fromObjFile(const std::string objFile);

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    GLuint vao;
    std::vector<GLuint> buffers;
    std::map<std::string, GLuint> shaderTextures;

    Shader *shader;
};
