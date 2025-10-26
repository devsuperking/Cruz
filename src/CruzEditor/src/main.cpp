#include <cruz/core/application.h>
#include <cruz/core/shader.h>
#include <vector>

int main() {
    Application app(800, 600, "Triangle Example");

    std::vector<Vertex> vertices = {
        { 0.0f,  0.5f, 0.0f },
        {-0.5f, -0.5f, 0.0f },
        { 0.5f, -0.5f, 0.0f }
    };

    static const char* vertexShaderSrc =
#if defined(EMSCRIPTEN)
        R"(layout(location = 0) in vec3 aPos;
uniform mat4 uProjection;
void main() { gl_Position = uProjection * vec4(aPos, 1.0); })";
#else
        R"(layout(location = 0) in vec3 aPos;
uniform mat4 uProjection;
void main() { gl_Position = uProjection * vec4(aPos, 1.0); })";
#endif

    static const char* fragmentShaderSrc =
#if defined(EMSCRIPTEN)
        R"(precision mediump float;
out vec4 FragColor;
void main() { FragColor = vec4(1.0, 0.5, 0.2, 1.0); })";
#else
        R"(out vec4 FragColor;
void main() { FragColor = vec4(1.0, 0.5, 0.2, 1.0); })";
#endif

    Shader* shader = nullptr;
    static bool initialized = false;

    app.SetUpdateCallback([&](float dt) {
        auto backend = app.GetBackend();

        if (!initialized) {
            shader = backend->CreateShader(vertexShaderSrc, fragmentShaderSrc);
            initialized = true;
        }

        float clearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
        backend->Clear(clearColor);

        backend->UseShader(shader);
        backend->SetUniformMat4(shader, "uProjection", app.GetBackend()->GetProjection());

        backend->Draw(vertices);
    });

    app.Run();

    delete shader;
    return 0;
}
