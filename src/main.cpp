#include "main.h"
#include "GUI/L2UIMap.h"

bool L2App::Init()
{
    g_window.create();
    g_input.Init();
    g_levelMgr.Init();
    g_skybox.Init();
    g_ui.Init();
    g_scene.Init();
    g_geo.Init();
    g_window.onResize();
    return true;
}

void L2App::MainLoop()
{
    g_time.update();
    g_input.update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    g_scene.preUpdate();
    g_camera.update();
    g_scene.update();
    g_ui.update();
}

int main(int argc, char** argv)
{
    if (!glfwInit()) {
        fprintf(stderr, "l2mapper: glfwInit() failed\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    int msaa = g_cfg.CFG_ANTIALIASING;
    if (msaa > 0) glfwWindowHint(GLFW_SAMPLES, msaa);

    GLFWwindow* window = glfwCreateWindow(
        g_cfg.CFG_SCREENRES_X, g_cfg.CFG_SCREENRES_Y,
        "L2 Map Viewer",
        g_cfg.CFG_FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr,
        nullptr);

    if (!window) {
        fprintf(stderr, "l2mapper: glfwCreateWindow() failed\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    g_window.setGLFWWindow(window);

    // Проверяем масштаб HiDPI
    int winW, winH, fbW, fbH;
    glfwGetWindowSize(window, &winW, &winH);
    glfwGetFramebufferSize(window, &fbW, &fbH);
    float scaleX = (float)fbW / winW;
    float scaleY = (float)fbH / winH;

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int winW, int winH) {
        int fbW, fbH;
        glfwGetFramebufferSize(w, &fbW, &fbH);
        g_camera.onResize(fbW, fbH);
        g_ui.onResize(fbW, fbH);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        // Конвертируем логические координаты в физические (framebuffer)
        int winW, winH, fbW, fbH;
        glfwGetWindowSize(w, &winW, &winH);
        glfwGetFramebufferSize(w, &fbW, &fbH);
        int px = (int)(x * fbW / winW);
        int py = (int)(y * fbH / winH);
        MyGUI::InputManager::getInstance().injectMouseMove(px, py, 0);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int) {
        double mx, my;
        glfwGetCursorPos(w, &mx, &my);

        // Конвертируем в физические координаты
        int winW, winH, fbW, fbH;
        glfwGetWindowSize(w, &winW, &winH);
        glfwGetFramebufferSize(w, &fbW, &fbH);
        int px = (int)(mx * fbW / winW);
        int py = (int)(my * fbH / winH);

        MyGUI::MouseButton mb;
        if      (button == GLFW_MOUSE_BUTTON_LEFT)  mb = MyGUI::MouseButton::Left;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) mb = MyGUI::MouseButton::Right;
        else return;

        if (action == GLFW_PRESS) {
            MyGUI::Widget* fw = MyGUI::InputManager::getInstance().getMouseFocusWidget();
            fprintf(stderr, "PRESS(%d,%d) focus=%s\n", px, py, fw ? fw->getName().c_str() : "NONE"); fflush(stderr);
            // Прямая передача клика в карту
            if (button == GLFW_MOUSE_BUTTON_LEFT)
                g_ui.getL2Map()->injectMouseDown(px, py, mb);

            MyGUI::InputManager::getInstance().injectMousePress(px, py, mb);
            MyGUI::Widget* fw2 = MyGUI::InputManager::getInstance().getKeyFocusWidget();
            fprintf(stderr, "  key_focus=%s\n", fw2 ? fw2->getName().c_str() : "NONE"); fflush(stderr);            if (button == GLFW_MOUSE_BUTTON_RIGHT)
                g_camera.injectMouseDown(mb);
        } else if (action == GLFW_RELEASE) {
            MyGUI::InputManager::getInstance().injectMouseRelease(px, py, mb);
            if (button == GLFW_MOUSE_BUTTON_LEFT)
                g_ui.getL2Map()->injectMouseUp(px, py, mb);
            if (button == GLFW_MOUSE_BUTTON_RIGHT)
                g_camera.injectMouseUp(mb);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow*, double, double yoff) {
        g_input.injectMouseScroll((int16)(yoff * 120));
        g_camera.injectMouseScroll((int16)(yoff * 120));
    });

    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int, int action, int) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            g_input.injectKeyDown(key);
        else if (action == GLFW_RELEASE)
            g_input.injectKeyUp(key);
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            if (g_ui.getL2Map() && g_ui.getL2Map()->isVisible())
                g_ui.getL2Map()->setVisible(false);
            else
                glfwSetWindowShouldClose(w, GLFW_TRUE);
        }
        if (key == GLFW_KEY_R && action == GLFW_PRESS)
            g_levelMgr.resetCamera();
        if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
            g_levelMgr.resetCamera();
    });

    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        fprintf(stderr, "l2mapper: glewInit() failed: %s\n", glewGetErrorString(glewErr));
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    if (!g_main.Init()) {
        fprintf(stderr, "l2mapper: Init() failed\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Устанавливаем размер MyGUI viewport после инициализации
    glfwGetFramebufferSize(window, &fbW, &fbH);
    g_ui.onResize(fbW, fbH);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        g_main.MainLoop();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
