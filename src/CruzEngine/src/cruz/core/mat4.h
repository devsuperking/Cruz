#pragma once
#include <iostream>

struct Mat4 {
    float data[16];
    Mat4();

    static Mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
    static Mat4 Scale(float x, float y, float z);
    static Mat4 Translate(float x, float y, float z);

    Mat4 operator*(const Mat4& rhs) const;
    void MultiplyVec4(const float in[4], float out[4]) const;
    
    void Print() const;
};
