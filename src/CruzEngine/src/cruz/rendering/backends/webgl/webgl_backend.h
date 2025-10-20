#pragma once
#include <cruz/rendering/rendering_backend.h>
#include <GLES3/gl3.h>
#include <emscripten/html5.h>

class WebGlBackend : public RenderingBackend
{
public:
  void HelloTriangle() override;
  static WebGlBackend *instance;
  static void RenderLoop();

private:
  static EM_BOOL ViewportResizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData);
  void ResizeCanvas();

  unsigned int shaderProgram = 0;
  unsigned int VAO = 0;
  GLint aspectLoc = -1; // uniform location for aspect ratio
};
