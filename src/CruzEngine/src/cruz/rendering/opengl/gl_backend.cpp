#include "gl_backend.h"
#include <glad/glad.h>
#include <cruz/core/shader.h>
#include <cruz/rendering/opengl/gl_shader.h>
#include <iostream>
#include <GLFW/glfw3.h>

void GlBackend::Initialize() {
    if (!platform) { std::cerr << "Platform not set!\n"; return; }

    platform->MakeContextCurrent();

    if (!gladLoadGLLoader((GLADloadproc)platform->GetProcAddress())) {
        std::cerr << "Failed to initialize GLAD\n";
        return;
    }

    glEnable(GL_MULTISAMPLE);
    
    platform->SetResizeCallback([this](int width, int height) {
        std::cout << "New size: " << width << " x " << height << "\n";
        Resize(width, height);
    });
}

void GlBackend::Resize(int width, int height) {
    if (width <= 0 || height <= 0) return;

    glViewport(0, 0, width, height);
    float aspect = static_cast<float>(width) / height;
    projection = Mat4::Ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
}

void GlBackend::Update(float deltaTime) {
    if (!platform) return;

    if (platform->WindowShouldClose()) return;
    if (platform->GetKeyPressed(GLFW_KEY_ESCAPE))
        platform->SetWindowShouldClose(true);
}

void GlBackend::Clear(const float color[4]) {
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlBackend::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void GlBackend::SetPipeline(const PipelineSettings& settings) {
    if (settings.depthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (settings.blend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
}

Shader* GlBackend::CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLShader* shader = new GLShader();
    shader->Compile(vertexSrc, fragmentSrc);
    return shader;
}

void GlBackend::UseShader(Shader* shader) {
    if (!shader) return;
    GLShader* glShader = dynamic_cast<GLShader*>(shader);
    if (glShader)
        glShader->Use();
}

void GlBackend::SetUniformMat4(Shader* shader, const std::string& name, const Mat4& mat) {
    if (!shader) return;
    GLShader* glShader = dynamic_cast<GLShader*>(shader);
    if (glShader)
        glShader->SetUniformMat4(name, mat.data);
}

void GlBackend::Draw(const std::vector<Vertex>& vertices) {
    if (vertices.empty()) return;

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}