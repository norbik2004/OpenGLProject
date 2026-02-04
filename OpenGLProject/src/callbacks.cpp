#include "../include/callbacks.h"
#include "../include/InfectionController.h"
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    auto* infection = static_cast<InfectionController*>(
        glfwGetWindowUserPointer(window)
        );

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    case GLFW_KEY_P:
        if (infection)
            infection->togglePause();
        break;

    case GLFW_KEY_UP:
        if (infection)
            infection->speedUp(50.0f);
        break;

    case GLFW_KEY_DOWN:
        if (infection)
            infection->speedDown(50.0f);
        break;
    case GLFW_KEY_RIGHT:
        if (infection)
            infection->fasterUpdate(0.1f);
        break;

    case GLFW_KEY_LEFT:
        if (infection)
            infection->slowerUpdate(0.1f);
        break;

    case GLFW_KEY_R:
        if (infection)
            infection->reset();
        break;
    }
}
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    auto* infection = static_cast<InfectionController*>(glfwGetWindowUserPointer(window));
    if (!infection || infection->paused)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (!infection->greenActive)
            infection->startGreen();
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (!infection->blueActive)
            infection->startBlue();
    }
}