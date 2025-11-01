#include <cruz/core/application.h>
#include <cruz/core/shader.h>
#include <vector>

struct Renderer {
    Shader* shader = nullptr;
    bool initialized = false;
    std::vector<Vertex> vertices;
    const char* vertexShaderSrc;
    const char* fragmentShaderSrc;

    void Update(Application& app, float dt) {
        auto backend = app.GetBackend();

        if (!initialized) {
            shader = backend->CreateShader(vertexShaderSrc, fragmentShaderSrc);
            initialized = true;
        }

        float clearColor[4] = { 0.25f, 0.1f, 0.1f, 1.0f };
        backend->Clear(clearColor);

        backend->UseShader(shader);
        backend->SetUniformMat4(shader, "uProjection", backend->GetProjection());

        backend->Draw(vertices);
    }
};

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
void main() { FragColor = vec4(1.0, 0.5, 0.3, 1.0); })";
#endif

    Renderer renderer;
    renderer.vertices = vertices;
    renderer.vertexShaderSrc = vertexShaderSrc;
    renderer.fragmentShaderSrc = fragmentShaderSrc;

    // Ustawiamy callback
    app.SetUpdateCallback([&](float dt) {
        renderer.Update(app, dt);
    });

    app.Run();

    delete renderer.shader;
    return 0;
}
