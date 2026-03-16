#include "L2UIEditor.h"
#include "../main.h"
#include "L2UIMap.h"

static char* readShader(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "l2mapper: cannot open shader '%s'\n", path); return nullptr; }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, size, 1, f);
    buf[size] = 0;
    fclose(f);
    return buf;
}

L2UIEditor::L2UIEditor() : L2UIBaseWidget() {}
L2UIEditor::~L2UIEditor() {}

void L2UIEditor::Init()
{
    // Простые кнопки вместо MenuBar с popup — избегаем проблем с событиями
    MyGUI::Button* btnMap = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>(
        "Button", 0, 0, 60, 22,
        MyGUI::Align::Default, "Main", "BtnMap");
    btnMap->setCaption("Map");
    btnMap->eventMouseButtonPressed +=
        MyGUI::newDelegate(this, &L2UIEditor::onTopMenu_Main_ShowMap);

    MyGUI::Button* btnExit = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>(
        "Button", 62, 0, 60, 22,
        MyGUI::Align::Default, "Main", "BtnExit");
    btnExit->setCaption("Exit");
    btnExit->eventMouseButtonPressed +=
        MyGUI::newDelegate(this, &L2UIEditor::onExitClick);

    ui_topMenu = nullptr;

    ui_leftPanel = MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>(
        "ImageBox", 0, 22, 40, 300,
        MyGUI::Align::Default, "Main", "LeftPanel");
    ui_leftPanel->setAlign(MyGUI::Align::Left | MyGUI::Align::VStretch);

    MyGUI::Button* btn1 = ui_leftPanel->createWidget<MyGUI::Button>(
        "Button", 4, 6, 32, 32, MyGUI::Align::Default, "btn1");
    btn1->eventMouseButtonClick +=
        MyGUI::newDelegate(this, &L2UIEditor::onBtn1MouseClick);

    ui_sceneShowBsp = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>(
        "CheckBox", 45, 27, 150, 20,
        MyGUI::Align::Default, "Main", "ShowBsp");
    ui_sceneShowBsp->setCaption("Draw BSP");
    ui_sceneShowBsp->eventMouseButtonPressed +=
        MyGUI::newDelegate(this, &L2UIEditor::onShowBspMouseClick2);
}

void L2UIEditor::onMenuAccept(MyGUI::MenuControl* sender, MyGUI::MenuItem* item) {}

void L2UIEditor::update() {}
bool L2UIEditor::injectMouseDown(int x, int y, MyGUI::MouseButton btn) { return false; }
bool L2UIEditor::injectMouseUp(int x, int y, MyGUI::MouseButton btn)   { return false; }

void L2UIEditor::onTopMenu_Main_ShowMap(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn)
{
    g_ui.getL2Map()->setVisible(true);
}

void L2UIEditor::onExitClick(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn)
{
    glfwSetWindowShouldClose(g_window.getGLFWWindow(), GLFW_TRUE);
}

void L2UIEditor::onBtn1MouseClick(MyGUI::Widget* sender)
{
    char* vert = readShader("data/shaders/terrain.vert");
    char* frag = readShader("data/shaders/terrain.frag");
    if (vert && frag) g_shader.Add(vert, frag, "L2Terrain");
    delete[] vert; delete[] frag;

    vert = readShader("data/shaders/default.vert");
    frag = readShader("data/shaders/default.frag");
    if (vert && frag) g_shader.Add(vert, frag, "L2Default");
    delete[] vert; delete[] frag;
}

void L2UIEditor::onShowBspMouseClick2(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn)
{
    onShowBspMouseClick(sender);
}
void L2UIEditor::onShowBspMouseClick(MyGUI::Widget* sender)
{
    MyGUI::Button* btn = sender->castType<MyGUI::Button>();
    btn->setStateSelected(!btn->getStateSelected());
    if (g_levelMgr.getVars()) {
        g_levelMgr.getVars()->showBsp = btn->getStateSelected();
    }
}
