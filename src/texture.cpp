#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <algorithm>

Texture::Texture(const std::string& path) {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
}

bool Texture::saveImage(const std::string& path, int width, int height, const std::vector<unsigned char>& buffer) {
    // Determine file format based on extension
    std::string ext = path.substr(path.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    int success = 0;
    if (ext == "png") {
        success = stbi_write_png(path.c_str(), width, height, 4, buffer.data(), width * 4);
    } else if (ext == "jpg" || ext == "jpeg") {
        success = stbi_write_jpg(path.c_str(), width, height, 4, buffer.data(), 95); // Quality 95
    } else {
        // Default to PNG if extension is not recognized
        success = stbi_write_png(path.c_str(), width, height, 4, buffer.data(), width * 4);
    }

    return success != 0;
}

void Texture::bind(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}