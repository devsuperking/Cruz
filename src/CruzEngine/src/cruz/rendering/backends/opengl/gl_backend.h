#pragma once

#include <cruz/rendering/rendering_backend.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GlBackend : public RenderingBackend
{
public:
  void HelloTriangle() override;

private:
  void ViewportResizeCallback(GLFWwindow *window, int width, int height);

  unsigned int VAO = 0;
  unsigned int shaderProgram = 0;
  GLFWwindow *window = nullptr;
};