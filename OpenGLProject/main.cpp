#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/callbacks.h"
#include <iostream>
#include <filesystem>

#include "include/shaderClass.h"
#include "include/Mesh.h"
#include "include/Scene.h"
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/Texture.h"
#include "include/Camera.h"
#include "include/DrawingHelper.h"


using namespace std;

Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

Vertex floatingVertices[] =
{
    Vertex{glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0,1,0), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0,1,0), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0,1,0), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(0.5f, 0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0,1,0), glm::vec2(1.0f, 0.0f)}
};

GLuint floatingIndices[] = {
    0, 1, 2,
    0, 2, 3
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES (wysokie œwiat³o)
    Vertex{glm::vec3(-0.1f, 2.0f,  0.1f)},
    Vertex{glm::vec3(-0.1f, 2.0f, -0.1f)},
    Vertex{glm::vec3(0.1f, 2.0f, -0.1f)},
    Vertex{glm::vec3(0.1f, 2.0f,  0.1f)},
    Vertex{glm::vec3(-0.1f, 2.2f,  0.1f)},
    Vertex{glm::vec3(-0.1f, 2.2f, -0.1f)},
    Vertex{glm::vec3(0.1f, 2.2f, -0.1f)},
    Vertex{glm::vec3(0.1f, 2.2f,  0.1f)}
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
    Scene scene;
    DrawingHelper drawingHelper;

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

    window = glfwCreateWindow(mode->width, mode->height, "Liszaj", monitor, NULL);


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


    ////////////////////////////////////////////////////////////////////////////////////
    ///                        TEXTURES  & SHADERS                                   ///
    ////////////////////////////////////////////////////////////////////////////////////
    string shaderDir = filesystem::current_path().string() + "/src/shaders/";
    string texturteDir = filesystem::current_path().string() + "/src/textures/";
    
    Shader textureShader = drawingHelper.setupShaderProgram(shaderDir, "default");
    Shader colorShader = drawingHelper.setupShaderProgram(shaderDir, "color");
    Shader lightShader = drawingHelper.setupShaderProgram(shaderDir, "light");

    std::string fullPath = texturteDir + "penguin.png";
    std::cout << "laduje teksture " << fullPath << std::endl;

    Texture textures[]
    {
        Texture((texturteDir + "penguin.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
    };
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////


    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    // Create floor mesh
    Mesh floor(verts, ind, tex);

    std::vector <Vertex> floatVerts(floatingVertices, floatingVertices + sizeof(floatingVertices) / sizeof(Vertex));
    std::vector <GLuint> floatInd(floatingIndices, floatingIndices + sizeof(floatingIndices) / sizeof(GLuint));
    Mesh float1(floatVerts, floatInd, tex);

    // Store mesh data in vectors for the mesh
    std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
    // Create light mesh
    Mesh light(lightVerts, lightInd, tex);

    //add meshes
    scene.AddTextureMesh(&floor);
    scene.AddTextureMesh(&float1);
    scene.AddLightMesh(&light);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);


    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    textureShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(textureShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(textureShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Creates camera object
    Camera camera(mode->width, mode->height, glm::vec3(0.0f, 0.0f, 2.0f));


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Handles camera inputs
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Draws different meshes
        drawingHelper.drawScene(textureShader, colorShader, lightShader, scene, camera);

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    textureShader.Delete();
    lightShader.Delete();
    colorShader.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}