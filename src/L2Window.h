#pragma once

#include "_afx.h"

// ---------------------------------------------------------------------------
// L2Window — обёртка над GLFWwindow.
//
// Вся платформо-специфичная логика (создание контекста, пиксельный формат,
// MSAA) перенесена в main.cpp / GLFW. Этот класс хранит указатель на окно
// и предоставляет интерфейс, который ожидает остальной код.
// ---------------------------------------------------------------------------
class L2Window
{
public:
    // Вызывается из main() после glfwCreateWindow
    void setGLFWWindow(GLFWwindow* window) { _window = window; }

    // Создание окна — legacy-вызов из main::Init().
    // Реальное создание окна происходит в main() до Init(),
    // поэтому здесь ничего не делаем.
    bool create() { return _window != nullptr; }

    void onResize();

    GLFWwindow* getGLFWWindow() const { return _window; }

    bool isInitialized() const  { return _isInitialized; }
    void setIsInitialized(bool v) { _isInitialized = v; }
    bool isActive() const        { return _isActive; }
    void setIsActive(bool v)     { _isActive = v; }

    // Размер клиентской области окна
    _vector2 getInnerSize();

public:
    L2Window() : _window(nullptr), _isInitialized(false), _isActive(true) {}
    ~L2Window() {}

private:
    GLFWwindow* _window;
    bool _isInitialized;
    bool _isActive;
};
