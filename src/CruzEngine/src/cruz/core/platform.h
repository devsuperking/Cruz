#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <cruz/core/definitions.h>

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
    virtual std::uint64_t AddResizeCallback(std::function<void(int,int)> func) = 0;
    virtual void RemoveResizeCallback(std::uint64_t id) = 0;
    virtual bool GetKeyDown(KeyCode key) = 0;
    virtual bool GetKeyUp(KeyCode key) = 0;
    virtual bool GetKey(KeyCode key) = 0;

protected:
    std::unordered_map<std::uint64_t, std::function<void(int,int)>> resizeCallbacks;
    std::uint64_t nextCallbackId = 1;

};