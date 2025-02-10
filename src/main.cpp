#include <filesystem>
#include <iostream>
#include <vector>
#include "shader.hpp"
#include "texture.hpp"
#include "gl_context.hpp"

namespace fs = std::filesystem;

const char* vertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    out vec2 TexCoord;
    
    void main() {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 450 core
    in vec2 TexCoord;
    out vec4 FragColor;
    uniform sampler2D inputTexture;
    uniform float intensity;
    
    void main() {
        vec4 color = texture(inputTexture, TexCoord);
        // Example effect: increase brightness
        FragColor = vec4(color.rgb * intensity, color.a);
    }
)";

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <input_folder> <output_folder>\n";
        return 1;
    }

    // Create context
    GLContext context(800, 600);
    context.makeContextCurrent();

    // Create shader
    Shader shader(vertexShaderSource, fragmentShaderSource);

    // Setup vertex data for full-screen quad
    float vertices[] = {
        // positions        // texture coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // Setup VAO/VBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Create framebuffer for rendering
    GLuint FBO;
    glGenFramebuffers(1, &FBO);

    // Process images
    fs::path inputPath(argv[1]);
    fs::path outputPath(argv[2]);
    fs::create_directories(outputPath);

    for (const auto& entry : fs::directory_iterator(inputPath)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            std::cout << "Processing: " << entry.path() << std::endl;

            // Load texture
            Texture inputTexture(entry.path().string());
            
            // Setup framebuffer
            GLuint outputTexture;
            glGenTextures(1, &outputTexture);
            glBindTexture(GL_TEXTURE_2D, outputTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, inputTexture.getWidth(), inputTexture.getHeight(), 
                        0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);

            // Render
            glViewport(0, 0, inputTexture.getWidth(), inputTexture.getHeight());
            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();
            shader.setFloat("intensity", 1.5f); // Example effect parameter
            inputTexture.bind(0);
            shader.setInt("inputTexture", 0);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Save processed image
            std::vector<unsigned char> buffer(inputTexture.getWidth() * inputTexture.getHeight() * 4);
            glReadPixels(0, 0, inputTexture.getWidth(), inputTexture.getHeight(), 
                        GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

            fs::path outputFile = outputPath / entry.path().filename();
            // Save processed image
            if (!Texture::saveImage(outputFile.string(), inputTexture.getWidth(), inputTexture.getHeight(), buffer)) {
                std::cerr << "Failed to save image: " << outputFile << std::endl;
            }
            
            glDeleteTextures(1, &outputTexture);
        }
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteFramebuffers(1, &FBO);

    return 0;
}