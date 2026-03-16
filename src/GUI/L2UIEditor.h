#pragma once
#include "L2UIBaseWidget.h"

class L2UIEditor : public L2UIBaseWidget
{
public:
    virtual void Init();
    virtual void update();
    virtual bool injectMouseDown(int x, int y, MyGUI::MouseButton btn);
    virtual bool injectMouseUp(int x, int y, MyGUI::MouseButton btn);

    void onMenuAccept(MyGUI::MenuControl* sender, MyGUI::MenuItem* item);
    void onTopMenu_Main_ShowMap(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn);
    void onExitClick(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn);
    void onBtn1MouseClick(MyGUI::Widget* sender);
    void onShowBspMouseClick(MyGUI::Widget* sender);
    void onShowBspMouseClick2(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn);

protected:
    MyGUI::MenuBar*  ui_topMenu;
    MyGUI::ImageBox* ui_leftPanel;
    MyGUI::Button*   ui_sceneShowBsp;

public:
    L2UIEditor();
    virtual ~L2UIEditor();
};
