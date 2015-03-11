#include "glfwUtils.hpp"

#include <GLFW/glfw3.h>

GLFWwindow* initGLFWwithWindow(int width, int heigh, const char* title) {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window and setup OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, heigh, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    return window;
}

void terminateGLFW() {
    glfwTerminate();
}
