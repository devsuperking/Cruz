#include <cruz/core/application.h>
#include <cruz/rendering/renderers/primitive/primitive_renderer.h>
#include <cruz/core/time.h>
#include <array>
#include <cruz/core/orthographic_camera.h>

int main() {
    Application app(800, 600, "Primitive Example");

    auto backend = app.GetBackend();
    auto platform = backend->GetPlatform();

    int w, h;
    platform->GetFramebufferSize(w, h);

    OrthographicCamera camera{ float(w), float(h) };
    camera.SetPosition(0.0f, 0.0f);
    camera.orthoSize = 300.0f;

    PrimitiveRenderer primitive(backend, &camera);
    primitive.Initialize();

    platform->AddResizeCallback([&](int newW, int newH){
        camera.width  = float(newW);
        camera.height = float(newH);
    });

    float quadX = 0;
    float quadY = 0;
    const float moveSpeed = 250.0f;


    app.SetUpdateCallback([&](float dt) {
        primitive.BeginFrame();
        primitive.Clear({0.2f, 0.2f, 0.25f, 1.0f});

        PrimitiveSettings settings;
        settings.blend = true;
        settings.depthTest = false;
        primitive.SetPipeline(settings);

        if (platform->GetKey(KeyCode::W)) {
            quadY += moveSpeed * Time::GetDeltaTime();
        }
        if (platform->GetKey(KeyCode::S)) {
            quadY += -moveSpeed * Time::GetDeltaTime();
        }
        if (platform->GetKey(KeyCode::D)) {
            quadX += moveSpeed * Time::GetDeltaTime();
        }
        if (platform->GetKey(KeyCode::A)) {
            quadX += -moveSpeed * Time::GetDeltaTime();
        }

        float size = 40.0f;
        float col = platform->GetKey(KeyCode::W) ? 1 : 0;
        primitive.DrawQuad(quadX, quadY, size, size, {1, col, 0, 1}); // kwadrat na środku

        primitive.EndFrame();
    });

    app.Run();
    return 0;
}
