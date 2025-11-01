#include "web_platform.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

WebPlatform::WebPlatform(int width, int height) {
    emscripten_set_canvas_element_size("#canvas", width, height);
    SetKeyboardCallbacks();
}

void WebPlatform::GetFramebufferSize(int& width, int& height) {
    double cssWidth, cssHeight;
    emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);

    double dpr = emscripten_get_device_pixel_ratio();
    width  = static_cast<int>(cssWidth  * dpr);
    height = static_cast<int>(cssHeight * dpr);

    emscripten_set_canvas_element_size("#canvas", width, height);
}

void WebPlatform::SetMainLoop(void (*func)())
{
    emscripten_set_main_loop(func, 0, true);
}

uint64_t WebPlatform::AddResizeCallback(std::function<void(int, int)> func) {
    uint64_t id = nextCallbackId++;
    resizeCallbacks[id] = std::move(func);

    static bool callbackRegistered = false;
    if (!callbackRegistered) {
        emscripten_set_resize_callback(
    EMSCRIPTEN_EVENT_TARGET_WINDOW,
    this,
    EM_TRUE,
    ( +[](int, const EmscriptenUiEvent*, void* userData) -> EM_BOOL {
        WebPlatform* platform = reinterpret_cast<WebPlatform*>(userData);
        int width, height;
        platform->GetFramebufferSize(width, height);

        for (auto& [id, callback] : platform->resizeCallbacks) {
            if (callback) callback(width, height);
        }
        return EM_TRUE;
    } )
);
        callbackRegistered = true;
    }

    return id;
}

void WebPlatform::RemoveResizeCallback(uint64_t id)
{
    resizeCallbacks.erase(id);
}

bool WebPlatform::GetKeyDown(KeyCode key)
{
    bool pressed = keyState[key];
    bool down = keyDownState[key];
    keyDownState[key] = false;
    return down;
}

bool WebPlatform::GetKeyUp(KeyCode key) {
    bool released = keyUpState[key];
    keyUpState[key] = false;
    return released;
}

bool WebPlatform::GetKey(KeyCode key)
{
    return keyState[key];
}

