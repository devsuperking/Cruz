#include <memory>
#include <cruz/core/mat4.h>
#include <cruz/rendering/rendering_backend.h>

#if defined(EMSCRIPTEN)
#include <cruz/platform/web/web_platform.h>
#include <cruz/rendering/backends/webgl/webgl_backend.h>
#include <emscripten/emscripten.h>
WebGlBackend* g_backend = nullptr;

void MainLoop() {
    if (g_backend) g_backend->Render();
}
#else
#include <cruz/platform/win32/windows_platform.h>
#include <cruz/rendering/backends/opengl/gl_backend.h>
#endif

int main() {
#if defined(EMSCRIPTEN)
    auto platform = std::make_unique<WebPlatform>(800, 600);
    auto backend = std::make_unique<WebGlBackend>();
#else
    auto platform = std::make_unique<WindowsPlatform>(800, 600, "Hello Triangle");
    auto backend = std::make_unique<GlBackend>();
#endif

    backend->SetPlatform(platform.get());
    backend->Initialize();

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

#if defined(EMSCRIPTEN)
    const char* vertexShaderSrc = R"(#version 300 es
layout(location = 0) in vec3 aPos;
uniform mat4 uProjection;
void main() {
    gl_Position = uProjection * vec4(aPos, 1.0);
})";

    const char* fragmentShaderSrc = R"(#version 300 es
precision mediump float;
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
})";
#else
    const char* vertexShaderSrc = R"(#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 uProjection;
void main() {
    gl_Position = uProjection * vec4(aPos, 1.0);
})";

    const char* fragmentShaderSrc = R"(#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
})";
#endif

    unsigned int shader = backend->CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    unsigned int vao = backend->CreateVertexArray(vertices, 3);

    int width, height;
    platform->GetFramebufferSize(width, height);
    backend->Resize(width, height);

#if defined(EMSCRIPTEN)
    g_backend = backend.get();
    emscripten_set_main_loop(MainLoop, 0, true);
#else
    while (!platform->WindowShouldClose()) {
        backend->Update(0.016f);
        backend->Render();

        platform->GetFramebufferSize(width, height);
        backend->Resize(width, height);

        platform->SwapBuffers();
        platform->PollEvents();
    }
#endif

    return 0;
}