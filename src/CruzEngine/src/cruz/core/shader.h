#pragma once

#include <string>

class Shader {
public:
    virtual ~Shader() = default;

    virtual void Compile(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
    virtual void SetUniformMat4(const std::string& name, const float* mat) = 0;
    virtual void Use() = 0;
};