KeyCode WebPlatform::ConvertKeyStringToKeyCode(const char* keyStr)
{
    if (strcmp(keyStr, "ArrowUp") == 0) return KeyCode::Up;
    if (strcmp(keyStr, "ArrowDown") == 0) return KeyCode::Down;
    if (strcmp(keyStr, "ArrowLeft") == 0) return KeyCode::Left;
    if (strcmp(keyStr, "ArrowRight") == 0) return KeyCode::Right;
    if (strcmp(keyStr, " ") == 0) return KeyCode::Space;
    if (strcmp(keyStr, "Enter") == 0) return KeyCode::Enter;
    if (strcmp(keyStr, "Escape") == 0) return KeyCode::Escape;
    if (strcmp(keyStr, "Tab") == 0) return KeyCode::Tab;
    if (strcmp(keyStr, "Backspace") == 0) return KeyCode::Backspace;
    if (strcmp(keyStr, "Shift") == 0) return KeyCode::ShiftLeft; // Uproszczone
    if (strcmp(keyStr, "Control") == 0) return KeyCode::ControlLeft;
    if (strcmp(keyStr, "Alt") == 0) return KeyCode::AltLeft;

    if (strcmp(keyStr, "a") == 0 || strcmp(keyStr, "A") == 0) return KeyCode::A;
    if (strcmp(keyStr, "b") == 0 || strcmp(keyStr, "B") == 0) return KeyCode::B;
    if (strcmp(keyStr, "c") == 0 || strcmp(keyStr, "C") == 0) return KeyCode::C;
    if (strcmp(keyStr, "d") == 0 || strcmp(keyStr, "D") == 0) return KeyCode::D;
    if (strcmp(keyStr, "e") == 0 || strcmp(keyStr, "E") == 0) return KeyCode::E;
    if (strcmp(keyStr, "f") == 0 || strcmp(keyStr, "F") == 0) return KeyCode::F;
    if (strcmp(keyStr, "g") == 0 || strcmp(keyStr, "G") == 0) return KeyCode::G;
    if (strcmp(keyStr, "h") == 0 || strcmp(keyStr, "H") == 0) return KeyCode::H;
    if (strcmp(keyStr, "i") == 0 || strcmp(keyStr, "I") == 0) return KeyCode::I;
    if (strcmp(keyStr, "j") == 0 || strcmp(keyStr, "J") == 0) return KeyCode::J;
    if (strcmp(keyStr, "k") == 0 || strcmp(keyStr, "K") == 0) return KeyCode::K;
    if (strcmp(keyStr, "l") == 0 || strcmp(keyStr, "L") == 0) return KeyCode::L;
    if (strcmp(keyStr, "m") == 0 || strcmp(keyStr, "M") == 0) return KeyCode::M;
    if (strcmp(keyStr, "n") == 0 || strcmp(keyStr, "N") == 0) return KeyCode::N;
    if (strcmp(keyStr, "o") == 0 || strcmp(keyStr, "O") == 0) return KeyCode::O;
    if (strcmp(keyStr, "p") == 0 || strcmp(keyStr, "P") == 0) return KeyCode::P;
    if (strcmp(keyStr, "q") == 0 || strcmp(keyStr, "Q") == 0) return KeyCode::Q;
    if (strcmp(keyStr, "r") == 0 || strcmp(keyStr, "R") == 0) return KeyCode::R;
    if (strcmp(keyStr, "s") == 0 || strcmp(keyStr, "S") == 0) return KeyCode::S;
    if (strcmp(keyStr, "t") == 0 || strcmp(keyStr, "T") == 0) return KeyCode::T;
    if (strcmp(keyStr, "u") == 0 || strcmp(keyStr, "U") == 0) return KeyCode::U;
    if (strcmp(keyStr, "v") == 0 || strcmp(keyStr, "V") == 0) return KeyCode::V;
    if (strcmp(keyStr, "w") == 0 || strcmp(keyStr, "W") == 0) return KeyCode::W;
    if (strcmp(keyStr, "x") == 0 || strcmp(keyStr, "X") == 0) return KeyCode::X;
    if (strcmp(keyStr, "y") == 0 || strcmp(keyStr, "Y") == 0) return KeyCode::Y;
    if (strcmp(keyStr, "z") == 0 || strcmp(keyStr, "Z") == 0) return KeyCode::Z;

    if (strcmp(keyStr, "0") == 0) return KeyCode::Num0;
    if (strcmp(keyStr, "1") == 0) return KeyCode::Num1;
    if (strcmp(keyStr, "2") == 0) return KeyCode::Num2;
    if (strcmp(keyStr, "3") == 0) return KeyCode::Num3;
    if (strcmp(keyStr, "4") == 0) return KeyCode::Num4;
    if (strcmp(keyStr, "5") == 0) return KeyCode::Num5;
    if (strcmp(keyStr, "6") == 0) return KeyCode::Num6;
    if (strcmp(keyStr, "7") == 0) return KeyCode::Num7;
    if (strcmp(keyStr, "8") == 0) return KeyCode::Num8;
    if (strcmp(keyStr, "9") == 0) return KeyCode::Num9;

    if (strcmp(keyStr, "F1") == 0) return KeyCode::F1;
    if (strcmp(keyStr, "F2") == 0) return KeyCode::F2;
    if (strcmp(keyStr, "F3") == 0) return KeyCode::F3;
    if (strcmp(keyStr, "F4") == 0) return KeyCode::F4;
    if (strcmp(keyStr, "F5") == 0) return KeyCode::F5;
    if (strcmp(keyStr, "F6") == 0) return KeyCode::F6;
    if (strcmp(keyStr, "F7") == 0) return KeyCode::F7;
    if (strcmp(keyStr, "F8") == 0) return KeyCode::F8;
    if (strcmp(keyStr, "F9") == 0) return KeyCode::F9;
    if (strcmp(keyStr, "F10") == 0) return KeyCode::F10;
    if (strcmp(keyStr, "F11") == 0) return KeyCode::F11;
    if (strcmp(keyStr, "F12") == 0) return KeyCode::F12;

    return KeyCode::Unknown;
}

void WebPlatform::SetKeyboardCallbacks()
{
    emscripten_set_keydown_callback(
    EMSCRIPTEN_EVENT_TARGET_WINDOW,
    this,
    EM_TRUE,
    (+[](int, const EmscriptenKeyboardEvent* e, void* userData) -> EM_BOOL {
        WebPlatform* platform = reinterpret_cast<WebPlatform*>(userData);
        KeyCode key = platform->ConvertKeyStringToKeyCode(e->key);
        if (key != KeyCode::Unknown) {
            if (!platform->keyState[key]) platform->keyDownState[key] = true;
            platform->keyState[key] = true;
        }
        return EM_TRUE;
    })
);

    emscripten_set_keyup_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW,
        this,
        EM_TRUE,
        (+[](int, const EmscriptenKeyboardEvent* e, void* userData) -> EM_BOOL {
            WebPlatform* platform = reinterpret_cast<WebPlatform*>(userData);
            KeyCode key = platform->ConvertKeyStringToKeyCode(e->key);
            if (key != KeyCode::Unknown) {
                platform->keyState[key] = false;
                platform->keyUpState[key] = true;
            }
            return EM_TRUE;
        })
    );
}
