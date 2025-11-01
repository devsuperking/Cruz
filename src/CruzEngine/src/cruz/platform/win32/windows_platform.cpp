#include "windows_platform.h"

WindowsPlatform::WindowsPlatform(int width, int height, const std::string& title) {
    InitializeWindow(width, height, title);
}

WindowsPlatform::~WindowsPlatform() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

bool WindowsPlatform::WindowShouldClose() {
    return window && glfwWindowShouldClose(window);
}

void WindowsPlatform::SetWindowShouldClose(bool value) {
    if (window) glfwSetWindowShouldClose(window, value);
}

void WindowsPlatform::PollEvents() {
    glfwPollEvents();
}

void WindowsPlatform::SwapBuffers() {
    if (window) glfwSwapBuffers(window);
}

void WindowsPlatform::GetFramebufferSize(int& width, int& height) {
    if (window) glfwGetFramebufferSize(window, &width, &height);
}

void WindowsPlatform::InitializeWindow(int width, int height, const std::string& title) {
    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW\n"; return; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); return; }

    glfwSetWindowUserPointer(window, this);
}

void WindowsPlatform::SetResizeCallback(std::function<void(int,int)> func) {
    resizeCallback = std::move(func);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h){
        if(auto* plat = static_cast<WindowsPlatform*>(glfwGetWindowUserPointer(win))) {
            if(plat->resizeCallback)
                plat->resizeCallback(w,h);
        }
    });
}

void WindowsPlatform::MakeContextCurrent() {
    if (window) glfwMakeContextCurrent(window);
}

int WindowsPlatform::GetKeyPressed(int key) {
    if (!window) return 0;
    return glfwGetKey(window, key);
}

void* WindowsPlatform::GetProcAddress() {
    return reinterpret_cast<void*>(glfwGetProcAddress);
}
