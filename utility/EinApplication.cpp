#include "EinApplication.hpp"

#include <stdexcept>

void EinApplication::setErrorCallback(void (*errorCallback)(int, const char *)) {
    glfwSetErrorCallback(errorCallback);
}

EinApplication::EinApplication() {
    if(!glfwInit())
    {
        throw std::runtime_error("[APPLICATION] GLFW initialization failed");
    }
}

EinApplication::~EinApplication() {
    Terminate();
}

double EinApplication::GetTime() {
    return glfwGetTime();
}

void EinApplication::Terminate() {
    glfwTerminate();
}
