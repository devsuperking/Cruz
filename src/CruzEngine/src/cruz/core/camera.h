#pragma once
#include <cruz/core/mat4.h>

class Camera {
public:
    virtual ~Camera() = default;
    virtual Mat4 GetViewMatrix() const = 0;
    virtual Mat4 GetProjectionMatrix() const = 0;
    Mat4 GetVPMatrix() const;
};