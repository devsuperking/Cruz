#include "primitive_renderer.h"
#include <iostream>

PrimitiveRenderer::PrimitiveRenderer(RenderingBackend* backend, Camera* cam)
{
    this->m_backend = backend;
    this->camera = cam;
}

PrimitiveRenderer::~PrimitiveRenderer()
{
    delete m_shader;
}

void PrimitiveRenderer::Initialize()
{
    if (!m_backend) {
        std::cerr << "PrimitiveRenderer: Backend not set!\n";
        return;
    }

    const char* vertexSrc = R"(
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec4 aColor;
        uniform mat4 uProjection;
        out vec4 vColor;
        void main() {
            vColor = aColor;
            gl_Position = uProjection * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentSrc = R"(
        precision mediump float;
        in vec4 vColor;
        out vec4 FragColor;
        void main() { FragColor = vColor; }
    )";

    m_shader = m_backend->CreateShader(vertexSrc, fragmentSrc);
}

void PrimitiveRenderer::Clear(const std::array<float, 4>& color)
{
    if (!m_backend) return;
    m_backend->Clear(color.data());
}

void PrimitiveRenderer::SetPipeline(const PrimitiveSettings& settings)
{
    if (!m_backend) return;
    m_backend->SetPipeline({settings.depthTest, settings.blend});
}

void PrimitiveRenderer::BeginFrame()
{
    m_vertices.clear();
}

void PrimitiveRenderer::EndFrame()
{
    Flush();
}

void PrimitiveRenderer::DrawQuad(float x, float y, float width, float height, const std::array<float, 4>& color)
{
    float z = 0.0f;
    m_vertices.push_back({x, y, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x + width, y, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x + width, y + height, z, color[0], color[1], color[2], color[3]});

    m_vertices.push_back({x, y, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x + width, y + height, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x, y + height, z, color[0], color[1], color[2], color[3]});
}

void PrimitiveRenderer::DrawLine(float x1, float y1, float x2, float y2, const std::array<float, 4>& color)
{
    float z = 0.0f;
    m_vertices.push_back({x1, y1, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x2, y2, z, color[0], color[1], color[2], color[3]});
}

void PrimitiveRenderer::DrawPoint(float x, float y, float size, const std::array<float, 4>& color)
{
    DrawQuad(x - size/2.0f, y - size/2.0f, size, size, color);
}

void PrimitiveRenderer::Flush() {
    if (!m_backend || m_vertices.empty()) return;

    m_backend->UseShader(m_shader);

    Mat4 vp = camera->GetVPMatrix();
    m_backend->SetUniformMat4(m_shader, "uProjection", vp);

    m_backend->Draw(m_vertices);
}