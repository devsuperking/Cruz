#include "webgl_backend.h"
#include <iostream>

EM_BOOL WebGlBackend::ResizeCallback(int eventType, const EmscriptenUiEvent* e, void* userData) {
    WebGlBackend* backend = reinterpret_cast<WebGlBackend*>(userData);
    int width, height;
    backend->platform->GetFramebufferSize(width, height);
    backend->Resize(width, height);
    return EM_TRUE;
}

void WebGlBackend::Initialize() {
    if (!platform) { std::cerr << "Platform not set!\n"; return; }

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

    int width, height;
    platform->GetFramebufferSize(width, height);
    Resize(width, height);

    platform->SetFramebufferSizeCallback([this](int w, int h) {
        this->Resize(w, h);
    });

    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_FALSE, ResizeCallback);
}

void WebGlBackend::Resize(int width, int height) {
    if (width <= 0 || height <= 0) return;

    glViewport(0, 0, width, height);
    float aspect = static_cast<float>(width) / height;
    projection = Mat4::Ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

    if (shaderProgram)
        SetUniformMat4(shaderProgram, "uProjection", projection);
}

void WebGlBackend::Render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

unsigned int WebGlBackend::CreateShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader error: " << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader error: " << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Program link error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    projLoc = glGetUniformLocation(shaderProgram, "uProjection");
    return shaderProgram;
}

unsigned int WebGlBackend::CreateVertexArray(const float* vertices, int vertexCount) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;
}

void WebGlBackend::SetUniformMat4(unsigned int program, const char* name, const Mat4& mat) {
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, mat.data);
}

void WebGlBackend::DrawTriangles(unsigned int vao, int vertexCount) {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
