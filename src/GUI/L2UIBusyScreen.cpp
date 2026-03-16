#include "L2UIBusyScreen.h"
#include "../main.h"

L2UIBusyScreen::L2UIBusyScreen(void)
    : L2UIBaseWidget()
{
    ui_busyScreen = nullptr;
    ui_busyScreen_msgWnd = nullptr;
    ui_busyScreen_msg = nullptr;
}

L2UIBusyScreen::~L2UIBusyScreen(void)
{
}

void L2UIBusyScreen::Init()
{
    // BusyScreen отложен — создаётся при первом setVisible(true)
}

void L2UIBusyScreen::_createWidgets()
{
    if (ui_busyScreen) return; // уже создан

    ui_busyScreen = MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>(
        "ImageBox", 0, 0, 800, 600,
        MyGUI::Align::Stretch, "Modal", "BusyScreen");

    ui_busyScreen_msg = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>(
        "EditClientSkin", 200, 260, 400, 80,
        MyGUI::Align::Default, "Modal", "BusyScreen_Msg");
    ui_busyScreen_msg->setTextAlign(MyGUI::Align::Center);
    ui_busyScreen_msg->setEditMultiLine(true);
    ui_busyScreen_msg->setEditWordWrap(true);
    ui_busyScreen_msg->setEditStatic(true);

    ui_busyScreen_msgWnd = nullptr;

    ui_busyScreen->setVisible(false);
    ui_busyScreen->setAlpha(0);
    ui_busyScreen_msg->setVisible(false);
}

void L2UIBusyScreen::update() {}

void L2UIBusyScreen::onResize(int width, int height)
{
    if (!ui_busyScreen) return;
    ui_busyScreen->setSize(width, height);
    if (ui_busyScreen_msg)
        ui_busyScreen_msg->setPosition(
            width / 2 - ui_busyScreen_msg->getWidth() / 2,
            height / 2 - ui_busyScreen_msg->getHeight() / 2);
}

bool L2UIBusyScreen::injectMouseDown(int x, int y, MyGUI::MouseButton btn) { return false; }
bool L2UIBusyScreen::injectMouseUp(int x, int y, MyGUI::MouseButton btn)   { return false; }

void L2UIBusyScreen::setMessage(const char* msg)
{
    _createWidgets();
    if (ui_busyScreen_msg)
        ui_busyScreen_msg->setCaption(msg);
}

void L2UIBusyScreen::setVisible(bool value)
{
    // Не создаём виджеты при hide если они ещё не созданы
    if (!value && !ui_busyScreen) return;
    _createWidgets();
    if (!ui_busyScreen) return;

    if (value) {
        MyGUI::LayerManager::getInstance().upLayerItem(ui_busyScreen);
        ui_busyScreen->setVisible(true);
        ui_busyScreen_msg->setVisible(true);
        MyGUI::InputManager::getInstance().addWidgetModal(ui_busyScreen);
    }

    MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance()
        .createItem(MyGUI::ControllerFadeAlpha::getClassTypeName());
    MyGUI::ControllerFadeAlpha* controller =
        item->castType<MyGUI::ControllerFadeAlpha>();
    controller->setCoef(2.0f);
    controller->setAlpha(value ? 1.0f : 0.0f);
    if (!value)
        controller->eventPostAction +=
            MyGUI::newDelegate(this, &L2UIBusyScreen::onFadeOutFinished);
    MyGUI::ControllerManager::getInstance().addItem(ui_busyScreen, controller);
}

void L2UIBusyScreen::onFadeOutFinished(MyGUI::Widget* sender)
{
    if (ui_busyScreen) {
        ui_busyScreen->setVisible(false);
        if (ui_busyScreen_msg) ui_busyScreen_msg->setVisible(false);
        MyGUI::InputManager::getInstance().removeWidgetModal(ui_busyScreen);
    }
}
