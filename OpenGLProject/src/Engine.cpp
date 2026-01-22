#include "../include/Engine.h"
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>
#include <thread>
#include <chrono>

Engine::Engine() {}

Engine::~Engine()
{
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

bool Engine::initWindow(GLFWmonitor* selectedMonitor, const std::string& title)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    this->monitor = selectedMonitor ? selectedMonitor : glfwGetPrimaryMonitor();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_DEPTH_TEST);

    return true;
}


static GLuint createSplashShader()
{
    const char* vs = R"(#version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTex;
        out vec2 Tex;
        void main()
        {
            Tex = aTex;
            gl_Position = vec4(aPos, 0.0, 1.0);
        })";

    const char* fs = R"(#version 330 core
        out vec4 FragColor;
        in vec2 Tex;
        uniform sampler2D tex;
        void main()
        {
            FragColor = texture(tex, Tex);
        })";

    auto compile = [](GLenum type, const char* src)
        {
            GLuint s = glCreateShader(type);
            glShaderSource(s, 1, &src, nullptr);
            glCompileShader(s);
            return s;
        };

    GLuint program = glCreateProgram();
    GLuint v = compile(GL_VERTEX_SHADER, vs);
    GLuint f = compile(GL_FRAGMENT_SHADER, fs);

    glAttachShader(program, v);
    glAttachShader(program, f);
    glLinkProgram(program);

    glDeleteShader(v);
    glDeleteShader(f);

    return program;
}

void Engine::showSplash(const std::string& imagePath)
{
    GLuint shader = createSplashShader();

    glViewport(0, 0, screenWidth, screenHeight);

    float quad[] = {
        // pos      // uv
        -1.f, -1.f, 0.f, 0.f,
         1.f, -1.f, 1.f, 0.f,
         1.f,  1.f, 1.f, 1.f,

        -1.f, -1.f, 0.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
        -1.f,  1.f, 0.f, 1.f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, ch;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(imagePath.c_str(), &w, &h, &ch, 0);
    if (!data)
    {
        std::cerr << "Nie mozna zaladowac splasha\n";
        return;
    }

    GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        w,
        h,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);

    glDisable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, tex);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    glEnable(GL_DEPTH_TEST);

    glDeleteTextures(1, &tex);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shader);
}