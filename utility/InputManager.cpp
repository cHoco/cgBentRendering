#include "InputManager.hpp"

bool InputManager::isUp(KeyActionType actionType) {

}

bool InputManager::isDown(KeyActionType actionType) {

}

bool InputManager::isLeft(KeyActionType actionType) {

}

bool InputManager::isRight(KeyActionType actionType) {

}

bool InputManager::isKey(int key, KeyActionType actionType) {
}

void InputManager::keysCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        downKeysCache[key] = true;
    }
    else if(action == GLFW_RELEASE) {
        upKeysCache[key] = true;
        downKeysCache[key] = false;
    }
}

void InputManager::poolEvents() {
    this->resetUpKeys();
    glfwPollEvents();
}

void InputManager::resetDownKeys() {
    for(int i=0; i<1024; i++)
        downKeysCache[i] = false;
}

void InputManager::resetUpKeys() {
    for(int i=0; i<1024; i++)
        upKeysCache[i] = false;
}

