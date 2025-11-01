#pragma once
#include <GLES3/gl3.h>
#include <emscripten/html5.h>
#include <functional>
#include <cruz/core/mat4.h>
#include <cruz/core/platform.h>
#include <cruz/core/rendering_backend.h>
#include <cruz/core/shader.h>
#include <cruz/core/vertex.h>

class WebGlBackend : public RenderingBackend {
public:
    void Initialize() override;
    void Resize(int width, int height) override;
    void Update(float deltaTime) override {}

    void Clear(const float color[4]) override;
    void SetViewport(int x, int y, int width, int height) override;
    void SetPipeline(const PipelineSettings& settings) override;

    Shader* CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) override;
    void UseShader(Shader* shader) override;
    void SetUniformMat4(Shader* shader, const std::string& name, const Mat4& mat) override;

    void Draw(const std::vector<Vertex>& vertices) override;
    const Mat4& GetProjection() const override;

    void UploadVertices(const std::vector<Vertex>& vertices) override;
    void DrawUploadedVertices() override;


private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;
    Mat4 projection;

    static EM_BOOL ResizeCallback(int eventType, const EmscriptenUiEvent* e, void* userData);
};
