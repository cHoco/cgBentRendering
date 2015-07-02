#ifndef EIN_WINDOW_HPP
#define EIN_WINDOW_HPP

#include "Ein.hpp"

#include <string>

struct Size {
    int width;
    int height;
};

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

    Size GetWindowSize();
    Size GetFramebufferSize();

    void Iconify();
    void Restore();

    void SwapBuffers();

    EinInputManager* GetInputManager();
    void UpdateEvents();

private:
    GLFWwindow* window;

    std::string windowTitle;

    Size windowSize;
    Size framebufferSize;

    EinInputManager* inputManager;
};
#endif

