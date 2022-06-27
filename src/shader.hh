#pragma once

#include <fstream>
#include <glad.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
public:
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    unsigned int id;
};