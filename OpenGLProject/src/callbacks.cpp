#include "../include/callbacks.h"
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback(int error, const char* description)
{
    std::cerr << "[ERROR IN APP " << error << "]: " << description << std::endl;
}