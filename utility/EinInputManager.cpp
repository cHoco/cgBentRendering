#include "EinInputManager.hpp"

EinInputManager::EinInputManager(EinWindow* window, bool keysStickyMode, bool mouseStickyMode, int mouseMode) {
    this->resetInputManager();
    this->window = window;
    glfwSetWindowUserPointer(this->window->getWindow(), this);
    this->setupCallbacks();
}

void EinInputManager::setupCallbacks() {
    glfwSetKeyCallback(window->getWindow(), KeysCallback);
}

bool EinInputManager::isUp(KeyActionType actionType) {
    if(this->isKey(GLFW_KEY_W, actionType) ||
            this->isKey(GLFW_KEY_K, actionType) ||
            this->isKey(GLFW_KEY_UP, actionType))
        return true;
    else
        return false;
}

bool EinInputManager::isDown(KeyActionType actionType) {
    if(this->isKey(GLFW_KEY_S, actionType) ||
            this->isKey(GLFW_KEY_J, actionType) ||
            this->isKey(GLFW_KEY_DOWN, actionType))
        return true;
    else
        return false;
}

bool EinInputManager::isLeft(KeyActionType actionType) {
    if(this->isKey(GLFW_KEY_A, actionType) ||
            this->isKey(GLFW_KEY_H, actionType) ||
            this->isKey(GLFW_KEY_LEFT, actionType))
        return true;
    else
        return false;
}

bool EinInputManager::isRight(KeyActionType actionType) {
    if(this->isKey(GLFW_KEY_D, actionType) ||
            this->isKey(GLFW_KEY_L, actionType) ||
            this->isKey(GLFW_KEY_RIGHT, actionType))
        return true;
    else
        return false;
}

bool EinInputManager::isKey(int key, KeyActionType actionType) {
    if(downKeysCache[key] && actionType==KEY_DOWN) {
        return true;
    }
    else if(upKeysCache[key] && actionType==KEY_UP) {
        return true;
    }
    else
        return false;
}

void EinInputManager::KeyEvent(int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        downKeysCache[key] = true;
    }
    else if(action == GLFW_RELEASE) {
        upKeysCache[key] = true;
        downKeysCache[key] = false;
    }
}

void EinInputManager::poolEvents() {
    this->resetUpKeys();
    glfwPollEvents();
}

void EinInputManager::resetDownKeys() {
    for(int i=0; i<1024; i++)
        downKeysCache[i] = false;
}

void EinInputManager::resetUpKeys() {
    for(int i=0; i<1024; i++)
        upKeysCache[i] = false;
}

void EinInputManager::resetInputManager() {
    this->resetUpKeys();
    this->resetDownKeys();
}
