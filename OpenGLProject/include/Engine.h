#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "callbacks.h"

class Engine
{
public:
    Engine();
    ~Engine();

    bool initWindow(GLFWmonitor* selectedMonitor, const std::string& title);

    void showSplash(const std::string& imagePath);

    GLFWwindow* getWindow() const { return window; }
    int getSreenWidth() const { return screenWidth; }
    int getSreenHeight() const { return screenHeight; }

private:
    GLFWwindow* window = nullptr;
    GLFWmonitor* monitor = nullptr;
    int screenWidth = 1920;
    int screenHeight = 1080;
};
