#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <string>
#include <iostream>

#include <GLFW/glfw3.h>

class InputManager
{
public:
    enum KeyActionType {
        KEY_UP,
        KEY_DOWN
    };

    bool isUp(KeyActionType actionType);
    bool isDown(KeyActionType actionType);
    bool isLeft(KeyActionType actionType);
    bool isRight(KeyActionType actionType);

    bool isKey(int key, KeyActionType actionType);

    void keysCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void poolEvents();
private:
    void resetDownKeys();
    void resetUpKeys();
    bool downKeysCache[1024];
    bool upKeysCache[1024];
};
#endif
