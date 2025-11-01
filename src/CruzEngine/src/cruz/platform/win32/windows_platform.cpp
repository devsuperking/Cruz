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

int WindowsPlatform::ToGLFWKey(KeyCode key)
{
    switch (key) {
        case KeyCode::A: return GLFW_KEY_A;
        case KeyCode::B: return GLFW_KEY_B;
        case KeyCode::C: return GLFW_KEY_C;
        case KeyCode::D: return GLFW_KEY_D;
        case KeyCode::E: return GLFW_KEY_E;
        case KeyCode::F: return GLFW_KEY_F;
        case KeyCode::G: return GLFW_KEY_G;
        case KeyCode::H: return GLFW_KEY_H;
        case KeyCode::I: return GLFW_KEY_I;
        case KeyCode::J: return GLFW_KEY_J;
        case KeyCode::K: return GLFW_KEY_K;
        case KeyCode::L: return GLFW_KEY_L;
        case KeyCode::M: return GLFW_KEY_M;
        case KeyCode::N: return GLFW_KEY_N;
        case KeyCode::O: return GLFW_KEY_O;
        case KeyCode::P: return GLFW_KEY_P;
        case KeyCode::Q: return GLFW_KEY_Q;
        case KeyCode::R: return GLFW_KEY_R;
        case KeyCode::S: return GLFW_KEY_S;
        case KeyCode::T: return GLFW_KEY_T;
        case KeyCode::U: return GLFW_KEY_U;
        case KeyCode::V: return GLFW_KEY_V;
        case KeyCode::W: return GLFW_KEY_W;
        case KeyCode::X: return GLFW_KEY_X;
        case KeyCode::Y: return GLFW_KEY_Y;
        case KeyCode::Z: return GLFW_KEY_Z;

        case KeyCode::Num0: return GLFW_KEY_0;
        case KeyCode::Num1: return GLFW_KEY_1;
        case KeyCode::Num2: return GLFW_KEY_2;
        case KeyCode::Num3: return GLFW_KEY_3;
        case KeyCode::Num4: return GLFW_KEY_4;
        case KeyCode::Num5: return GLFW_KEY_5;
        case KeyCode::Num6: return GLFW_KEY_6;
        case KeyCode::Num7: return GLFW_KEY_7;
        case KeyCode::Num8: return GLFW_KEY_8;
        case KeyCode::Num9: return GLFW_KEY_9;

        case KeyCode::Escape: return GLFW_KEY_ESCAPE;
        case KeyCode::Space: return GLFW_KEY_SPACE;
        case KeyCode::Enter: return GLFW_KEY_ENTER;
        case KeyCode::Tab: return GLFW_KEY_TAB;
        case KeyCode::Backspace: return GLFW_KEY_BACKSPACE;

        case KeyCode::Left: return GLFW_KEY_LEFT;
        case KeyCode::Right: return GLFW_KEY_RIGHT;
        case KeyCode::Up: return GLFW_KEY_UP;
        case KeyCode::Down: return GLFW_KEY_DOWN;

        case KeyCode::ShiftLeft: return GLFW_KEY_LEFT_SHIFT;
        case KeyCode::ShiftRight: return GLFW_KEY_RIGHT_SHIFT;
        case KeyCode::ControlLeft: return GLFW_KEY_LEFT_CONTROL;
        case KeyCode::ControlRight: return GLFW_KEY_RIGHT_CONTROL;
        case KeyCode::AltLeft: return GLFW_KEY_LEFT_ALT;
        case KeyCode::AltRight: return GLFW_KEY_RIGHT_ALT;

        case KeyCode::F1: return GLFW_KEY_F1;
        case KeyCode::F2: return GLFW_KEY_F2;
        case KeyCode::F3: return GLFW_KEY_F3;
        case KeyCode::F4: return GLFW_KEY_F4;
        case KeyCode::F5: return GLFW_KEY_F5;
        case KeyCode::F6: return GLFW_KEY_F6;
        case KeyCode::F7: return GLFW_KEY_F7;
        case KeyCode::F8: return GLFW_KEY_F8;
        case KeyCode::F9: return GLFW_KEY_F9;
        case KeyCode::F10: return GLFW_KEY_F10;
        case KeyCode::F11: return GLFW_KEY_F11;
        case KeyCode::F12: return GLFW_KEY_F12;

        default: return -1;
    }
}

uint64_t WindowsPlatform::AddResizeCallback(std::function<void(int,int)> func) {
    const uint64_t id = nextCallbackId++;
    resizeCallbacks[id] = std::move(func);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h) {
        if (auto* plat = static_cast<WindowsPlatform*>(glfwGetWindowUserPointer(win))) {
            for (auto& [_, cb] : plat->resizeCallbacks) {
                if (cb) cb(w, h);
            }
        }
    });

    return id;
}

void WindowsPlatform::RemoveResizeCallback(std::uint64_t id) {
    resizeCallbacks.erase(id);
}

bool WindowsPlatform::GetKeyDown(KeyCode key)
{
    int state = glfwGetKey(window, ToGLFWKey(key));
    static std::unordered_map<int, bool> prevState;

    bool wasPressed = prevState[ToGLFWKey(key)];
    prevState[ToGLFWKey(key)] = (state == GLFW_PRESS);

    return (state == GLFW_PRESS && !wasPressed);
}

bool WindowsPlatform::GetKeyUp(KeyCode key)
{
    int state = glfwGetKey(window, ToGLFWKey(key));
    static std::unordered_map<int, bool> prevState;

    bool wasPressed = prevState[ToGLFWKey(key)];
    prevState[ToGLFWKey(key)] = (state == GLFW_PRESS);

    return (state == GLFW_RELEASE && wasPressed);
}

bool WindowsPlatform::GetKey(KeyCode key)
{
    int state = glfwGetKey(window, ToGLFWKey(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
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
