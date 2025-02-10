#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>

class Texture {
public:
    Texture(const std::string& path);
    void bind(GLuint unit = 0);
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    static bool saveImage(const std::string& path, int width, int height, const std::vector<unsigned char>& buffer);

private:
    GLuint m_texture;
    int m_width, m_height, m_channels;
};