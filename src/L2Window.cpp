#include "L2Window.h"
#include "main.h"

void L2Window::onResize()
{
    if (!_window) return;

    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    if (width == 0 || height == 0) return;

    g_camera.onResize(width, height);
    g_ui.onResize(width, height);
}

_vector2 L2Window::getInnerSize()
{
    if (!_window) return _vector2(0, 0);
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    return _vector2((float)width, (float)height);
}
