#include "L2InputManager.h"
#include "../main.h"

L2InputManager::L2InputManager()
    : _mouseScroll(0)
{
    memset(_keyboard, 0, sizeof(_keyboard));
    _mousePos = _mousePosAbs = _mousePosPrev = _mousePosPrevAbs = _vector2(0, 0);
    _mouseMove = _vector2(0, 0);
}

L2InputManager::~L2InputManager()
{}

void L2InputManager::Init()
{
    // Массив уже обнулён в конструкторе
}

void L2InputManager::update()
{
    GLFWwindow* window = g_window.getGLFWWindow();
    if (!window) return;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    // «abs» — позиция в экранных координатах (через glfwGetWindowPos)
    int wx = 0, wy = 0;
    glfwGetWindowPos(window, &wx, &wy);

    _mousePos.x = static_cast<float>(mx);
    _mousePos.y = static_cast<float>(my);

    _mousePosAbs.x = static_cast<float>(wx + mx);
    _mousePosAbs.y = static_cast<float>(wy + my);

    _mouseMove     = _mousePos - _mousePosPrev;
    _mousePosPrev  = _mousePos;
    _mousePosPrevAbs = _mousePosAbs;

    // Сбрасываем скролл в каждом кадре (колбэк накапливает за кадр)
    _mouseScroll = 0;
}

void L2InputManager::setMousePos(int x, int y)
{
    GLFWwindow* window = g_window.getGLFWWindow();
    if (!window) return;

    // Переводим из экранных координат в координаты окна
    int wx = 0, wy = 0;
    glfwGetWindowPos(window, &wx, &wy);
    glfwSetCursorPos(window, x - wx, y - wy);
}

void L2InputManager::setCursorVisible(bool visible)
{
    GLFWwindow* window = g_window.getGLFWWindow();
    if (!window) return;
    glfwSetInputMode(window, GLFW_CURSOR,
        visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

bool L2InputManager::injectMouseDown(MyGUI::MouseButton btn)
{
    if (g_ui.injectMouseDown(_mousePos.x, _mousePos.y, btn))
        return true;
    if (g_camera.injectMouseDown(btn))
        return true;
    if (g_scene.injectMouseDown(btn))
        return true;
    return false;
}

bool L2InputManager::injectMouseUp(MyGUI::MouseButton btn)
{
    if (g_ui.injectMouseUp(_mousePos.x, _mousePos.y, btn))
        return true;
    if (g_camera.injectMouseUp(btn))
        return true;
    if (g_scene.injectMouseUp(btn))
        return true;
    return false;
}

bool L2InputManager::injectMouseScroll(int16 scroll)
{
    _mouseScroll = scroll;
    if (g_camera.injectMouseScroll(_mouseScroll))
        return true;
    return false;
}

bool L2InputManager::injectKeyDown(int key)
{
    if (key >= 0 && key < 512)
        _keyboard[key] = true;
    return false;
}

bool L2InputManager::injectKeyUp(int key)
{
    if (key >= 0 && key < 512)
        _keyboard[key] = false;
    return false;
}
