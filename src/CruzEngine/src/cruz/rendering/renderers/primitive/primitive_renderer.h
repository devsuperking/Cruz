#pragma once
#include <cruz/core/rendering_backend.h>
#include <cruz/core/mat4.h>
#include <cruz/core/vertex.h>
#include <cruz/core/camera.h>
#include <vector>
#include <array>

struct PrimitiveSettings {
    bool depthTest = false;
    bool blend = true;
};

class PrimitiveRenderer {
public:
    PrimitiveRenderer(RenderingBackend* backend, Camera* cam);
    ~PrimitiveRenderer();

    void Initialize();
    void Clear(const std::array<float,4>& color);
    void SetPipeline(const PrimitiveSettings& settings);

    void BeginFrame();
    void EndFrame();

    void DrawQuad(float x, float y, float width, float height, const std::array<float,4>& color);
    void DrawLine(float x1, float y1, float x2, float y2, const std::array<float,4>& color);
    void DrawPoint(float x, float y, float size, const std::array<float,4>& color);

    void Flush();

private:
    RenderingBackend* m_backend = nullptr;
    Shader* m_shader = nullptr;

    std::vector<ColoredVertex> m_vertices;
    Mat4 m_projection;
    Camera* camera;
};
