#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh
{
public:
    Mesh(const std::string objFile);

    void render();

private:
    void fromObjFile(const std::string objFile);

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    GLuint vao;
    std::vector<GLuint> buffers;
};
