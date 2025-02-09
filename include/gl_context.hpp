#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLContext {
public:
    GLContext(int width, int height);
    ~GLContext();
    void makeContextCurrent();
    bool shouldClose();
    void swapBuffers();
    void pollEvents();

private:
    GLFWwindow* m_window;
};