#include "gl_context.hpp"
#include <glad/glad.h>
#include <stdexcept>

GLContext::GLContext(int width, int height) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, "Image Processor", nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    // Initialize GLAD after creating OpenGL context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

GLContext::~GLContext() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GLContext::makeContextCurrent() {
    glfwMakeContextCurrent(m_window);
}

bool GLContext::shouldClose() {
    return glfwWindowShouldClose(m_window);
}

void GLContext::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void GLContext::pollEvents() {
    glfwPollEvents();
}