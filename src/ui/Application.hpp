#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Application {
public:
    Application();
    ~Application();

    bool initialize();
    void run();

private:
    GLFWwindow* window;
    const int WINDOW_WIDTH = 450;
    const int WINDOW_HEIGHT = 650;
    const char* WINDOW_TITLE = "Professional Calculator";
};
