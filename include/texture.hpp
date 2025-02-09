#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
public:
    Texture(const std::string& path);
    void bind(GLuint unit = 0);
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    GLuint m_texture;
    int m_width, m_height, m_channels;
};