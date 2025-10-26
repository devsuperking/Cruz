#pragma once
#include <cruz/rendering/rendering_backend.h>
#include <glad/glad.h>

class GlBackend : public RenderingBackend {
public:
    void Initialize() override;
    void Resize(int width, int height) override;
    void Update(float deltaTime) override;
    void Render() override;

    unsigned int CreateShaderProgram(const char* vertexSrc, const char* fragmentSrc) override;
    unsigned int CreateVertexArray(const float* vertices, int vertexCount) override;
    void SetUniformMat4(unsigned int program, const char* name, const Mat4& mat) override;
    void DrawTriangles(unsigned int vao, int vertexCount) override;

    ~GlBackend();

private:
    unsigned int shaderProgram = 0;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    Mat4 projection;
};