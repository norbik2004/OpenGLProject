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
{ //   POSITIONS           COLORS                 TEXCOORDS   NORMALS (UP)
    -25.0f, 0.0f,  25.0f,   0.83f, 0.70f, 0.44f,     0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     25.0f, 0.0f,  25.0f,   0.83f, 0.70f, 0.44f,    50.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     25.0f, 0.0f, -25.0f,   0.83f, 0.70f, 0.44f,    50.0f, 50.0f,  0.0f, 1.0f, 0.0f,
    -25.0f, 0.0f, -25.0f,   0.83f, 0.70f, 0.44f,     0.0f, 50.0f,  0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
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

    glViewport(0, 0, mode->width, mode->height);

    //CALLBACKS
    glfwSetKeyCallback(window, key_callback);

    string shaderDir = filesystem::current_path().string() + "/src/shaders/";
    string texturteDir = filesystem::current_path().string() + "/src/textures/";

    DrawingHelper drawingHelper;
    Shader textureShader = drawingHelper.setupShaderProgram(shaderDir, "default");
    Shader colorShader = drawingHelper.setupShaderProgram(shaderDir, "color");
    Shader lightShader = drawingHelper.setupShaderProgram(shaderDir, "light");

    //LIGHT

    VAO VAO1;
    VAO1.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));
    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    // Generates Vertex Array Object and binds it
    VAO lightVAO;
    lightVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    // Generates Element Buffer Object and links it to indices
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    // Links VBO attributes such as coordinates and colors to VAO
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // Unbind all to prevent accidentally modifying them
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();



    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);


    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    textureShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(textureShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(textureShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

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
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handles camera inputs
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(45.0f, 0.1f, 100.0f);


        // Tells OpenGL which Shader Program we want to use
        textureShader.Activate();
        // Exports the camera Position to the Fragment Shader for specular lighting
        glUniform3f(glGetUniformLocation(textureShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        // Export the camMatrix to the Vertex Shader of the pyramid
        camera.Matrix(textureShader, "camMatrix");
        // Binds texture so that is appears in rendering
        linux.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);



        // Tells OpenGL which Shader Program we want to use
        lightShader.Activate();
        // Export the camMatrix to the Vertex Shader of the light cube
        camera.Matrix(lightShader, "camMatrix");
        // Bind the VAO so OpenGL knows to use it
        lightVAO.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }


    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    linux.Delete();
    textureShader.Delete();
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    lightShader.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}