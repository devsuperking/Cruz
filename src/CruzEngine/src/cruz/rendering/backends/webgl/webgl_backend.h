#pragma once

#include <GLES3/gl3.h>
#include <emscripten/html5.h>
#include <functional>
#include <cruz/core/mat4.h>
#include <cruz/core/platform.h>

#pragma once

#include <GLES3/gl3.h>
#include <emscripten/html5.h>
#include <functional>
#include <cruz/core/mat4.h>
#include <cruz/core/platform.h>
#include <cruz/rendering/rendering_backend.h>

class WebGlBackend : public RenderingBackend {
public:
    void Initialize() override;
    void Resize(int width, int height) override;
    void Update(float deltaTime) override {}
    void Render() override;

    unsigned int CreateShaderProgram(const char* vertexSrc, const char* fragmentSrc) override;
    unsigned int CreateVertexArray(const float* vertices, int vertexCount) override;
    void SetUniformMat4(unsigned int program, const char* name, const Mat4& mat) override;
    void DrawTriangles(unsigned int vao, int vertexCount) override;

private:
    unsigned int shaderProgram = 0;
    unsigned int VAO = 0, VBO = 0;
    GLint projLoc = -1;
    Mat4 projection;

    static EM_BOOL ResizeCallback(int eventType, const EmscriptenUiEvent* e, void* userData);
};