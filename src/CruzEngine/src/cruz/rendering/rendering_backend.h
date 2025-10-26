#pragma once
#include <cruz/core/mat4.h>
#include <cruz/core/platform.h>

class RenderingBackend
{
public:
    virtual ~RenderingBackend() = default;

    virtual void Initialize() = 0;
    virtual void Resize(int width, int height) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    virtual unsigned int CreateShaderProgram(const char* vertexSrc, const char* fragmentSrc) = 0;
    virtual unsigned int CreateVertexArray(const float* vertices, int vertexCount) = 0;
    virtual void SetUniformMat4(unsigned int program, const char* name, const Mat4& mat) = 0;
    virtual void DrawTriangles(unsigned int vao, int vertexCount) = 0;

    void SetPlatform(Platform* platform) { this->platform = platform; }

protected:
    Platform* platform = nullptr;
};
