#include "mat4.h"

Mat4::Mat4() {
    for (int i = 0; i < 16; ++i) data[i] = 0.0f;
    data[0] = data[5] = data[10] = data[15] = 1.0f;
}

Mat4 Mat4::Ortho(float left, float right, float bottom, float top, float near, float far) {
    Mat4 m;
    m.data[0] = 2.0f / (right - left);
    m.data[5] = 2.0f / (top - bottom);
    m.data[10] = -2.0f / (far - near);
    m.data[12] = -(right + left) / (right - left);
    m.data[13] = -(top + bottom) / (top - bottom);
    m.data[14] = -(far + near) / (far - near);
    m.data[15] = 1.0f;

    return m;
}

Mat4 Mat4::operator*(const Mat4& rhs) const {
    Mat4 result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.data[col * 4 + row] =
                data[0 * 4 + row] * rhs.data[col * 4 + 0] +
                data[1 * 4 + row] * rhs.data[col * 4 + 1] +
                data[2 * 4 + row] * rhs.data[col * 4 + 2] +
                data[3 * 4 + row] * rhs.data[col * 4 + 3];
        }
    }
    return result;
}

void Mat4::MultiplyVec4(const float in[4], float out[4]) const {
    for (int i = 0; i < 4; ++i) {
        out[i] = data[i + 0] * in[0] + data[i + 4] * in[1] + data[i + 8] * in[2] + data[i + 12] * in[3];
    }
}

void Mat4::Print() const {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::cout << data[col * 4 + row] << " ";
        }
        std::cout << "\n";
    }
}
