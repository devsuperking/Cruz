#pragma once
#include "mat4.h"
#include "camera.h"

class OrthographicCamera : public Camera {
public:
    OrthographicCamera() = default;
    OrthographicCamera(float width, float height);

    void SetPosition(float x, float y);
    void Move(float dx, float dy);
    void SetZoom(float z);

    Mat4 GetViewMatrix() const override;
    Mat4 GetProjectionMatrix() const override;

    float x = 0.0f, y = 0.0f;
    float zoom = 1.0f;

    float width = 800.0f;
    float height = 600.0f;

    float orthoSize = 300.0f; // stała wysokość widocznego obszaru
};
