#ifdef __EMSCRIPTEN__
#include <cruz/rendering/backends/webgl/webgl_backend.h>
#else
#include <cruz/rendering/backends/opengl/gl_backend.h>
#endif
#include <memory>

int main()
{
  std::unique_ptr<RenderingBackend> backend;

#ifdef __EMSCRIPTEN__
  backend = std::make_unique<WebGlBackend>();
#else
  backend = std::make_unique<GlBackend>();
#endif

  backend->HelloTriangle();

  return 0;
}