#pragma once
#include <cmath>
#include <iostream>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    float length() const { return std::sqrt(x*x + y*y + z*z); }
    Vector3 normalized() const {
        float len = length();
        if (len == 0) return Vector3(0,0,0);
        return Vector3(x/len, y/len, z/len);
    }
    void print() const { std::cout << "(" << x << ", " << y << ", " << z << ")\n"; }
};

class Vector4 {
public:
    float x, y, z, w;
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4 operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
    Vector4 operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
    Vector4 operator*(float scalar) const { return Vector4(x * scalar, y * scalar, z * scalar, w * scalar); }
    float length() const { return std::sqrt(x*x + y*y + z*z + w*w); }
    Vector4 normalized() const {
        float len = length();
        if (len == 0) return Vector4(0,0,0,0);
        return Vector4(x/len, y/len, z/len, w/len);
    }
    void print() const { std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")\n"; }
};