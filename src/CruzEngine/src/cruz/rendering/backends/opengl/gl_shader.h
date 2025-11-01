#pragma once

#include <cruz/core/shader.h>
#include <glad/glad.h>
#include <string>

class GLShader : public Shader {
public:
    GLShader() = default;
    ~GLShader() override;

    void Compile(const std::string& vertexSrc, const std::string& fragmentSrc) override;
    void Use() override;
    void SetUniformMat4(const std::string& name, const float* mat) override;
    GLuint GetProgram() const;

private:
    GLuint program = 0;

    void CheckCompileErrors(GLuint shader, const std::string& type);
};