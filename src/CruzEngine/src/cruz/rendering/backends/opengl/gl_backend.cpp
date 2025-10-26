#include "gl_backend.h"
#include <GLFW/glfw3.h>
#include <iostream>

void GlBackend::Initialize()
{
    if (!platform) {
        std::cerr << "Platform not set!\n";
        return;
    }

    platform->MakeContextCurrent();

    if (!gladLoadGLLoader((GLADloadproc)platform->GetProcAddress())) {
        std::cerr << "Failed to initialize GLAD\n";
        return;
    }

    glEnable(GL_MULTISAMPLE);

    int width, height;
    platform->GetFramebufferSize(width, height);
    Resize(width, height);
}

void GlBackend::Resize(int width, int height)
{
    glViewport(0, 0, width, height);

    float aspect = static_cast<float>(width) / height;
    projection = Mat4::Ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
}

void GlBackend::Update(float deltaTime)
{
    if (!platform) return;

    if (platform->WindowShouldClose()) return;
    if (platform->GetKeyPressed(GLFW_KEY_ESCAPE))
        platform->SetWindowShouldClose(true);
}

void GlBackend::Render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!shaderProgram) return;

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, projection.data);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

unsigned int GlBackend::CreateShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader compilation failed:\n" << infoLog << "\n";
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader compilation failed:\n" << infoLog << "\n";
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program linking failed:\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int GlBackend::CreateVertexArray(const float* vertices, int vertexCount)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;
}

void GlBackend::SetUniformMat4(unsigned int program, const char* name, const Mat4& mat)
{
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, mat.data);
}

void GlBackend::DrawTriangles(unsigned int vao, int vertexCount)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

GlBackend::~GlBackend()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}
