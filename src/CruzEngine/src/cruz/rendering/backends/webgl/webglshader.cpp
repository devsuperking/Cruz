#include "webglshader.h"
#include <iostream>

WebGLShader::~WebGLShader() {
    if (program != 0)
        glDeleteProgram(program);
}

void WebGLShader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexCode = PreprocessVertex(vertexSrc);
    const char* vsrc = vertexCode.c_str();
    glShaderSource(vertex, 1, &vsrc, nullptr);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentCode = PreprocessFragment(fragmentSrc);
    const char* fsrc = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fsrc, nullptr);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    CheckCompileErrors(program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void WebGLShader::Use() {
    glUseProgram(program);
}

void WebGLShader::SetUniformMat4(const std::string& name, const float* mat) {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}

GLuint WebGLShader::GetProgram() const {
    return program;
}

std::string WebGLShader::PreprocessVertex(const std::string& src) {
#if defined(EMSCRIPTEN)
    return "#version 300 es\n" + src;
#else
    return src;
#endif
}

std::string WebGLShader::PreprocessFragment(const std::string& src) {
#if defined(EMSCRIPTEN)
    return "#version 300 es\nprecision mediump float;\n" + src;
#else
    return src;
#endif
}

void WebGLShader::CheckCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << type << " SHADER COMPILATION ERROR:\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "PROGRAM LINK ERROR:\n" << infoLog << std::endl;
        }
    }
}
 