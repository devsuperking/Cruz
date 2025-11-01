#include "camera.h"

Mat4 Camera::GetVPMatrix() const {
    return GetProjectionMatrix() * GetViewMatrix();
}