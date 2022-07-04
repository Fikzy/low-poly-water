#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "shader.hh"

class Model
{
public:
    Model(const std::string objFile, Shader *shader);
    Model(const std::string objFile, Shader *shader,
          const std::string &textureFile);
    ~Model();

    void render(glm::mat4x4 &mvp);

private:
    void fromObjFile(const std::string objFile);

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    // std::vector<unsigned int> vertexIndices;

    std::vector<GLuint> buffers;
    std::vector<GLuint> textures;
    GLuint vao;

    Shader *shader;
};
