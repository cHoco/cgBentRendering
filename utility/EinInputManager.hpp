#ifndef EIN_INPUT_MANAGER_HPP
#define EIN_INPUT_MANAGER_HPP

#include <string>
#include <iostream>

#include "Ein.hpp"

class EinInputManager
{
public:
    enum KeyActionType {
        KEY_UP,
        KEY_DOWN
    };

    EinInputManager(EinWindow* window, bool keysStickyMode, bool mouseStickyMode, int mouseMode);
    EinInputManager(EinWindow* window) : EinInputManager(window, false, false, GLFW_CURSOR_NORMAL){};

    bool isUp(KeyActionType actionType);
    bool isDown(KeyActionType actionType);
    bool isLeft(KeyActionType actionType);
    bool isRight(KeyActionType actionType);

    bool isKey(int key, KeyActionType actionType);

    void poolEvents();
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
    void resetInputManager();
    void resetDownKeys();
    void resetUpKeys();
    bool downKeysCache[1024];
    bool upKeysCache[1024];
};
#endif
