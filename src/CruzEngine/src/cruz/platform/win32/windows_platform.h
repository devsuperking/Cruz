#pragma once
#include <cruz/core/platform.h>
#include <string>
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>

class WindowsPlatform : public Platform {
public:
    WindowsPlatform(int width, int height, const std::string& title);
    ~WindowsPlatform();

    bool WindowShouldClose() override;
    void SetWindowShouldClose(bool value) override;
    void PollEvents() override;
    void SwapBuffers() override;
    void GetFramebufferSize(int& width, int& height) override;
    void MakeContextCurrent() override;
    int GetKeyPressed(int key) override;
    void* GetProcAddress() override;

    void SetFramebufferSizeCallback(std::function<void(int, int)> cb) { framebufferCallback = cb; }

    GLFWwindow* GetWindow() { return window; }

private:
    void InitializeWindow(int width, int height, const std::string& title);
    void ResizeEvent(int width, int height) {
        if (framebufferCallback) framebufferCallback(width, height);
    }

    GLFWwindow* window = nullptr;
    std::function<void(int,int)> framebufferCallback;
};