#include "EinInputManager.hpp"

EinInputManager::EinInputManager(EinWindow* window, bool keysStickyMode, bool mouseStickyMode, int mouseMode) {
    this->resetInputManager();
    this->window = window;
    glfwSetInputMode(window->getWindow(), GLFW_STICKY_KEYS, keysStickyMode);
    glfwSetInputMode(window->getWindow(), GLFW_STICKY_MOUSE_BUTTONS, mouseStickyMode);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, mouseMode);
    glfwSetWindowUserPointer(this->window->getWindow(), this);
    this->setupCallbacks();
}

void EinInputManager::setupCallbacks() {
    glfwSetKeyCallback(window->getWindow(), KeysCallback);
    glfwSetCursorPosCallback(window->getWindow(), CursorPositionCallback);
    glfwSetScrollCallback(window->getWindow(), ScrollCallback);
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

bool EinInputManager::isExit(KeyActionType actionType) {
    if(this->isKey(GLFW_KEY_ESCAPE, actionType))
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

double EinInputManager::getCursorPosX() {
    return cursorPosX;
}

double EinInputManager::getCursorPosY() {
    return cursorPosY;
}

bool EinInputManager::didCursorPositionChange() {
    return cursorPositionChanged;
}

void EinInputManager::CursorPositionEvent(double xpos, double ypos) {
    cursorPosX = xpos;
    cursorPosY = ypos;
    cursorPositionChanged = true;
}

double EinInputManager::getScrollOffsetX() {
    return scrollOffsetX;
}

double EinInputManager::getScrollOffsetY() {
    return scrollOffsetY;
}

void EinInputManager::ScrollEvent(double xscroll, double yscroll) {
    scrollOffsetX = xscroll;
    scrollOffsetY = yscroll;
}

void EinInputManager::pollEvents() {
    cursorPositionChanged = false;
    this->resetUpKeys();
    this->resetScrollOffsets();
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

void EinInputManager::resetScrollOffsets() {
    scrollOffsetX = 0;
    scrollOffsetY = 0;
}

void EinInputManager::resetInputManager() {
    this->resetUpKeys();
    this->resetDownKeys();
    this->resetScrollOffsets();
}
