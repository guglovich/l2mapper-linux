#include "L2UIMain.h"

#include "../OpenGLImageLoader_FreeImage.h"
#include "../main.h"

#include "L2UIEditor.h"
#include "L2UIMap.h"
#include "L2UIBusyScreen.h"

L2UIMain::L2UIMain(void)
{
    _isInitialized = false;
    mGUI = nullptr;
    mPlatform = nullptr;
    _L2Editor = nullptr;
    _L2Map = nullptr;
    _L2BusyScreen = nullptr;
}

L2UIMain::~L2UIMain(void)
{
}

void L2UIMain::Init()
{
    mPlatform = new MyGUI::OpenGL3Platform();

    // Регистрируем пути ДО initialise()
    MyGUI::xml::Document doc;
    if (doc.open(std::string("resources.xml")))
    {
        MyGUI::xml::ElementPtr root = doc.getRoot();
        if (root && root->getName() == "Paths")
        {
            MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
            while (node.next())
            {
                if (node->getName() == "Path")
                    mPlatform->getDataManagerPtr()->addResourceLocation(
                        node->getContent(), false);
            }
        }
    }
    else
    {
        mPlatform->getDataManagerPtr()->addResourceLocation(".", false);
        fprintf(stderr, "UI: resources.xml not found, using .\n");
    }

    mPlatform->initialise(new OpenGLImageLoader_FreeImage());

    mGUI = new MyGUI::Gui();
    mGUI->initialise();

    MyGUI::PointerManager::getInstance().setVisible(false);

    _L2Editor = new L2UIEditor();
    _L2Editor->Init();

    _L2Map = new L2UIMap();
    _L2Map->Init();

    _L2BusyScreen = new L2UIBusyScreen();
    _L2BusyScreen->Init();

    _isInitialized = true;
}

void L2UIMain::onResize(int width, int height)
{
    if (!_isInitialized) return;
    mPlatform->getRenderManagerPtr()->setViewSize(width, height);
}

void L2UIMain::update()
{
    if (!_isInitialized) return;
    if (_L2Map) _L2Map->update();
    mPlatform->getRenderManagerPtr()->drawOneFrame();
}

bool L2UIMain::injectMouseDown(int x, int y, MyGUI::MouseButton btn)
{
    if (!_isInitialized) return false;
    MyGUI::InputManager::getInstance().injectMousePress(x, y, btn);
    return MyGUI::InputManager::getInstance().isModalAny();
}

bool L2UIMain::injectMouseUp(int x, int y, MyGUI::MouseButton btn)
{
    if (!_isInitialized) return false;
    MyGUI::InputManager::getInstance().injectMouseRelease(x, y, btn);
    return MyGUI::InputManager::getInstance().isModalAny();
}

void L2UIMain::injectMouseMove(int x, int y)
{
    if (!_isInitialized) return;
    MyGUI::InputManager::getInstance().injectMouseMove(x, y, 0);
}
