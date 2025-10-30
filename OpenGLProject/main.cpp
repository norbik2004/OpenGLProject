#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/callbacks.h"
#include <iostream>
#include <filesystem>

#include "include/shaderClass.h"
#include "include/VAO.h"
#include "include/VBO.h"
#include "include/EBO.h"


using namespace std;

GLfloat vertices[] =
{
    // coordinates   / colors //
    -0.5,  0.5, 0,   1, 1, 0.5,
    -0.5, -0.5, 0,   1, 0.5, 0.5,
     0.5, -0.5, 0,   1, 1, 0.5,
     0.5,  0.5, 0,   0.5, 1, 0.5

};

GLuint indices[] =
{
    0,1,2,
    0,2,3
};

int main(void)
{
    GLFWwindow* window;
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // SET THE ERROR CALLBACK
    glfwSetErrorCallback(error_callback);

    //Setup the monitor
    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);

    // IF window is not created, terminate
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //CALLBACKS
    glfwSetKeyCallback(window, key_callback);

    string shaderDir = filesystem::current_path().string() + "/src/shaders/";

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram(
        (shaderDir + "default.vert").c_str(),
        (shaderDir + "default.frag").c_str()
    );

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }


    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}