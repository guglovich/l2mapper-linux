#pragma once

#include "../_afx.h"
#include "L2UIBaseWidget.h"

class L2UIEditor;
class L2UIMap;
class L2UIBusyScreen;

class L2UIMain
{
public:
    void Init();
    void update();
    void onResize(int width, int height);

    L2UIEditor*     getL2Editor()      { return _L2Editor; }
    L2UIMap*        getL2Map()         { return _L2Map; }
    L2UIBusyScreen* getL2BusyScreen()  { return _L2BusyScreen; }

    bool injectMouseDown(int x, int y, MyGUI::MouseButton btn);
    bool injectMouseUp(int x, int y, MyGUI::MouseButton btn);
    void injectMouseMove(int x, int y);

protected:
    bool _isInitialized;

    MyGUI::Gui*             mGUI;
    MyGUI::OpenGL3Platform* mPlatform;   // OpenGL3 вместо OpenGLPlatform

    jfArray<L2UIBaseWidget*, uint16> _widgets;

    L2UIEditor*     _L2Editor;
    L2UIMap*        _L2Map;
    L2UIBusyScreen* _L2BusyScreen;

public:
    L2UIMain(void);
    ~L2UIMain(void);
};
