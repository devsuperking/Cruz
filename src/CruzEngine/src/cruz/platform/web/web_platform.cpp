#include "web_platform.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

WebPlatform::WebPlatform(int width, int height) {
    emscripten_set_canvas_element_size("#canvas", width, height);
}

void WebPlatform::GetFramebufferSize(int& width, int& height) {
    double cssWidth, cssHeight;
    emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);

    double dpr = emscripten_get_device_pixel_ratio();
    width  = static_cast<int>(cssWidth  * dpr);
    height = static_cast<int>(cssHeight * dpr);

    emscripten_set_canvas_element_size("#canvas", width, height);
}

