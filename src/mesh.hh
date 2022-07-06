#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh
{
public:
    Mesh(const std::string objFile, const GLenum draWmode);
    Mesh(const std::string objFile);

    virtual void render();

protected:
    Mesh(){};

    void loadMeshData(const std::string objFile);

protected:
    const GLenum draWmode = GL_TRIANGLES;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    GLuint vao;
    std::vector<GLuint> buffers;
};

class Quad : public Mesh
{
public:
    Quad();

    void render() override;

private:
    std::vector<GLuint> indices;
};
