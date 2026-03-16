#pragma once

#include "../_afx.h"

// ---------------------------------------------------------------------------
// L2InputManager — ввод мыши и клавиатуры через GLFW.
//
// Оригинал использовал GetCursorPos / ScreenToClient / SetCursorPos /
// ShowCursor (Win32). Здесь всё заменено на GLFW API.
//
// Индексация _keyboard[]:
//   - Символьные клавиши 'A'–'Z' (65–90): GLFW_KEY_A == 65, совпадает с VK
//   - Специальные: VK_SPACE = GLFW_KEY_SPACE, VK_CONTROL = GLFW_KEY_LEFT_CONTROL
//     (определены в _afx.h)
//   - Размер массива 512 покрывает все GLFW_KEY_* коды
// ---------------------------------------------------------------------------
class L2InputManager
{
public:
    void Init();
    void update();

    _vector2 getMousePos()        { return _mousePos; }
    _vector2 getMousePosAbs()     { return _mousePosAbs; }
    _vector2 getMousePosPrev()    { return _mousePosPrev; }
    _vector2 getMousePosPrevAbs() { return _mousePosPrevAbs; }
    _vector2 getMouseMove()       { return _mouseMove; }
    int16    getMouseScroll()     { return _mouseScroll; }

    // key — GLFW_KEY_* код (или ASCII для A-Z)
    bool getKeyState(int key)
    {
        if (key < 0 || key >= 512) return false;
        return _keyboard[key];
    }

    void setMousePos(int x, int y);
    void setCursorVisible(bool visible);

    bool injectMouseDown(MyGUI::MouseButton btn);
    bool injectMouseUp(MyGUI::MouseButton btn);
    bool injectMouseScroll(int16 scroll);
    bool injectKeyDown(int key);
    bool injectKeyUp(int key);

public:
    L2InputManager();
    ~L2InputManager();

private:
    _vector2 _mousePos;
    _vector2 _mousePosAbs;
    _vector2 _mousePosPrev;
    _vector2 _mousePosPrevAbs;
    _vector2 _mouseMove;
    int16    _mouseScroll;

    bool _keyboard[512];
};
