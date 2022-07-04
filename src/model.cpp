#include "model.hh"

#define TINYOBJLOADER_IMPLEMENTATION

#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "tiny_obj_loader.hh"

void Model::fromObjFile(const std::string objFile)
{
    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(objFile, reader_config))
    {
        std::cerr << "TinyObjReader: " << reader.Error();
        return;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexcoords;

    auto attrib = reader.GetAttrib();

    // Vertices
    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
        tempVertices.emplace_back(attrib.vertices[i + 0],
                                  attrib.vertices[i + 1],
                                  attrib.vertices[i + 2]);
    // Normals
    for (size_t i = 0; i < attrib.normals.size(); i += 3)
        tempNormals.emplace_back(attrib.normals[i + 0], attrib.normals[i + 1],
                                 attrib.normals[i + 2]);
    // UVs
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
        tempTexcoords.emplace_back(attrib.texcoords[i + 0],
                                   attrib.texcoords[i + 1]);

    auto shapes = reader.GetShapes();

    for (auto shape : shapes)
    {
        for (auto idx : shape.mesh.indices)
        {
            vertices.push_back(tempVertices[idx.vertex_index]);
            // vertexIndices.push_back(idx.vertex_index);
            normals.push_back(tempNormals[idx.normal_index]);
            texcoords.push_back(tempTexcoords[idx.texcoord_index]);
        }
    }
}

Model::Model(const std::string objFile, Shader *shader)
    : shader(shader)
{
    // Parse data
    fromObjFile(objFile);

    // Model VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // start recording bind calls

    // Buffers
    buffers.resize(3);
    glGenBuffers(buffers.size(), &buffers[0]);

    // - Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // - Normals
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // - Texture Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2),
                 &texcoords[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Model::~Model()
{
    // glDeleteBuffers(3, buffers);
    // glDeleteVertexArrays(1, &vao);
}

void Model::addTexture(std::string name, GLuint texture)
{
    shaderTextures.emplace(name, texture);
}

void Model::removeTexture(std::string name)
{
    shaderTextures.erase(name);
}

void Model::render(glm::mat4x4 &mvp)
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

    glBindVertexArray(vao); // enable

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindVertexArray(0); // disable

    textureIdx = 0;
    for (auto p : shaderTextures)
    {
        glActiveTexture(GL_TEXTURE0 + textureIdx);
        glBindTexture(GL_TEXTURE_2D, 0);
        textureIdx++;
    }
}
