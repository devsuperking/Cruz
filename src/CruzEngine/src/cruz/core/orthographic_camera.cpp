#include "orthographic_camera.h"
#include <cmath>

OrthographicCamera::OrthographicCamera(float width, float height)
    : width(width), height(height) {}

void OrthographicCamera::SetPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void OrthographicCamera::Move(float dx, float dy) {
    x += dx;
    y += dy;
}

void OrthographicCamera::SetZoom(float z) {
    zoom = (z <= 0.0f) ? 0.001f : z;
}

Mat4 OrthographicCamera::GetViewMatrix() const {
    Mat4 translation = Mat4::Translate(-x, -y, 0.0f);
    return translation;
}

Mat4 OrthographicCamera::GetProjectionMatrix() const {
    float aspect = width / height;

    float halfSize = orthoSize * 0.5f / zoom;

    float left, right, bottom, top;

    if (aspect >= 1.0f) {
        left = -halfSize * aspect;
        right = halfSize * aspect;
        bottom = -halfSize;
        top = halfSize;
    } else {
        left = -halfSize;
        right = halfSize;
        bottom = -halfSize / aspect;
        top = halfSize / aspect;
    }

    return Mat4::Ortho(left, right, bottom, top, -1.0f, 1.0f);
}