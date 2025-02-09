#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    GLuint getId() const { return m_program; }

private:
    GLuint m_program;
    void checkCompileErrors(GLuint shader, const std::string& type);
};