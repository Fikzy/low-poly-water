#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "stb_image.h"

GLuint loadTexture(const std::string &textureFile)
{
    // OpenGL textures are loaded bottom to top
    stbi_set_flip_vertically_on_load(true); // VERY IMPORTANT!

    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "Failed to load texture " << textureFile << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return texture;
}
