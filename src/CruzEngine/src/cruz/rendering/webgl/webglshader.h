#pragma once

#include <cruz/core/shader.h>
#include <GLES3/gl3.h>
#include <string>

class WebGLShader : public Shader {
public:
    WebGLShader() = default;
    ~WebGLShader() override;

    void Compile(const std::string& vertexSrc, const std::string& fragmentSrc) override;
    void Use() override;
    void SetUniformMat4(const std::string& name, const float* mat) override;
    GLuint GetProgram() const;

private:
    GLuint program = 0;

    std::string PreprocessVertex(const std::string& src);
    std::string PreprocessFragment(const std::string& src);
    void CheckCompileErrors(GLuint shader, const std::string& type);
};