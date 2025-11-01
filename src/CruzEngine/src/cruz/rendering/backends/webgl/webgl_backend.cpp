#include "webgl_backend.h"
#include <cruz/rendering/backends/webgl/webglshader.h>
#include <iostream>

EM_BOOL WebGlBackend::ResizeCallback(int, const EmscriptenUiEvent*, void* userData) {
    WebGlBackend* backend = reinterpret_cast<WebGlBackend*>(userData);
    int width, height;
    backend->platform->GetFramebufferSize(width, height);
    backend->Resize(width, height);
    return EM_TRUE;
}

void WebGlBackend::Initialize() {
    if (!platform) { std::cerr << "Platform not set!\n"; return; }

    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.majorVersion = 2;
    attr.minorVersion = 0;
    attr.alpha = false;
    attr.depth = true;
    attr.stencil = false;
    attr.antialias = true;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attr);
    if (context <= 0) { std::cerr << "Failed to create WebGL2 context!\n"; return; }
    emscripten_webgl_make_context_current(context);

    int width, height;
    platform->GetFramebufferSize(width, height);
    Resize(width, height);

    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_FALSE, ResizeCallback);
}

void WebGlBackend::Resize(int width, int height) {
    if (width <= 0 || height <= 0) return;
    glViewport(0, 0, width, height);

    float aspect = static_cast<float>(width) / height;
    projection = Mat4::Ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
}

void WebGlBackend::Clear(const float color[4]) {
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WebGlBackend::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void WebGlBackend::SetPipeline(const PipelineSettings& settings) {
    if (settings.depthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (settings.blend) { 
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else glDisable(GL_BLEND);
}

Shader* WebGlBackend::CreateShader(const std::string& vs, const std::string& fs) {
    WebGLShader* shader = new WebGLShader();
    shader->Compile(vs, fs);
    return shader;
}

void WebGlBackend::UseShader(Shader* shader) {
    if (!shader) return;
    WebGLShader* webglShader = dynamic_cast<WebGLShader*>(shader);
    if (webglShader) webglShader->Use();
}

void WebGlBackend::SetUniformMat4(Shader* shader, const std::string& name, const Mat4& mat) {
    if (!shader) return;
    WebGLShader* webglShader = dynamic_cast<WebGLShader*>(shader);
    if (webglShader) webglShader->SetUniformMat4(name, mat.data);
}

void WebGlBackend::UploadVertices(const std::vector<Vertex>& verts) {
    if (verts.empty()) return;

    if (vao != 0) {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexCount = static_cast<GLsizei>(verts.size());
    coloredVAO = false;
}

void WebGlBackend::DrawUploadedVertices() {
    if (vao == 0) return;
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void WebGlBackend::Draw(const std::vector<Vertex>& verts) {
    UploadVertices(verts);
    DrawUploadedVertices();
}

void WebGlBackend::Draw(const std::vector<ColoredVertex>& verts) {
    if (verts.empty()) return;

    if (vao != 0) { glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao); }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(ColoredVertex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, r));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexCount = static_cast<GLsizei>(verts.size());
    coloredVAO = true;
    DrawUploadedVertices();
}
