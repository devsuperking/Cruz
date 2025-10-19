#include "Renderer.h"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#ifdef __EMSCRIPTEN__
unsigned int Renderer::VAO = 0;
unsigned int Renderer::shaderProgram = 0;

// Callback do dynamicznego dopasowania viewportu
EM_BOOL resizeCallback(int eventType, const EmscriptenUiEvent* e, void* userData) {
    int width, height;
    emscripten_get_canvas_element_size("#canvas", &width, &height);
    glViewport(0, 0, width, height);
    return EM_TRUE;
}
#endif

void Renderer::HelloTriangle() {
    float vertices[] = {
        0.0f, 0.5f, 0.0f,    // top
       -0.5f, -0.5f, 0.0f,   // left
        0.5f, -0.5f, 0.0f    // right
    };

#ifdef __EMSCRIPTEN__
    // WebGL2 shaders (bez pustej linii przed #version!)
    const char* vertexShaderSource = R"(#version 300 es
layout(location = 0) in vec3 aPos;
void main() { gl_Position = vec4(aPos, 1.0); }
)";

    const char* fragmentShaderSource = R"(#version 300 es
precision mediump float;
out vec4 FragColor;
void main() { FragColor = vec4(1.0, 0.5, 0.2, 1.0); }
)";

    // Tworzenie kontekstu WebGL2
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;
    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.majorVersion = 2;
    attr.minorVersion = 0;
    attr.alpha = false;
    attr.depth = true;
    attr.stencil = false;
    attr.antialias = true;

    context = emscripten_webgl_create_context("#canvas", &attr);
    if (context <= 0) {
        std::cerr << "Failed to create WebGL2 context!\n";
        return;
    }
    emscripten_webgl_make_context_current(context);

    // Początkowy viewport
    int width, height;
    emscripten_get_canvas_element_size("#canvas", &width, &height);
    glViewport(0, 0, width, height);

    // Callback do resize okna
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, resizeCallback);

    // VAO / VBO
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Kompilacja shaderów
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Uruchomienie pętli renderującej
    emscripten_set_main_loop(renderLoop, 0, 1);

#else
    // Desktop OpenGL + GLFW
    const char* vertexShaderSource = R"(#version 330 core
layout(location = 0) in vec3 aPos;
void main() { gl_Position = vec4(aPos, 1.0); }
)";

    const char* fragmentShaderSource = R"(#version 330 core
out vec4 FragColor;
void main() { FragColor = vec4(1.0, 0.5, 0.2, 1.0); }
)";

    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW\n"; return; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); return; }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cerr << "Failed to initialize GLAD\n"; return; }

    glViewport(0, 0, 800, 600);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
#endif
}

#ifdef __EMSCRIPTEN__
void Renderer::renderLoop() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
#endif
