#include "application.h"

#if defined(EMSCRIPTEN)
#include "web_platform.h"
#include "webgl_backend.h"
#include <emscripten/emscripten.h>
#else
#include "windows_platform.h"
#include <cruz/rendering/backends/opengl/gl_backend.h>
#endif
#include <cruz/core/time.h>

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

    static double s_lastTime = 0.0;
    s_lastTime = emscripten_get_now() / 1000.0;

    // Główna pętla przeglądarkowa
    auto mainLoop = []()
    {
        double now = emscripten_get_now() / 1000.0;
        static double lastTime = now;

        double dt = now - lastTime;
        if (dt < 0.0 || dt > 1.0) dt = 0.016;
        lastTime = now;

        Time::SetDeltaTime(static_cast<float>(dt));

        if (g_app && g_app->GetUpdateFunc())
            g_app->GetUpdateFunc()(static_cast<float>(dt));
    };

    if (auto web = dynamic_cast<WebPlatform *>(m_platform.get()))
    {
        web->SetMainLoop(mainLoop);
    }

#else
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (!m_platform->WindowShouldClose())
    {
        auto now = clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        Time::SetDeltaTime(dt);
        m_updateFunc(dt);

        m_platform->SwapBuffers();
        m_platform->PollEvents();
    }
#endif
}