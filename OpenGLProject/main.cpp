#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/callbacks.h"
#include <iostream>
#include <filesystem>

#include "include/shaderClass.h"
#include "include/VAO.h"
#include "include/VBO.h"
#include "include/EBO.h"
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/Texture.h"
#include "include/Camera.h"
#include "include/DrawingHelper.h"


using namespace std;

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

GLfloat floorVertices[] =
{
    //   X      Y     Z     R     G     B
    -25.0f, 0.0f,  25.0f,  0.4f, 0.6f, 0.4f,
    -25.0f, 0.0f, -25.0f,  0.4f, 0.6f, 0.4f,
     25.0f, 0.0f, -25.0f,  0.4f, 0.6f, 0.4f,
     25.0f, 0.0f,  25.0f,  0.4f, 0.6f, 0.4f
};

GLuint floorIndices[] =
{
    0, 1, 2,
    0, 2, 3
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //CALLBACKS
    glfwSetKeyCallback(window, key_callback);

    string shaderDir = filesystem::current_path().string() + "/src/shaders/";
    string texturteDir = filesystem::current_path().string() + "/src/textures/";

    DrawingHelper drawingHelper;
    Shader textureShader = drawingHelper.setupShaderProgram(shaderDir, "default");
    Shader colorShader = drawingHelper.setupShaderProgram(shaderDir, "color");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    //Pod³oga

    VAO floorVAO;
    floorVAO.Bind();

    VBO floorVBO(floorVertices, sizeof(floorVertices));
    EBO floorEBO(floorIndices, sizeof(floorIndices));

    // aPos = location 0 (3 floaty), aColor = location 1 (3 floaty)
    floorVAO.LinkAttrib(floorVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    floorVAO.LinkAttrib(floorVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    floorVAO.Unbind();
    floorVBO.Unbind();
    floorEBO.Unbind();


    //texture

    std::string fullPath = texturteDir + "penguin.png";
    std::cout << "laduje teksture " << fullPath << std::endl;

    Texture linux((texturteDir + "penguin.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    linux.texUnit(textureShader, "tex0", 0);


    glEnable(GL_DEPTH_TEST);

    Camera camera(mode->width, mode->height, glm::vec3(0.0f, 1.0f, 2.0f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);

        // --- PIRAMIDA ---
        textureShader.Activate();
        camera.Matrix(45.0f, 0.1f, 100.0f, textureShader, "camMatrix");
        linux.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // --- POD£OGA ---
        colorShader.Activate();
        camera.Matrix(45.0f, 0.1f, 100.0f, colorShader, "camMatrix");
        floorVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    linux.Delete();
    textureShader.Delete();

    floorVAO.Delete();
    floorVBO.Delete();
    floorEBO.Delete();
    colorShader.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}