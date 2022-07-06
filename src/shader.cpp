#include "shader.hh"

#include <glm/gtc/type_ptr.hpp>

#include "utils.hh"

Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
{
    linkProgram({
        compileShader(vertexPath, GL_VERTEX_SHADER),
        compileShader(fragmentPath, GL_FRAGMENT_SHADER),
    });
}

Shader::Shader(const std::string vertexPath, const std::string fragmentPath,
               const std::string tessContPath, const std::string tessEvalPath,
               const std::string geomPath)
{
    linkProgram({
        compileShader(vertexPath, GL_VERTEX_SHADER),
        compileShader(tessContPath, GL_TESS_CONTROL_SHADER),
        compileShader(tessEvalPath, GL_TESS_EVALUATION_SHADER),
        compileShader(fragmentPath, GL_FRAGMENT_SHADER),
        compileShader(geomPath, GL_GEOMETRY_SHADER),
    });
}

GLuint Shader::compileShader(const std::string shaderPath,
                             const GLenum shaderType)
{
    std::string shaderCode = readText(shaderPath);
    const char *code = shaderCode.c_str();

    // Shader compilation
    int success;
    char infoLog[512];

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << shaderPath
                  << std::endl;
        std::cout << infoLog << std::endl;
    };

    return shader;
}

void Shader::linkProgram(const std::vector<GLuint> shaders)
{
    int success;
    char infoLog[512];

    // Shader program
    id = glCreateProgram();

    for (auto s : shaders)
        glAttachShader(id, s);

    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    for (auto s : shaders)
        glDeleteShader(s);
}

void Shader::use()
{
    glUseProgram(id);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1,
                 glm::value_ptr(vec));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1,
                 glm::value_ptr(vec));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}
