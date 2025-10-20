#include "webgl_backend.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

WebGlBackend *WebGlBackend::instance = nullptr;

void RenderLoopCallback()
{
	if (WebGlBackend::instance)
		WebGlBackend::instance->RenderLoop();
}

void WebGlBackend::HelloTriangle()
{
	WebGlBackend::instance = this;

	float vertices[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f};

	const char *vertexShaderSource = R"(#version 300 es
layout(location = 0) in vec3 aPos;
uniform float uAspect;
void main() {
    gl_Position = vec4(aPos.x / uAspect, aPos.y, aPos.z, 1.0);
})";

	const char *fragmentShaderSource = R"(#version 300 es
precision mediump float;
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
})";

	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;
	EmscriptenWebGLContextAttributes attr;
	emscripten_webgl_init_context_attributes(&attr);
	attr.majorVersion = 2;
	attr.minorVersion = 0;
	attr.alpha = false;
	attr.depth = true;
	attr.stencil = false;
	attr.antialias = true;

	context = emscripten_webgl_create_context("#canvas", &attr);
	if (context <= 0)
	{
		std::cerr << "Failed to create WebGL2 context!\n";
		return;
	}
	emscripten_webgl_make_context_current(context);

	double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	int width = static_cast<int>(cssWidth * emscripten_get_device_pixel_ratio());
	int height = static_cast<int>(cssHeight * emscripten_get_device_pixel_ratio());
	emscripten_set_canvas_element_size("#canvas", width, height);
	glViewport(0, 0, width, height);

	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr,
								   EM_TRUE, WebGlBackend::ViewportResizeCallback);

	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	aspectLoc = glGetUniformLocation(shaderProgram, "uAspect");

	glUseProgram(shaderProgram);
	glUniform1f(aspectLoc, static_cast<float>(width) / static_cast<float>(height));

	emscripten_set_main_loop(RenderLoopCallback, 0, 1);
}

void WebGlBackend::RenderLoop()
{
	if (!instance)
		return;

	double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	int width = static_cast<int>(cssWidth * emscripten_get_device_pixel_ratio());
	int height = static_cast<int>(cssHeight * emscripten_get_device_pixel_ratio());
	emscripten_set_canvas_element_size("#canvas", width, height);
	glViewport(0, 0, width, height);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(instance->shaderProgram);
	glUniform1f(instance->aspectLoc, static_cast<float>(width) / static_cast<float>(height));

	glBindVertexArray(instance->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

EM_BOOL WebGlBackend::ViewportResizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData)
{
	if (!instance)
		return EM_TRUE;

	double cssWidth, cssHeight;
	emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);
	int width = static_cast<int>(cssWidth * emscripten_get_device_pixel_ratio());
	int height = static_cast<int>(cssHeight * emscripten_get_device_pixel_ratio());
	emscripten_set_canvas_element_size("#canvas", width, height);
	glViewport(0, 0, width, height);

	return EM_TRUE;
}
