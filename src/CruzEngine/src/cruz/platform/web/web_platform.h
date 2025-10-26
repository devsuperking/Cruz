#pragma once

#include <cruz/core/platform.h>
#include <functional>

class WebPlatform : public Platform {
public:
    WebPlatform(int width, int height);
    ~WebPlatform() = default;

    bool WindowShouldClose() override { return shouldClose; }
    void SetWindowShouldClose(bool value) override { shouldClose = value; }
    void PollEvents() override {}
    void SwapBuffers() override {}
    void GetFramebufferSize(int& width, int& height) override;
    void MakeContextCurrent() override {}
    int GetKeyPressed(int key) override { return 0; }
    void SetMainLoop(void (*func)());
    void* GetProcAddress() override { return nullptr; }

private:
    bool shouldClose = false;
};
