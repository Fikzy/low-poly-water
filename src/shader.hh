#pragma once

#include <fstream>
#include <glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Shader
{
public:
    Shader(const std::string vertexPath, const std::string fragmentPath);
    Shader(const std::string vertexPath, const std::string fragmentPath,
           const std::string tessContPath, const std::string tessEvalPath,
           const std::string geomPath);

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setVec4(const std::string &name, const glm::vec4 &vec) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    GLuint id;

protected:
    GLuint compileShader(const std::string shaderPath, const GLenum shaderType);
    void linkProgram(const std::vector<GLuint> shaders);
};
