#pragma once
#include <string>
#include <functional>

class Platform {
public:
    virtual ~Platform() = default;
    virtual void MakeContextCurrent() = 0;
    virtual bool WindowShouldClose() = 0;
    virtual void SetWindowShouldClose(bool value) = 0;
    virtual void PollEvents() = 0;
    virtual void SwapBuffers() = 0;
    virtual void GetFramebufferSize(int& width, int& height) = 0;
    virtual int GetKeyPressed(int key) = 0;
    virtual void* GetProcAddress() = 0;
    virtual void SetFramebufferSizeCallback(std::function<void(int,int)> cb) {}
};