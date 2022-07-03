#include "model.hh"

#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "obj_loader.hh"
#include "stb_image.h"

Model::Model(const std::string objFile, Shader *shader)
    : shader(shader)
{
    if (!loadObj(objFile, vertices, UVs, normals))
        return;

    buffers.reserve(2);

    // Model VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // start recording bind calls

    // Buffers
    glGenBuffers(2, &buffers[0]);

    // - Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // - Normals
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindVertexArray(0); // stop recording bind calls
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Model::Model(const std::string objFile, Shader *shader,
             const std::string &textureFile)
    : shader(shader)
{
    if (!loadObj(objFile, vertices, UVs, normals))
        return;

    // Model VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // start recording bind calls

    // Buffers
    buffers.reserve(4);
    glGenBuffers(4, &buffers[0]);

    // - Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // - Normals
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // - UVs
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Texture
    textures.reserve(1);
    glGenTextures(1, &textures[0]);

    // activate the texture unit before binding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));

    glBindVertexArray(0); // stop recording bind calls
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Model::~Model()
{
    // glDeleteBuffers(3, buffers);
    // glDeleteVertexArrays(1, &vao);
}

void Model::render(glm::mat4x4 &mvp)
{
    GLuint mvpLocation = glGetUniformLocation(shader->id, "MVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(vao); // enable

    shader->use();

    if (textures.size() > 0)
        glBindTexture(GL_TEXTURE_2D, textures[0]);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindVertexArray(0); // disable
}
