#include "EinWindow.hpp"

EinWindow::EinWindow(int width, int height, std::string title, bool fullScreen) {
    GLFWmonitor *monitor = (fullScreen? glfwGetPrimaryMonitor() : nullptr);
    // int count = 0;
    // const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);
    // for(int i=0; i<=count; i++) {
    //     std::cout << "videomode" << std::endl;
    // }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window and setup OpenGL context
    window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    inputManager = new EinInputManager(this);
}

EinWindow::~EinWindow() {
    Close();
    delete inputManager;
}

GLFWwindow* EinWindow::getWindow() {
    return window;
}

void EinWindow::Show() {
    glfwShowWindow(window);
}

void EinWindow::Hide() {
    glfwHideWindow(window);
}

void EinWindow::Close() {
    glfwDestroyWindow(window);
}

bool EinWindow::ShouldClose() {
    return (bool)glfwWindowShouldClose(window);
}

void EinWindow::SetShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(window, (int)shouldClose);
}

std::string EinWindow::GetTitle() {
    return windowTitle;
}

void EinWindow::SetTitle(std::string windowTitle) {
    this->windowTitle = windowTitle;
    glfwSetWindowTitle(window, this->windowTitle.c_str());
}

void EinWindow::Iconify() {
    glfwIconifyWindow(window);
}

void EinWindow::Restore() {
    glfwRestoreWindow(window);
}

void EinWindow::SwapBuffers() {
    glfwSwapBuffers(window);
}

EinInputManager* EinWindow::GetInputManager() {
    return inputManager;
}
