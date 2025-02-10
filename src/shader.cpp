#include "shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    // Add validation to ensure we have a valid OpenGL context
    if (!gladLoadGL) {
        throw std::runtime_error("GLAD not initialized before creating shader");
    }

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    if (vertex == 0) {
        throw std::runtime_error("Failed to create vertex shader");
    }

    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragment == 0) {
        glDeleteShader(vertex);
        throw std::runtime_error("Failed to create fragment shader");
    }

    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    m_program = glCreateProgram();
    if (m_program == 0) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        throw std::runtime_error("Failed to create shader program");
    }

    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    checkCompileErrors(m_program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(m_program);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                      << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                      << infoLog << std::endl;
        }
    }
}