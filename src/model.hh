#pragma once

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
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> UVs;
    std::vector<glm::vec3> normals;

    std::vector<GLuint> buffers;
    std::vector<GLuint> textures;
    GLuint vao;

    Shader *shader;
};
