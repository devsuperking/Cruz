#include "application.h"

#if defined(EMSCRIPTEN)
#include "web_platform.h"
#include "webgl_backend.h"
#include <emscripten/emscripten.h>
#else
#include "windows_platform.h"
#include <cruz/rendering/opengl/gl_backend.h>
#endif

static Application *g_app = nullptr;

#if defined(EMSCRIPTEN)
static void MainLoop()
{
    if (g_app)
        g_app->GetUpdateFunc()(0.016f);
}
#endif

Application::Application(int width, int height, const char *title)
{
#if defined(EMSCRIPTEN)
    m_platform = std::make_unique<WebPlatform>(width, height);
    m_backend = std::make_unique<WebGlBackend>();
#else
    m_platform = std::make_unique<WindowsPlatform>(width, height, title);
    m_backend = std::make_unique<GlBackend>();
#endif

    m_backend->SetPlatform(m_platform.get());
    m_backend->Initialize();
}

Application::~Application()
{
#if defined(EMSCRIPTEN)
    emscripten_cancel_main_loop();
#endif
}

void Application::SetUpdateCallback(UpdateFunc func)
{
    m_updateFunc = func;
}

void Application::Run()
{
#if defined(EMSCRIPTEN)
    g_app = this;
    if (auto web = dynamic_cast<WebPlatform *>(m_platform.get()))
    {
        web->SetMainLoop(MainLoop);
    }
#else
    int width, height;
    while (!m_platform->WindowShouldClose())
    {
        m_updateFunc(0.016f);

        m_platform->SwapBuffers();
        m_platform->PollEvents();
    }
#endif
}