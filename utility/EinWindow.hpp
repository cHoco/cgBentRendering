#ifndef EIN_WINDOW_HPP
#define EIN_WINDOW_HPP

#include "Ein.hpp"

#include <string>

class EinWindow
{
public:
    EinWindow(int width, int height, std::string title, bool fullScreen);
    ~EinWindow();
    GLFWwindow* getWindow();

    void Show();
    void Hide();
    void Close();

    bool ShouldClose();
    void SetShouldClose(bool shouldClose);

    std::string GetTitle();
    void SetTitle(std::string windowTitle);

    void Iconify();
    void Restore();

    void SwapBuffers();

    EinInputManager* GetInputManager();
    void UpdateEvents();

private:
    GLFWwindow* window;

    std::string windowTitle;

    EinInputManager* inputManager;
};
#endif

