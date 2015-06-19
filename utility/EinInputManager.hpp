#ifndef EIN_INPUT_MANAGER_HPP
#define EIN_INPUT_MANAGER_HPP

#include <string>
#include <iostream>

#include "Ein.hpp"

enum KeyActionType {
    KEY_UP,
    KEY_DOWN
};

class EinInputManager
{
public:
    EinInputManager(EinWindow* window, bool keysStickyMode, bool mouseStickyMode, int mouseMode);
    EinInputManager(EinWindow* window) : EinInputManager(window, false, false, GLFW_CURSOR_DISABLED){};

    bool isUp(KeyActionType actionType);
    bool isDown(KeyActionType actionType);
    bool isLeft(KeyActionType actionType);
    bool isRight(KeyActionType actionType);
    bool isExit(KeyActionType actionType);

    bool isKey(int key, KeyActionType actionType);

    bool didCursorPositionChange();
    double getCursorPosX();
    double getCursorPosY();

    double getScrollOffsetX();
    double getScrollOffsetY();

    void pollEvents();
private:
    EinWindow* window;
    void setupCallbacks();

    inline static void KeysCallback(
            GLFWwindow *win,
            int key,
            int scancode,
            int action,
            int mods) {
        EinInputManager *inputManager = static_cast<EinInputManager*>(glfwGetWindowUserPointer(win));
        inputManager->KeyEvent(key, scancode, action, mods);
    };
    void KeyEvent(int key, int scancode, int action, int mods);

    inline static void CursorPositionCallback(
            GLFWwindow* win,
            double xpos,
            double ypos) {
        EinInputManager *inputManager = static_cast<EinInputManager*>(glfwGetWindowUserPointer(win));
        inputManager->CursorPositionEvent(xpos, ypos);
    };
    void CursorPositionEvent(double xpos, double ypos);

    inline static void ScrollCallback(
            GLFWwindow *win,
            double xscroll,
            double yscroll) {
        EinInputManager *inputManager = static_cast<EinInputManager*>(glfwGetWindowUserPointer(win));
        inputManager->ScrollEvent(xscroll, yscroll);
    };
    void ScrollEvent(double xscroll, double yscroll);

    void resetInputManager();
    void resetDownKeys();
    void resetUpKeys();
    void resetScrollOffsets();
    bool downKeysCache[1024];
    bool upKeysCache[1024];
    double cursorPosX;
    double cursorPosY;
    bool cursorPositionChanged;
    double scrollOffsetX;
    double scrollOffsetY;
};
#endif
