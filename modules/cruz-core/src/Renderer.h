#pragma once

class Renderer {
public:
    // Uruchamia renderowanie prostego trójkąta
    void HelloTriangle();

private:
#ifdef __EMSCRIPTEN__
    static void renderLoop();
    static unsigned int VAO;
    static unsigned int shaderProgram;
#endif
};