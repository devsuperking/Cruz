#pragma once

#include <cruz/core/platform.h>
#include <cruz/core/rendering_backend.h>
#include <memory>
#include <functional>

class Application
{
public:
    using UpdateFunc = std::function<void(float)>;
    Application(int width, int height, const char* title = "App");
    ~Application();
    void SetUpdateCallback(UpdateFunc func);
    void Run();
    RenderingBackend* GetBackend() const { return m_backend.get(); }
    UpdateFunc GetUpdateFunc() const { return m_updateFunc; }
    void CallUpdate(float dt) { if (m_updateFunc) m_updateFunc(dt); }

private:
    std::unique_ptr<Platform> m_platform;
    std::unique_ptr<RenderingBackend> m_backend;

    UpdateFunc m_updateFunc;
};